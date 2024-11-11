#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "shared.h"

void consumer() {
    int item;
    for (int i = 0; i < 10; i++) { // Consume 10 items for this example
        // Check if the buffer is empty
        if (sem_trywait(full) != 0) {
            printf("Consumer waiting, buffer is empty...\n");
            sem_wait(full); // Wait for a full slot
        }

        pthread_mutex_lock(mutex); // Lock the buffer

        printf("Consumer arrived, buffer before: ");
        print_buffer("before"); // Print buffer before removing item

        // Remove item from the buffer
        item = buffer[0];
        printf("Consumer consumed: %d\n", item);

        // Shift all items in the buffer to the left
        for (int j = 0; j < *count - 1; j++) {
            buffer[j] = buffer[j + 1];
        }
        buffer[*count - 1] = -1; // Mark the last position as empty
        (*count)--;

        printf("Buffer after: ");
        print_buffer("after"); // Print buffer after removing item

        pthread_mutex_unlock(mutex); // Unlock the buffer
        sem_post(empty); // Signal that there’s an empty slot

        sleep(1); // Simulate time taken to consume
    }
}

int main() {
    setup_shared_memory();

    consumer();

    cleanup_shared_memory();
    return 0;
}
