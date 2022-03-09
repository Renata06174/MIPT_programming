#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

int main(int argc, char *argv[], char *envp[]){
        int fd[2], result;
        size_t size,size1;
	char string[] = "Hello world p!";
        char resstring[30];
	char string1[] = "Hello world ch !";
	key_t key, key1; /*IPC-key*/
	long i;
	char pathname[] = "dzpipe.c";
	struct sembuf mybuf;
	int semid, shmid1;

        if(pipe(fd)<0){
                printf("Can\'t create pipe\n");
                exit(-1);
        }
	result = fork();
	if(result<0){
		printf("Can\'t fork child\n");
		exit(-1);
	}
	else if(result>0){
		
        size = write(fd[1], string, 30);

	if ((key = ftok(pathname, 0)) < 0) {
		printf("Can\'t generate key\n");
		exit(-1);
	}
 
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
      printf("Can\'t get semid\n");
      exit(-1);
    }

    if ((shmid1 = shmget(key, 3 * sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) <
        0) {
      if (errno != EEXIST) {
        printf("Can\'t create shared memory\n");
        exit(-1);
      } else {
        if ((shmid1 = shmget(key, 3 * sizeof(int), 0)) < 0) {
          printf("Can\'t find shared memory\n");
          exit(-1);
        }
      }
    }

	mybuf.sem_op = -1;
	mybuf.sem_flg = 0;
	mybuf.sem_num = 0;
	if (semop(semid, &mybuf, 1) < 0) {
		printf("Can\'t wait for condition\n");
		exit(-1);
	}
	size1 = read(fd[0], resstring, 30);
	printf("I'm a parent and child said\n");
	printf("%s\n", resstring);
	}
	else{
	size = read(fd[0], resstring, 30);
	size1 = write(fd[1], string1, 30);
	if ((key = ftok(pathname, 0)) < 0) {
      printf("Can\'t generate key\n");
      exit(-1);
    }
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
      printf("Can\'t get semid\n");
      exit(-1);
    }

    if ((shmid1 = shmget(key, 3 * sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) <
        0) {
      if (errno != EEXIST) {
        printf("Can\'t create shared memory\n");
        exit(-1);
      } else {
        if ((shmid1 = shmget(key, 3 * sizeof(int), 0)) < 0) {
          printf("Can\'t find shared memory\n");
          exit(-1);
        }
      }
    }
	mybuf.sem_op = 1;
	mybuf.sem_flg = 0;
	mybuf.sem_num = 0;
	if (semop(semid, &mybuf, 1) < 0) {
		printf("Can\'t wait for condition\n");
		exit(-1);
		}

       
	printf("I'm a child and  parent said\n");
	printf("%s\n", resstring);
	close(fd[0]);
	close(fd[1]);
	}
        return 0;
}
