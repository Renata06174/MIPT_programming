#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
int main(){
    int sockfd; 
    int fd;
    int clilen, n; 
    char line[1000]; 
    struct sockaddr_un servaddr, cliaddr; 
    if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
        perror("socket"); 
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_UNIX;
    strcpy(servaddr.sun_path, "dddc");
    
    if(bind(sockfd, (struct sockaddr *) &servaddr,
            SUN_LEN(&servaddr)) < 0){
        perror("bind");
        close(sockfd);
        exit(1);
    }
    if(listen(sockfd,5) <0){
	    perror(NULL);
	    close(sockfd);
	    exit(1);
    }
    while(1){
        
        clilen = sizeof(struct sockaddr_un);
     	fd = accept(sockfd,(struct sockaddr *) &cliaddr,&clilen);   
        if((n = read(fd, line, 999)) < 0){
            perror("receive");
            close(fd);
            exit(1);
        }
        
        printf("%s\n", line);
      
        if(write(fd, line, strlen(line)) < 0){
            perror("send");
            close(fd);
            exit(1);
        }
	close(fd);	
    }
    return 0;
}

