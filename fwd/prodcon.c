#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>

#define BUFFER_SIZE 10

pthread_mutex_t mutex;
sem_t empty, full;
int buffer[BUFFER_SIZE];
int counter = 0;
pthread_t tid1[5], tid2[5];

void *producer();
void *consumer();
void insert_item(int);
int remove_item();

void initialize() {
    pthread_mutex_init(&mutex, NULL);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);
}

void *producer() {
    int item, wait_time;
    wait_time = rand() % 5;
    sleep(wait_time % 5);
    item = rand() % 10;

    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    printf("Producer produces %d\n\n", item);
    insert_item(item);

    pthread_mutex_unlock(&mutex);
    sem_post(&full);
}

void *consumer() {
    int item, wait_time;
    sem_wait(&full);
    pthread_mutex_lock(&mutex);

    item = remove_item();
    printf("Consumer consumes %d\n\n", item);

    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
}

void insert_item(int item) {
    buffer[counter++] = item;
}

int remove_item() {
    return buffer[--counter];
}

int main() {
    int n1, n2;
    int i;

    printf("Enter number of Producers: ");
    scanf("%d", &n1);
    printf("Enter number of Consumers: ");
    scanf("%d", &n2);

    initialize();

    for (int i = 0; i < n1; i++) {
        pthread_create(&tid1[i], NULL, producer, NULL);
    }

    for (int i = 0; i < n2; i++) {
        pthread_create(&tid2[i], NULL, consumer, NULL);
    }

    for (int i = 0; i < n1; i++) {
        pthread_join(tid1[i], NULL);
    }

    for (int i = 0; i < n2; i++) {
        pthread_join(tid2[i], NULL);
    }

    exit(0);
}