#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#define SIZE 4096

int main() {
    int shm_id;
    int *shm_ptr;
    int data;
    shm_id = shmget(IPC_PRIVATE, SIZE, IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("ERROR creating shared memory");
        exit(1);
    } else {
        printf("Shared memory created with ID: %d\n", shm_id);	}
    shm_ptr = (int*) shmat(shm_id, NULL, 0);
    if (shm_ptr == (int*)(-1)) {
        perror("ERROR attaching shared memory");
        exit(1);    }
    printf("Shared memory attached at address: %p\n", shm_ptr);
    printf("Enter an integer value to store in shared memory: ");
    scanf("%d", &data);
    *shm_ptr = data;
    int pid = fork();
    if (pid < 0) {
        perror("ERROR forking");
        exit(1);
    } else if (pid == 0) {
        sleep(1);
        printf("Child: Accessing shared memory...\n");
        printf("Child read data from shared memory: %d\n", *shm_ptr);
        if (shmdt((void *)shm_ptr) == -1) {
            perror("ERROR detaching shared memory in child");
        }    exit(0);
    } else {
        printf("Parent: Data written to shared memory: %d\n", *shm_ptr); }
 if (shmdt((void *)shm_ptr) == 0) {
        printf("Parent: Successfully detached from shared memory.\n");
    } else {
        perror("ERROR detaching shared memory in parent");   }
    wait(NULL);
    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        perror("ERROR removing shared memory segment");
    } else {
        printf("Parent: Shared memory segment removed.\n");
    }
    return 0;
}
