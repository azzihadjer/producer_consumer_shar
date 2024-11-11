#ifndef SHARED_H
#define SHARED_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 4
#define SHM_KEY 0x1234  // Shared memory key
#define SEM_KEY 0x5678  // Semaphore key


#define SEM_EMPTY 0
#define SEM_FULL 1
#define SEM_MUTEX 2

typedef struct {
    int buffer[BUFFER_SIZE];   // Shared buffer
    int in;                    
    int out;                  
} shared_data;

shared_data* initialize_shared_memory(int *shmid);
void initialize_semaphores(int semid);
void cleanup_shared_memory(int shmid, shared_data* data);
void cleanup_semaphores(int semid);


void sem_wait(int semid, int sem_num);
void sem_signal(int semid, int sem_num);

#endif
