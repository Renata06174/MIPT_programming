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

	FILE *wash_time;
	if ((wash_time = fopen("Wash_time.txt", "r")) == NULL) {
		printf("Error, couldn't open the file\n");
		return 2;
	}

	char dish_type[50];
	char d_type[50];
	int dish_number;
	int dish_time;
	/* Length of the message to be sent*/
	int len;
	
	struct mymsgbuf {
                long mtype;
                char dish_type[50];
        } mybuf;

	struct table {
		long mtype;
		int used_place;
	} table;

	int maxlen = sizeof(int);

	while ((fscanf(dishes, "%s : %d", dish_type, &dish_number)) && (feof(dishes) == 0)) {
		/* printf("Dish_type: %s, dish_number: %d\n", dish_type, dish_number); */
		rewind(wash_time);

	
		while ((fscanf(wash_time, "%s : %d", d_type, &dish_time)) && (feof(wash_time) == 0)) {
			if (strcmp(d_type, dish_type) == 0) {
				break;
			}
		}

		while (dish_number != 0) {
			mybuf.mtype = 1;
			strcpy(mybuf.dish_type, dish_type);
			len = strlen(mybuf.dish_type) + 1;
			sleep(dish_time);
			dish_number--;

			if (msgrcv(msqid_2, (struct table *) &table, maxlen, 0, 0) < 0) {
				printf("Can\'t receive message from queue\n");
				exit(-1);
			}

                        printf("Washed the %s\n", dish_type);


			
			if (msgsnd(msqid_1, (struct mymsbuf *) &mybuf, len, 0) < 0) {
				printf("Can\'t send message to queue\n");
				msgctl(msqid_1, IPC_RMID, (struct msqid_ds *) NULL);
				exit(-1);
			}
		}
	}

	
	mybuf.mtype = LAST_MESSAGE;
	len = 0;

	if (msgsnd(msqid_1, (struct mymsbuf *) &mybuf, len, 0) < 0) {
		printf("Can\'t send message to queue\n");
		msgctl(msqid_1, IPC_RMID, (struct msqid_ds *) NULL);
		exit(-1);
	}

	fclose(dishes);
	fclose(wash_time);
	return 0;
}
