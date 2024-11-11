#include "shared.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> // Add this line for ftruncate and close

int *buffer;
int *count;
sem_t *empty, *full;
pthread_mutex_t *mutex;

void setup_shared_memory() {
    int fd;

    fd = shm_open(BUFFER_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, N * sizeof(int)); // Ensures buffer has enough space
    buffer = mmap(NULL, N * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    fd = shm_open(COUNT_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, sizeof(int)); // Ensures count has enough space
    count = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    empty = sem_open(EMPTY_SEM, O_CREAT, 0666, N);
    full = sem_open(FULL_SEM, O_CREAT, 0666, 0);

    fd = shm_open(MUTEX_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, sizeof(pthread_mutex_t)); // Ensures mutex has enough space
    mutex = mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(mutex, &mutex_attr);

    *count = 0;
    for (int i = 0; i < N; i++) {
        buffer[i] = -1;
    }
}

void print_buffer(const char *label) {
    time_t now;
    time(&now);
    char *time_str = ctime(&now);
    time_str[strcspn(time_str, "\n")] = '\0';
    printf("[%s] %s [ ", time_str, label);
    for (int i = 0; i < N; i++) {
        if (buffer[i] == -1) {
            printf("_ ");
        } else {
            printf("%d ", buffer[i]);
        }
    }
    printf("]\n");
    // Only print an additional newline if label is "after"
    if (strcmp(label, "after") == 0) {
        printf("\n");
    }
}

void cleanup_shared_memory() {
    sem_close(empty);
    sem_close(full);
    sem_unlink(EMPTY_SEM);
    sem_unlink(FULL_SEM);

    pthread_mutex_destroy(mutex);

    shm_unlink(BUFFER_NAME);
    shm_unlink(COUNT_NAME);
    shm_unlink(MUTEX_NAME);
}
