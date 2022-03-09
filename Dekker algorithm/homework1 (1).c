#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<errno.h>
#include<stdlib.h>
#include<stdbool.h>

int main(int argc, char *argv[], char *envp[]){
	int *turn;
	bool *flag;
//	flag[0] = false;
//	flag[1] = false;
//	*turn = 0;
	int *array;/*pointer on shared memory*/
	int shmid;/*IPC-descriptor*/
	int new = 1;
	char pathname[] = "homework2.c";/*name, used for generating the key*/
	key_t key;/*IPC-key*/
	if((key = ftok(pathname, 0)) < 0)
	{
		printf("Can\'t generate key\n");
		exit(-1);
	}
	
	if((shmid = shmget(key, 3*sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0)
	{
		if(errno!=EEXIST)
		{
			printf("Can\'t create shared memory\n");
			return -1;
		}
		else
		{
			if((shmid = shmget(key, 3*sizeof(int), 0)) < 0)
			{
				printf("Can\'t find shared memory\n");
				return -1;
			}
			new = 0;
		}

	}

	if((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)){
		printf("Can\'t attach shared memory\n");
		exit(-1);

	}
	flag =(bool *)shmat(shmid,NULL,0);
        turn = (int *)shmat(shmid,NULL,0);
	
	
	if(new){
		array[1] = 1;
		array[2] = 0;
		array[3] = 1; 
	}
	else
	{
	flag[0] = true;
        while (flag[1] == true) 
	    {
            if (*turn == 1) {
                flag[0] = false;
                while(*turn == 1) {}
                flag[0] = true;
            }
        }
	    

     // критическая секция
 
		array[1] +=1;
		for(long i = 0; i < 1000000000L; i++);
		array[3] +=1;
	*turn = 1;
	flag[0] = false;

	}

	printf("Program 1 was spawn %d times, program 2 - %d times, total - %d times\n", array[1], array[2], array[3]);

	if(shmdt(array) < 0){
		printf("Can\'t detach shared memory\n");
		exit(-1);

	}
	shmdt(turn);
        shmdt(flag);
	return 0;
}
