#include "shared.h"
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

shared_data* initialize_shared_memory(int *shmid) {

    *shmid = shmget(SHM_KEY, sizeof(shared_data), IPC_CREAT | 0666);
    if (*shmid < 0) {
        perror("shmget");
        exit(1);
    }

   
    shared_data* data = (shared_data*) shmat(*shmid, NULL, 0);
    if (data == (void*) -1) {
        perror("shmat");
        exit(1);
    }

   
    data->in = 0;
    data->out = 0;
    memset(data->buffer, 0, BUFFER_SIZE * sizeof(int));

    return data;
}

void initialize_semaphores(int semid) {
    // Initialize semaphores: empty = BUFFER_SIZE, full = 0, mutex = 1
    semctl(semid, SEM_EMPTY, SETVAL, BUFFER_SIZE);
    semctl(semid, SEM_FULL, SETVAL, 0);
    semctl(semid, SEM_MUTEX, SETVAL, 1);
}

void cleanup_shared_memory(int shmid, shared_data* data) {
    shmdt(data);  
    shmctl(shmid, IPC_RMID, NULL);  
}

void cleanup_semaphores(int semid) {
    semctl(semid, 0, IPC_RMID);
}

void sem_wait(int semid, int sem_num) {
    struct sembuf operation = {sem_num, -1, 0};
    semop(semid, &operation, 1); 
}

void sem_signal(int semid, int sem_num) {
    struct sembuf operation = {sem_num, 1, 0};
    semop(semid, &operation, 1);  
}
