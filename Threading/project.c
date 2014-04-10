//CPSC405 Project 1. Frankie DePaola

#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <time.h>
#include <stdbool.h>

int numHeroes = 1, numMonsters = 1;
int errcheck = 0;
sem_t printSem;
struct fighter {
    char name[20];
    int hitPoints;
    int damage;
    sem_t hpSem;
    pthread_t fighterThread;
};

struct fighter heroes[4];
struct fighter monsters[7];

//On a successful hit (41% chance) deducts the attack damage of attacker from defender's hp.
void fight(struct fighter* attacker, struct fighter *defender){
    while (attacker->hitPoints > 0 && defender->hitPoints > 0){
        int dieRoll = rand() % 100;
        if (dieRoll < 41){
            errcheck = sem_wait(&defender->hpSem);
            if (errcheck != 0){
                printf("Error locking Semaphore.\n");
                return;
            }
            errcheck = sem_wait(&printSem);
            if (errcheck != 0){
                printf("Error locking Semaphore.\n");
                return;
            }
            if(attacker->hitPoints > 0){
                defender->hitPoints -= attacker->damage;
                printf("%s swings for %d, bringing %s down to %d hit points\n",attacker->name,attacker->damage,defender->name,defender->hitPoints);
            }
            errcheck = sem_post(&printSem);
            if (errcheck != 0){
                printf("Error unlocking Semaphore.\n");
                return;
            }
            errcheck = sem_post(&defender->hpSem);
            if (errcheck != 0){
                printf("Error unlocking Semaphore.\n");
                return;
            }
        } 
    }
}

//Returns true iff no heroes have remaining hp.
bool areAllHeroesDead(){
    bool allDead = true;
    int i;
    for(i=0;i<numHeroes;i++)
        if(heroes[i].hitPoints > 0)
            allDead = false;
    return allDead;
}

//Chooses a random (living) hero for a monster to attack. If the hero is defeated, chooses a new target.
void* chooseHero(void* m){
    if (areAllHeroesDead())
        return;
    struct fighter* monster = m;
    if (monster->hitPoints > 0){
        int target = rand() % numHeroes;
        while (heroes[target].hitPoints < 1)
            target = rand() % numHeroes;
        struct fighter* h = &heroes[target];
        fight(monster,h);
        if (areAllHeroesDead())
            return;
        else
            chooseHero(m);
    }
}

//Returns true iff no monsters have remaining hp.
bool areAllMonstersDead(){
    bool allDead = true;
    int i;
    for(i=0;i<numMonsters;i++)
        if(monsters[i].hitPoints > 0)
            allDead = false;
    return allDead;
}

//Chooses a random (living) monster for a hero to attack. If the monster is defeated, chooses a new target.
void* chooseMonster(void* h){
    if (areAllMonstersDead())
        return;
    struct fighter* hero = h;
    int target = rand() % numMonsters;
    if (hero->hitPoints > 0){
        while (monsters[target].hitPoints < 1)
            target = rand() % numMonsters;
        struct fighter* m = &monsters[target];
        printf("hero %s chooses monster %s, which has %d hp\n",hero->name,m->name,m->hitPoints);
        fight(hero,m);
        if (areAllMonstersDead())
            return;
        else
            chooseMonster(h);
    }
}

int main(int argc, char *argv[]){
    errcheck = sem_init(&printSem,0,1);
    if (errcheck !=0)
        printf("Error initializing print Semaphore.\n");
    //Seed for random numbers
    srand(time(NULL));
    FILE *battleList = fopen("simulator.txt","r");
    if (battleList == NULL){
        printf("Input file not found. Please make sure there is a file in the current directory called \"simulator.txt\" and try again.\n");
        //Error
        return 1;
    } else {
        //All good. Start reading and generating combatants.
        numHeroes = fgetc(battleList) - '0';
        int i;
        char nextLine[40];
        fgets(nextLine,40,battleList);  //Read the next line from input file.
        for (i=0;i<numHeroes;i++){
            fgets(nextLine,40,battleList);  //Read the next line from input file.
            sscanf(nextLine,"%s %d %d",&heroes[i].name,&heroes[i].hitPoints,&heroes[i].damage); //Parse the line and fill in a hero.
            errcheck = sem_init(&heroes[i].hpSem,0,1);
            if (errcheck != 0){
                printf("Error initializing Semaphore.\n");
                return 1;
            }
        }
        numMonsters = fgetc(battleList) - '0';
        fgets(nextLine,40,battleList);  //Read the next line from input file.
        for(i=0;i<numMonsters;i++){
            fgets(nextLine,40,battleList);   //Read the next line from input file.
            sscanf(nextLine,"%s %d %d",&monsters[i].name,&monsters[i].hitPoints,&monsters[i].damage);   //Parse the line and fill in a monster.
            errcheck = sem_init(&monsters[i].hpSem,0,1);
            if (errcheck != 0){
                printf("Error initializing Semaphore.\n");
                return 1;
            }
        }
        
        //After all the fighters are generated, spawn a thread for each and send them off to do epic battle!
        for(i=0;i<numHeroes;i++){
            errcheck = pthread_create(&heroes[i].fighterThread,NULL,chooseMonster,&heroes[i]);
            if (errcheck != 0){
                printf("Error creating Pthread.\n");
                return 1;
            }
        }
        for(i=0;i<numMonsters;i++){
            errcheck = pthread_create(&monsters[i].fighterThread,NULL,chooseHero,&monsters[i]);
            if (errcheck != 0){
                printf("Error creating Pthread.\n");
                return 1;
            }
        }

        //Main will wait for the dust to clear before proclaiming a decisive end.
        for(i=0;i<numHeroes;i++){
            errcheck = pthread_join(heroes[i].fighterThread,NULL);
            if (errcheck != 0){
                printf("Error joining Pthread.\n");
                return 1;
            }
        }
        for(i=0;i<numMonsters;i++){
            errcheck = pthread_join(monsters[i].fighterThread,NULL);
            if (errcheck != 0){
                printf("Error joining Pthread.\n");
                return 1;
            }
        }

        errcheck = sem_wait(&printSem);
        if (errcheck != 0){
            printf("Error locking Semaphore.\n");
            return 1;
        }
        printf("To the victors, the spoils. Your survivors:\n");

        for (i=0; i<numHeroes; i++){
            if (heroes[i].hitPoints > 0){
                printf("%s with %d health remaining. ",heroes[i].name,heroes[i].hitPoints);
            }
        }

        for (i=0; i<numMonsters; i++){
            if (monsters[i].hitPoints > 0){
                printf("%s with %d health remaining. ",monsters[i].name,monsters[i].hitPoints);
            }
        }

        printf("\n");
        errcheck = sem_post(&printSem);
        if (errcheck != 0){
            printf("Error unlocking Semaphore.\n");
            return 1;
        }
        return 0;
    }
}
