// shared.h

#ifndef SHARED_H
#define SHARED_H

#include <semaphore.h>
#include <pthread.h>

#define N 4 // Buffer size

// Shared memory object names
#define BUFFER_NAME "/buffer"
#define COUNT_NAME "/count"
#define EMPTY_SEM "/empty_sem"
#define FULL_SEM "/full_sem"
#define MUTEX_NAME "/mutex"

extern int *buffer;
extern int *count;
extern sem_t *empty, *full;
extern pthread_mutex_t *mutex;

void setup_shared_memory();
void cleanup_shared_memory();

// Add the declaration for print_buffer
void print_buffer(const char *label);

#endif
