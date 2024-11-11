#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "shared.h"

void producer() {
    int item;
    for (int i = 0; i < 10; i++) { // Produce 10 items for this example
        item = rand() % 100; // Produce an item

        // Check if the buffer is full
        if (sem_trywait(empty) != 0) {
            printf("Producer waiting, buffer is full...\n");
            sem_wait(empty); // Wait for an empty slot
        }

        pthread_mutex_lock(mutex); // Lock the buffer

        printf("Producer arrived, buffer before: ");
        print_buffer("before"); // Print buffer before adding item

        // Add item to the buffer
        buffer[*count] = item;
        printf("Producer produced: %d\n", item);
        (*count)++;

        printf("Buffer after: ");
        print_buffer("after"); // Print buffer after adding item

        pthread_mutex_unlock(mutex); // Unlock the buffer
        sem_post(full); // Signal that there’s a new full slot

        usleep(200000); // Simulate time taken to produce
    }
}

int main() {
    setup_shared_memory();

    producer();

    cleanup_shared_memory();
    return 0;
}
