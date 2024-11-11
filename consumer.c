#include "shared.h"
#include <stdio.h>
#include <unistd.h>

int count_items_in_buffer(shared_data* data) {
    int count = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (data->buffer[i] != 0) {  // Assuming 0 means the slot is empty
            count++;
        }
    }
    return count;
}

int main() {
    int shmid;
    shared_data* data = initialize_shared_memory(&shmid);  // Initialize shared memory

    // Initialize semaphores
    int semid = semget(SEM_KEY, 3, IPC_CREAT | 0666);
    
    while (1) {
        sem_wait(semid, SEM_FULL);  
        sem_wait(semid, SEM_MUTEX);  

        
        int item = data->buffer[data->out];
         printf("Consumer consume item %d from position %d\n", item+1, data->out);
        data->buffer[data->out] = 0;  
        data->out = (data->out + 1) % BUFFER_SIZE;  

        
        int num_items = count_items_in_buffer(data);
        printf("Number of items in the buffer after consuming: %d\n", num_items);

        sem_signal(semid, SEM_MUTEX);  
        sem_signal(semid, SEM_EMPTY);  

        sleep(2);  
    }

    cleanup_shared_memory(shmid, data);  // Cleanup shared memory
    return 0;
}
