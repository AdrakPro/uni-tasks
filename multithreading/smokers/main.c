#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_SMOKERS 3

enum { TOBACCO, PAPER, MATCHES };

sem_t smokers[NUM_SMOKERS];
sem_t agent_sem;

void* agent(void* arg) {
    while (1) {
        sem_wait(&agent_sem); // Wait for a smoker to finish

        int choice = rand() % NUM_SMOKERS;
        printf("Agent provides %s and %s.\n",
               choice == TOBACCO ? "paper" : "tobacco",
               choice == MATCHES ? "paper" : "matches");

        sem_post(&smokers[choice]); // Notify the appropriate smoker
        sleep(1);
    }
    return NULL;
}

void* smoker(void* arg) {
    int smoker_type = *(int*)arg;

    while (1) {
        sem_wait(&smokers[smoker_type]); // Wait for ingredients

        printf("Smoker %d with %s is smoking.\n", smoker_type + 1,
               smoker_type == TOBACCO ? "tobacco" :
               smoker_type == PAPER ? "paper" : "matches");

        sleep(1);
        printf("Smoker %d ended smoking.\n", smoker_type + 1);
        sem_post(&agent_sem); // Notify agent
    }
    return NULL;
}

int main() {
    srand(time(NULL));

    pthread_t agent_thread, smoker_threads[NUM_SMOKERS];
    int smoker_types[NUM_SMOKERS] = { TOBACCO, PAPER, MATCHES };

    sem_init(&agent_sem, 0, 1);
    for (int i = 0; i < NUM_SMOKERS; i++)
        sem_init(&smokers[i], 0, 0);

    pthread_create(&agent_thread, NULL, agent, NULL);

    for (int i = 0; i < NUM_SMOKERS; i++)
        pthread_create(&smoker_threads[i], NULL, smoker, &smoker_types[i]);

    pthread_join(agent_thread, NULL);
    for (int i = 0; i < NUM_SMOKERS; i++)
        pthread_join(smoker_threads[i], NULL);

    for (int i = 0; i < NUM_SMOKERS; i++)
        sem_destroy(&smokers[i]);
    sem_destroy(&agent_sem);

    return 0;
}
