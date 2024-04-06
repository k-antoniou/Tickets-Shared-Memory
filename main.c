#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "match.h"
#include "ticket.h"

int main(int argc, char* argv[]) {
    int key, shmid, semid;
    int pid;
    match *football_match, *sharedMemory;

    // Allocate memory for football_match array
    football_match = (match *)malloc(sizeof(match) * 10);

    // Initialize football matches
    initializeMatches(football_match);

    // Allocate shared memory
    key = ftok(".", 20);
    shmid = shmget(key, 10 * sizeof(match), 0666 | IPC_CREAT);

    // Attach to shared memory
    sharedMemory = (match*)shmat(shmid, NULL, 0);

    // Copy football match array to shared memory
    for (int i = 0; i < 10; i++) {
        sharedMemory[i] = football_match[i];
    }

    // Initialize semaphore
    semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    semctl(semid, 0, SETVAL, 1);

    // Handle tickets
    handleTickets(football_match, semid);

    // Detach and delete shared memory
    shmdt(sharedMemory);
    shmctl(shmid, IPC_RMID, NULL);

    // Delete semaphore
    semctl(semid, 0, IPC_RMID);

    // Free memory
    for (int i = 0; i < 10; i++) {
        free(football_match[i].team1);
        free(football_match[i].team2);
    }
    free(football_match);

    return 0;
}
