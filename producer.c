#include "shared.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

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
    shared_data* data = initialize_shared_memory(&shmid);  

    // Initialize semaphores
    int semid = semget(SEM_KEY, 3, IPC_CREAT | 0666);
    initialize_semaphores(semid);

    srand(time(NULL));  

    while (1) {
        int item = rand() % 100;  

        sem_wait(semid, SEM_EMPTY);  
        sem_wait(semid, SEM_MUTEX); 

        data->buffer[data->in] = item;
         printf("Producer produce item %d from position %d\n", item, data->in);
        data->in = (data->in + 1) % BUFFER_SIZE;  

        
        int num_items = count_items_in_buffer(data);
        printf("Number of items in the buffer after producing: %d\n", num_items);

        sem_signal(semid, SEM_MUTEX); 
        sem_signal(semid, SEM_FULL);   

        sleep(1);  
    }

    cleanup_shared_memory(shmid, data);  
    return 0;
}
