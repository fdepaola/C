#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include "proj3.h"

int errcheck;

void readCandidates(){
    FILE *candidateList = fopen("candidates.txt","r");
    if (candidateList == NULL){
        printf("Input file not found. Please make sure there is a file in the current directory called \"candidates.txt\" and try again.\n");
        exit(EXIT_FAILURE);
    } else {
        fscanf(candidateList,"%d",&numCandidates);
        int i;
        char nextLine[32];
        fgets(nextLine,32,candidateList);
        char *nl;
        if((nl=strchr(nextLine,'\n')) != NULL)
            *nl = '\0';
        for (i=0;i<numCandidates;i++){
            fgets(nextLine,32,candidateList);
            if((nl=strchr(nextLine,'\n')) != NULL)
                *nl = '\0';
            strcpy(candidates[i].name,nextLine);
        }
    }
}

int main(){
    readCandidates();
    int fd = 0;
    struct sockaddr_in serverAddress;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1){
        printf("Error creating socket.\n");
        return 1;
    }
    memset(&serverAddress,0,sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    struct hostent *he;
    he = gethostbyname("cayenne.cpsc.umw.edu");
    serverAddress.sin_addr = *((struct in_addr *)he->h_addr);
    serverAddress.sin_port = htons(54031);
    errcheck = connect(fd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (errcheck == -1){
        printf("Error connecting.\n");
        printf("%s\n",strerror(errno));
        return -1;
    }
    int i;
    for (i=0; i<numCandidates;i++){
        printf("%d. %s\n",i,candidates[i].name);
    }
    int input=99;
    while(input >= numCandidates || input < 0){
        printf("Cast your vote (by number). ");
        scanf("%d",&input);
    }
    errcheck = write(fd,&input,sizeof(input));
    if (errcheck < 0){
        printf("Error writing to socket.");
        return 1;
    }
    char ack[14];
    errcheck = read(fd,ack,sizeof(ack));
    if (errcheck < 0){
        printf("Error reading from socket.");
        return 1;
    }
    printf("%s",ack);
    errcheck = close(fd);
    if (errcheck < 0){
        printf("Error closing socket.\n");
        return 1;
    }
    return 0;
}
