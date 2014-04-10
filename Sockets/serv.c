//CPSC405 Project 3. Frankie DePaola

#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <netdb.h>
#include "proj3.h"

int votes[20];
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
int fd;

void ctrlC(){
    //print the total votes and winner, then close.
    int i;
    printf("%d\n",numCandidates);
    printf("\n");
    for (i=0; i<numCandidates; i++){
        printf("%s finished with %d votes.\n",candidates[i].name,votes[i]);
    }
    errcheck = close(fd);
    if (errcheck < 0){
        printf("Error closing socket.\n");
        exit(1);
    }
    exit(0);
}

void readCandidates(){
    FILE *candidateList = fopen("candidates.txt","r");
    if (candidateList == NULL){
        printf("Input file not found. Please make sure there is a file in the current directory called \"candidates.txt\" and try again.\n");
        exit(EXIT_FAILURE);
    } else {
        fscanf(candidateList,"%d", &numCandidates);
        char nextLine[32];
        int i;
        fgets(nextLine,32,candidateList);
        char *nl;
        if ((nl=strchr(nextLine,'\n')) != NULL)
            *nl = '\0';
        for (i=0;i<numCandidates;i++){
            fgets(nextLine,32,candidateList);
            if ((nl=strchr(nextLine,'\n')) != NULL)
                *nl = '\0';
            strcpy(candidates[i].name,nextLine);
        }
    }
}

void* castVote(void* s){
    int sock = *((int *) s);
    int sel;
    char ack[14];
    errcheck = read(sock, &sel, sizeof(sel));
    if (errcheck < 0){
        printf("Error reading from socket.\n");
        exit(1);
    }
    errcheck = pthread_mutex_lock(&mtx);
    if (errcheck > 0){
        printf("Error locking mutex.\n");
        exit(EXIT_FAILURE);
    }
    votes[sel]++;
    printf("%d votes for %s.\n",votes[sel],candidates[sel].name);
    errcheck = pthread_mutex_unlock(&mtx);
    if (errcheck > 0){
        printf("Error unlocking mutex.\n");
        exit(EXIT_FAILURE);
    }
    strncpy(ack,"VOTE_TALLIED\n",sizeof(ack));
    errcheck = write(sock,ack,sizeof(ack));
    if (errcheck < 0){
        printf("Error writing acknowledgment.\n");
        exit(1);
    }
}

int main(int argc, char *argv[]){
    signal(SIGINT,ctrlC);
    readCandidates();
    int i;
    for(i=0; i<numCandidates; i++){
        votes[i] = 0;
    }
    struct sockaddr_in serverAddress, clientAddress;
    fd = socket(AF_INET,SOCK_STREAM,0);
    if (fd == -1){
        printf("Error creating socket.\n");
        return 1;
    }
    memset(&serverAddress,0,sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(54031);
    errcheck = bind(fd,(struct sockaddr *) &serverAddress, sizeof(serverAddress));
    if (errcheck == -1){
        printf("Error binding.\n");
        return 1;
    }
    errcheck = listen(fd,5);
    if (errcheck == -1){
        printf("Error listening.\n");
        return 1;
    }
    int clientSize = sizeof(clientAddress);
    printf("Now accepting connections.\n");
    while(1){
        int current = accept(fd, (struct sockaddr *) &clientAddress, &clientSize);
        if (current == -1){
            printf("Error accepting.\n");
            return 1;
        }
        pthread_t thread;
        int *tmp = malloc(sizeof(*tmp));
        *tmp = current;
        errcheck = pthread_create(&thread,NULL,&castVote,tmp);
        if (errcheck != 0){
            printf("Error spawning thread.\n");
            return 1;
        }
    }
    return 0;
}
