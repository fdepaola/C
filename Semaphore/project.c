//CPSC405 Project 2. Frankie DePaola

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

int errcheck = 0;
struct potion {
    char name[32];
    int available;
};
struct memStruct {
    int numPotionTypes;
    sem_t sharedMemSem;
    sem_t apSem;
    int activeProcesses;
    struct potion potions[10];
};
struct memStruct *sm;
off_t memSize = sizeof(struct memStruct);

void purchaser(){
    int i;
    int input = 13;
    while(input != 99){
        for(i=0; i<sm->numPotionTypes; i++)
            printf("%d: Buy %s potions\n",i,&sm->potions[i].name);
        printf("11: Print the current inventory\n");
        printf("99: Exit\n");
        scanf("%d",&input);
        if(input == 11){
            for(i=0; i<sm->numPotionTypes; i++)
                printf("%s: %d in stock\n",&sm->potions[i].name,sm->potions[i].available);
        } else {
            if(input == 99){
                printf("Thank you. Come again.\n");
                return;
            } else {
                struct potion *potSell = &sm->potions[input];
                printf("How many?\n");
                scanf("%d",&input);
                if(input <= potSell->available){
                    errcheck = sem_wait(&sm->sharedMemSem);
                    if (errcheck != 0){
                        printf("Error on sem_wait. Exiting.");
                        return;
                    }
                    potSell->available -= input;
                    errcheck = sem_post(&sm->sharedMemSem);
                    if (errcheck != 0){
                        printf("Error on sem_post. Exiting.");
                        return;
                    }
                    printf("You receive %d potions.\n",input);
                }
                else{
                    printf("We didn't have %d in stock, so you only got %d.\n",input,potSell->available);
                    errcheck = sem_wait(&sm->sharedMemSem);
                    if (errcheck != 0){
                        printf("Error on sem_wait. Exiting.");
                        return;
                    }
                    potSell->available = 0;
                    errcheck = sem_post(&sm->sharedMemSem);
                    if (errcheck != 0){
                        printf("Error on sem_post. Exiting.");
                        return;
                    }
                }
            }
        }
    }
    return;
}

stocker(){
    int i;
    int input = 13;
    while(input != 99){
        for(i=0; i<sm->numPotionTypes; i++)
            printf("%d: Restock %s potions\n",i,&sm->potions[i].name);
        printf("11: Print the current inventory\n");
        printf("99: Exit\n");
        scanf("%d",&input);
        if(input == 11){
            for(i=0; i<sm->numPotionTypes; i++)
                printf("%s: %d in stock\n",&sm->potions[i].name,sm->potions[i].available);
        } else {
            if(input == 99){
                printf("Thanks for the supplies, bub.\n");
                return;
            } else {
                struct potion *potSell = &sm->potions[input];
                printf("How many?\n");
                scanf("%d",&input);
                errcheck = sem_wait(&sm->sharedMemSem);
                if (errcheck != 0){
                    printf("Error on sem_wait. Exiting.");
                    return;
                }
                potSell->available += input;
                errcheck = sem_post(&sm->sharedMemSem);
                if (errcheck != 0){
                    printf("Error on sem_post. Exiting.");
                    return;
                }
                printf("Added %d potions to inventory.\n",input);
            }
        }
    }
    return;
}

int main(int argc, char *argv[]){
    //Attempt to create shared memory object. If it succeeds, initialize everything that will be shared
    int fd = shm_open("/potshop",O_CREAT|O_RDWR|O_EXCL,0777);
    if (fd >= 0){  //Shared memory did not already exist, so we fill it
        errcheck = ftruncate(fd,memSize);
        if (errcheck != 0){
            printf("Error truncating shared memory. Exiting.\n");
            return 1;
        }
        void *mem = mmap(NULL,memSize,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
        if (mem == MAP_FAILED){
            printf("Error mapping shared memory. Exiting.\n");
            return 1;
        }
        sm = (struct memStruct*)mem;
        errcheck = sem_init(&sm->apSem,0,1);
        if (errcheck != 0){
            printf("Error initializing process counter semaphore. Exiting.\n");
            return 1;
        }
        sm->activeProcesses = 1;
        errcheck = sem_init(&sm->sharedMemSem,0,1);
        if (errcheck != 0){
            printf("Error initializing shared memory semaphore. Exiting.\n");
            return 1;
        }
        //Create potionList using input file
        FILE *potionList = fopen("inventory_input.txt","r");
        if (potionList == NULL){
            printf("Input file not found. Please make sure there is a file in the current directory called \"inventory_input.txt\" and try again.\n");
            return 1;
        }
        sm->numPotionTypes = fgetc(potionList) - '0';
        int i;
        char nextLine[40];
        struct potion potArray[10];
        fgets(nextLine,40,potionList);  //Read the next line from input file.
        for (i=0;i<sm->numPotionTypes;i++){
            fgets(nextLine,40,potionList);  //Read the next line from input file.
            sscanf(nextLine,"%s %d",&potArray[i].name,&potArray[i].available); //Parse the line and fill in a potion.
        }
        //Add potions array to our shared memory
        memcpy(&sm->potions,&potArray,sizeof(potArray));
    } else {    //shared memory object did exist. so open it for read/write!
        fd = shm_open("/potshop",O_RDWR,0777);
        if (fd == -1){
            printf("Error opening shared memory. Exiting.\n");
            return 1;
        }
        void *mem = mmap(NULL,memSize,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
        if (mem == MAP_FAILED){
            printf("Error mapping shared memory. Exiting.\n");
            return 1;
        }
        sm = (struct memStruct*)mem;
        sm->activeProcesses++;
    }
    printf("Welcome to Uncle Trapspringer and Fizban the Fabulous's Potion Emporium!\n");
    char input = '3';
    while(input != '1' && input != '2'){
        printf("Are you a customer or stocker?\n1. Customer\n2. Stocker\n");
        scanf("%c",&input);
    }
    if(input == '1'){
        purchaser();
    }
    else if(input == '2'){
        stocker();
    }
    sm->activeProcesses--;
    //Destroy all resources only when they're not needed any longer. ie: process counter is 0
    if (sm->activeProcesses == 0){
        errcheck = sem_destroy(&sm->sharedMemSem);
        if (errcheck != 0){
            printf("Error destroying shared memory semaphore. Exiting.\n");
            return 1;
        }
        errcheck = sem_destroy(&sm->apSem);
        if (errcheck != 0){
            printf("Error destroying active process semaphore. Exiting.\n");
            return 1;
        }
        errcheck = shm_unlink("/potshop");
        if (errcheck != 0){
            printf("Error unlinking shared memory. Exiting.\n");
            return 1;
        }
    }
    return 0;
}
