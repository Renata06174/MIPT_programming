#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

int sem;
void signal_handler1(int nsig){
	sem = -1;
}
void signal_handler2(int nsig){
        sem = 1 ;
}


int main(int argc, char *argv[], char *envp[]){
	int fd[2], result;
        size_t size,size1;
	char string[] = "Hello, world p !";
        	char resstring[30];
	char string1[] = "Hello world c !";
	
	if( signal( SIGUSR1, signal_handler1) == SIG_ERR  ){
        	printf("Pærent: Unable to create handler for SIGUSR1\n");
	}
 
	if( signal( SIGUSR2, signal_handler2) == SIG_ERR  ){
        	printf("Pærent: Unable to create handler for SIGUSR2\n");
    	}
        
	if(pipe(fd)<0){
                printf("Can\'t create pipe\n");
                exit(-1);
        }
	result = fork();
	if(result<0){
		printf("Can\'t fork child\n");
		exit(-1);
	}
	  else if (result  > 0) {
                size = write(fd[1], string, 30);
       		kill(getpid(),SIGUSR1);
		while(sem !=  1){}
                size1 = read(fd[0],resstring,30);
                printf("I'm a parent and child said\n");
                printf("%s\n", resstring);
        }
	else {
		if (sem != -1){
			for(;;);
		}
        	size = read(fd[0], resstring, 30);
		size1 = write(fd[1],string1,30);
		kill(getppid(),SIGUSR2);
       		printf("I'm a child and  parent said\n");
		printf("%s\n", resstring);
        	close(fd[0]);
		close(fd[1]);
		
	}

        return 0;
}
