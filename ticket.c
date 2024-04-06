#include "ticket.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/wait.h>

void handleTickets(match *football_match, int semid) {
    int i, j, counter, requested_match_id, requested_tickets;
    int pid;

    // Semaphore operation structure
    struct sembuf op;

    // Loop through each process
    for (i = 0; i < 10; i++) {
        counter = rand(); // To get more random values

        if ((pid = fork()) == 0) {
            counter = i + 1;

            // Semaphore is 0
            // Only current process can write to shared memory
            op.sem_num = 0;
            op.sem_op = -1;
            op.sem_flg = 0;
            semop(semid, &op, 1);

            // Read from shared memory
            for (j = 0; j < 10; j++) {
                football_match[j] = football_match[j];
            }

            // Get random values
            requested_match_id = (rand() % 10) + 1;
            requested_tickets = (rand() % 3) + 1;
            printf("Results for client: %d\n", counter);
            printf("Requested MatchID: %d\n", requested_match_id);
            printf("Requested tickets: %d\n", requested_tickets);
            printf("Available tickets: %d \n", football_match[requested_match_id - 1].available_tickets);

            if (requested_tickets <= football_match[requested_match_id - 1].available_tickets) {
                // Write to shared memory
                football_match[requested_match_id - 1].available_tickets -= requested_tickets;

                // Print details
                printf("MatchID: %d\n", football_match[requested_match_id - 1].match_id);
                printf("Team 1: %s\n", football_match[requested_match_id - 1].team1);
                printf("Team 2: %s\n", football_match[requested_match_id - 1].team2);
                printf("Available tickets: %d\n", football_match[requested_match_id - 1].available_tickets);
                printf("Price: %f\n", football_match[requested_match_id - 1].price);
                printf("\n");
            } else {
                printf("Not enough tickets available!\n");
            }

            // Semaphore is 1
            op.sem_num = 0;
            op.sem_op = 1;
            op.sem_flg = 0;
            semop(semid, &op, 1);

            // Terminate the child process
            exit(0);
        } else {
            // Wait for child process to finish
            wait(NULL);
        }
    }
}
