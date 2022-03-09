#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
 
int main(){
	pid_t pid,ppid;
	int child1 = 0;
	int child2 = 0;
	int grand1 = 0;
	int grand2 = 0;
	int grand3 = 0;
	pid = fork();
	child1 = pid;
	if (pid != 0){
		pid = fork();
		if(pid == 0){
			printf("мой PID: %d мои дети: %d %d\n",getppid(), child1, getpid());
			sleep(2);
			pid = fork();
			grand1 = pid;
			if (pid != 0) {
				pid = fork();
				if (pid == 0){
					printf("мой PID: %d мои дети: %d %d\n",getppid(),grand1, getpid());
					sleep(3);
					exit(0);
				}
		} exit(0);
		
			
	}
		
	}
	if(pid == 0){
		pid = fork();
		grand2 = pid;
		if (pid != 0){
			pid = fork();
			grand3 = pid;
			if (pid != 0){
				pid = fork();
				if(pid == 0){
					printf("мой PID: %d и мои дети:%d %d  %d\n",getppid(), grand2,grand3, getpid());
					sleep(3);
					exit(0);
				}
			
			}exit(0);
		
		}exit(0);
		
	}

	sleep(3);
	exit(0);
}
