#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

typedef struct match {
    int match_id;
    char* team1;
    char* team2;
    int available_tickets;
    float price;
} match;
 
int main(int argc, char* argv[]) {
    int i, j, key, shmid, pid, semid, counter, requested_match_id, requested_tickets;
    struct sembuf op;
    match *football_match, *sharedMemory;
    //allocate football match array
    football_match = (match *)malloc(sizeof(match) * 10);
    for (i = 0; i < 10; i++) {
        football_match[i].team1 = (char*)malloc(sizeof(char) * 120);
        football_match[i].team2 = (char*)malloc(sizeof(char) * 120);
    }
    //allocate shared memory
    sharedMemory = (match*)malloc(10 * sizeof(match));
    for (i = 0; i < 10; i++) {
        sharedMemory[i].team1 = (char*)malloc(sizeof(char) * 120);
        sharedMemory[i].team2 = (char*)malloc(sizeof(char) * 120);
    }
    //init shared memory
    key = ftok(".", 20);
    shmid = shmget(key, 10*sizeof(match), 0666 | IPC_CREAT);
    //attach to shared memory
    sharedMemory = (match*)shmat(shmid, NULL, 0);
    //init semaphore to 1
    semid = semget(IPC_PRIVATE, 1, S_IRWXU);
    semctl(semid, 0, SETVAL, 1);
 
    //initialize football_match array
    srand(time(0));
    for (i = 0; i < 10; i++) {
        football_match[i].match_id = i + 1;
        football_match[i].available_tickets = (rand() % 5) + 1;
    }
    strcpy(football_match[0].team1, "panaigialios");
    strcpy(football_match[0].team2, "panachaiki");
    strcpy(football_match[1].team1, "pao");
    strcpy(football_match[1].team2, "aek");
    strcpy(football_match[2].team1, "paok");
    strcpy(football_match[2].team2, "pao");
    strcpy(football_match[3].team1, "pas.giannena");
    strcpy(football_match[3].team2, "aek");
    strcpy(football_match[4].team1, "aek");
    strcpy(football_match[4].team2, "aris");
    strcpy(football_match[5].team1, "aris");
    strcpy(football_match[5].team2, "paok");
    strcpy(football_match[6].team1, "paok");
    strcpy(football_match[6].team2, "aek");
    strcpy(football_match[7].team1, "aek");
    strcpy(football_match[7].team2, "osfp");
    strcpy(football_match[8].team1, "pao");
    strcpy(football_match[8].team2, "osfp");
    strcpy(football_match[9].team1, "osfp");
    strcpy(football_match[9].team2, "lamia");
    football_match[0].price = 10.0;
    football_match[1].price = 12.4;
    football_match[2].price = 50.3;
    football_match[3].price = 23.7;
    football_match[4].price = 11.1;
    football_match[5].price = 21.5;
    football_match[6].price = 67.2;
    football_match[7].price = 32.1;
    football_match[8].price = 72.0;
    football_match[9].price = 45.8;
    //copy football match array to shared memory
    for (i = 0; i < 10; i++) {
        sharedMemory[i] = football_match[i];
    }
    if((pid=getpid()) > 0){
        //parent sets counter to 0
        counter = 0;
    }
    for(i=0;i<10;i++) {
        counter=rand(); //to get more random values
        if ((pid = fork()) == 0) {
            counter=i+1;
            //semaphore is 0
            //only current process can write to shared memory
            op.sem_num = 0;
            op.sem_op = -1;
            op.sem_flg = 0;
            semop(semid, &op, 1);
            //read from shared memory
            for (j = 0; j < 10; j++) {
                football_match[j] = sharedMemory[j];
            }
            //get random values
            requested_match_id = (rand() % 10) + 1;
            requested_tickets = (rand() % 3) + 1;
            printf("Results for client: %d\n", counter);

            printf("Requested MatchID: %d\n", requested_match_id);
            printf("Requested tickets: %d\n", requested_tickets);
            printf("Available tickets: %d \n", football_match[requested_match_id - 1].available_tickets);
            if (requested_tickets <= football_match[requested_match_id - 1].available_tickets) {
                
                //write to shared memory
                football_match[requested_match_id - 1].available_tickets -= requested_tickets;
                sharedMemory[requested_match_id - 1] = football_match[requested_match_id - 1];
                //print details
                printf("MatchID: %d\n", football_match[requested_match_id - 1].match_id);
                printf("Team 1: %s\n", football_match[requested_match_id - 1].team1);
                printf("Team 2: %s\n", football_match[requested_match_id - 1].team2);
                printf("Available tickets: %d\n", football_match[requested_match_id - 1].available_tickets);
                printf("Price: %f\n", football_match[requested_match_id - 1].price);
                printf("\n");

            } else {
                printf("Not enough tickets available!\n");
            }    
            //semaphore is 1
            op.sem_num = 0;
            op.sem_op = 1;
            op.sem_flg = 0;
            semop(semid, &op, 1);
            //terminate the child process
            exit(0); 
        } else {
            //wait for child process to finish 
            wait(NULL);
        }
    }
    //delete shared memory
    shmctl(shmid, IPC_RMID, NULL);
    shmdt(sharedMemory);
    //delete semaphore
    semctl(semid, 0, IPC_RMID);
    //free football match array
    for (i = 0; i < 10; i++) {
        free(football_match[i].team1);
        free(football_match[i].team2);
    }
    free(football_match);
 
    return 0;
}