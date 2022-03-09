#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
    int sockfd; 
    int n, len; 
    char sendline[1000], recvline[1000]; 
    struct sockaddr_un servaddr, cliaddr; 
    
    if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
        perror("socket"); 
        exit(1);
    }
    
    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sun_family = AF_UNIX;
    strcpy(cliaddr.sun_path, "cccd");
    
     
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_UNIX;
    strcpy(servaddr.sun_path, "dddc");
	  if(connect(sockfd, (struct sockaddr *) &servaddr,
            SUN_LEN(&servaddr)) < 0){
        perror("bind");
        close(sockfd);
        exit(1);
    }
    
    printf("String => ");
    fgets(sendline, 1000, stdin);
   
    if(write(sockfd, sendline, strlen(sendline)+1) < 0){
        perror("send");
        close(sockfd);
        exit(1);
    }
   
    if((n = read(sockfd, recvline, 1000)) < 0){
        perror("receive");
        close(sockfd);
        exit(1);
    }
    recvline[n] = 0;
    printf("%s\n", recvline);
    close(sockfd);
    unlink("cccd");
    return 0;
}

