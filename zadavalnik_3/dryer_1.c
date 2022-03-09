#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define TABLE_LIMIT 3
#define LAST_MESSAGE 255

int main()
{
	
	int msqid_1, msqid_2;
	
	char pathname_1[] = "washer.c";
	char pathname_2[] = "dryer.c";
	key_t key_1, key_2;
	if ((key_1 = ftok(pathname_1, 0)) < 0) {
		printf("Can\'t generate key\n");
		exit(-1);
	}
	if ((key_2 = ftok(pathname_2, 0)) < 0) {
                printf("Can\'t generate key\n");
                exit(-1);
        }

	
	if ((msqid_1 = msgget(key_1, 0666 | IPC_CREAT)) < 0) {
		printf("Can\'t get msqid\n");
		exit(-1);
	}
	if ((msqid_2 = msgget(key_2, 0666 | IPC_CREAT)) < 0) {
                printf("Can\'t get msqid\n");
                exit(-1);
    }
	FILE *dishes;
	if ((dishes = fopen("Dishes.txt", "r")) == NULL) {
		printf("Error, couldn't open the file\n");
		return 1;
	}

	FILE *dry_time;
	if ((dry_time = fopen("Dry_time.txt", "r")) == NULL) {
		printf("Error, couldn't open the file\n");
		return 2;
	}

	char d_type[50];
	int dish_number;
	int dish_time;
	
	int len = sizeof(int);
	
	struct mymsgbuf {
                long mtype;
                char dish_type[50];
        } mybuf;

	struct table {
		long mtype;
		int used_place;
	} table;

	
	int maxlen = 51;
	table.mtype = 2;
	table.used_place = 1;

	for (int i = 0; i < TABLE_LIMIT; i++) {
		if (msgsnd(msqid_2, (struct table *) &table, len, 0) < 0) {
			printf("Can\'t send message to queue\n");
			msgctl(msqid_2, IPC_RMID, (struct msqid_ds *) NULL);
			exit(-1);
		}
	}

	while (1) {
		if (msgrcv(msqid_1, (struct mymsgbuf *) &mybuf, maxlen, 0, 0) < 0) {
			printf("Can\'t receive message from queue\n");
			exit(-1);
		}

		if (mybuf.mtype == LAST_MESSAGE) {
			printf("End\n");
			msgctl(msqid_1, IPC_RMID,(struct msqid_ds *) NULL);
			msgctl(msqid_2, IPC_RMID,(struct msqid_ds *) NULL);
			break;
		}

		printf("I took %s\n", mybuf.dish_type);

		if (msgsnd(msqid_2, (struct table *) &table, len, 0) < 0) {
			printf("Can\'t send message to queue\n");
			msgctl(msqid_2, IPC_RMID, (struct msqid_ds *) NULL);
			exit(-1);
		}

		rewind(dry_time);

	
		while ((fscanf(dry_time, "%s : %d", d_type, &dish_time)) && (feof(dry_time) == 0)) {
			if (strcmp(d_type, mybuf.dish_type) == 0) {
				break;
			}
		}
		sleep(dish_time);
		printf("Wiped the %s\n", mybuf.dish_type);
	}

	fclose(dishes);
	fclose(dry_time);
	return 0;
}
