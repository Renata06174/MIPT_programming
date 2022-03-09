#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/wait.h>
int maxprog = 0;
void handler(int a){
	wait(NULL);
	maxprog+=1;
}

int main(int argc, char **argv){
	char **arg=(char **) malloc(2*sizeof(char*));
        arg[0]=(char *) malloc(100*sizeof(char));   
        arg[1]=NULL;
	pid_t pid;
	char proga[100];
	maxprog=atoi(argv[1]);			   		 
	signal(SIGCHLD,handler);
	char name[100];
	while(1)
	{		  
		if(maxprog==0){                         
			printf("ввод окончен макс количество программ введено\n");
			free(arg[0]);
			free(arg);
			break;
		}
		if(maxprog!=0){
		scanf("%s",proga);
                strcpy(arg[0],proga);
		maxprog-=1;
		int pip=fork();                     
		if(pip==0){                             
			pip = execvp(arg[0],arg);
		}
		
	}
	}
	
	return 0;
}
