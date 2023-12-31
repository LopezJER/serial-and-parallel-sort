/*
This program implements serial and parallel versions of bubble sort. Bubble sort involves bringing higher values to the end of the list through consecutive swaps. For the parallel implementation, the array is split in half and separate threads work on sorting each half through bubble sort. Then, a final bubble sort is performed on the entire array.

@author Jose Enrique R. Lopez
@date-create June 12, 2022

*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#define SERIAL 0
#define PARALLEL 1

int N_threads = 0;
//int SIZES [11] = {100, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};
int SIZES [10] = {10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000};
int ARRAY [100000000];
int GO_FLAGS [100000000];
int ARRAY_SIZE = 0;
int NO_SWAP = 1;

typedef struct indices{
    int upper;
    int lower;
} Indices;

void printArray(){
    for (int i = 0; i < ARRAY_SIZE; i++){
        printf("%d ", ARRAY[i]);
    }
    printf("\n");
}

void * hello(void *input) {
    printf("%s\n", (char *)input);
    pthread_exit(NULL);
}




void oddAndEven(){
    for (int i = 0; i < ARRAY_SIZE; i++){
        if (i%2 == 0 && i!=ARRAY_SIZE){
            printf("%d %d,", ARRAY[i], ARRAY[i+1]);
        }
    }
        printf("/n");
        for (int i = 0; i < ARRAY_SIZE; i++){
        if (i%2 != 0 && i!=ARRAY_SIZE){
            printf("%d %d", ARRAY[i], ARRAY[i+1]);
        }
    }
}

void checkSequence(){
    for (int i = 0; i < ARRAY_SIZE; i++){
        if (ARRAY[i]!=i+1){
            printf("Found discrepant element at ARRAY[%d] = %d.\n", i+1, ARRAY[i]);
        }
    }
    printf("No discrepant element.");
}

void bubbleSortPass(void *input){
    int* index_ptr = (int*) input;
    int index = *index_ptr;


    int temp = 0;

    if (index < ARRAY_SIZE-1 && ARRAY[index] > ARRAY[index+1]){
        NO_SWAP = 0;
        temp = ARRAY [index];
        ARRAY[index] = ARRAY[index+1];
        ARRAY[index+1] = temp;
    }


}

void serialBubbleSort(){
    int temp = 0;

    for (int i = 0; i < ARRAY_SIZE; i++){
        int no_swap = 1;
        for (int j = 0; j < ARRAY_SIZE; j++){
            if (j < ARRAY_SIZE-1 && ARRAY[j] > ARRAY[j+1]){
                no_swap = 0;
                temp = ARRAY [j];
                ARRAY[j] = ARRAY[j+1];
                ARRAY[j+1] = temp;
            }
        }
        if (no_swap) break;
    }

}

void sortSubarray(void * indices){
    Indices *my_indices = (Indices*)indices;

    int lower = (*my_indices).lower;
    int upper = (*my_indices).upper;

    int temp = 0;

    for (int i = lower; i < upper; i++){
        int no_swap = 1;
        for (int j = lower; j < upper; j++){
            if (j < upper-1 && ARRAY[j] > ARRAY[j+1]){

                no_swap = 0;
                temp = ARRAY [j];
                ARRAY[j] = ARRAY[j+1];
                ARRAY[j+1] = temp;
            }
        }
        if (no_swap) break;
    }

}

void parallelBubbleSort(){
    pthread_t tids[2];
    int offset = floor(ARRAY_SIZE/2);
    for (int i = 0 ; i < 2; i++){
        Indices *indices;
        indices = malloc(sizeof(Indices));
        (*indices).lower = i * offset;
        (*indices).upper = i * offset + offset;
        if (i==1) (*indices).upper = ARRAY_SIZE;
        pthread_t tid;
        pthread_create(&tid, NULL, (void *) sortSubarray, (void *) indices);
        tids[i] = tid;
    }

    for (int i = 0; i < 2; i++) pthread_join(tids[i], NULL);
    serialBubbleSort();


}

void initializeArray(int N){
    int upper = ceil(N/2);
    int lower = -1 * upper;
    int count = N;

    int i;
    for (i = 0; i < count; i++) {
        int num = (rand() %
           (upper - lower + 1)) + lower;
        ARRAY[i]=num;
    }
    ARRAY_SIZE = count;
}


int main(){
    srand(time(0));
    int t;
    int mode;
    printf("--------------BUBBLE SORT----------------\n");
    mode = SERIAL;
    printf("SERIAL \n");
    for (int i = 0; i < sizeof(SIZES)/sizeof(int); i++){
        initializeArray(SIZES[i]);
        t = clock();
        if (mode == SERIAL) serialBubbleSort();
        else if (mode == PARALLEL) parallelBubbleSort();
        t = clock() - t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
        printf("N = %d | %f seconds\n", SIZES[i], time_taken);
    }
    mode = PARALLEL;
    printf("PARALLEL \n");
        for (int i = 0; i < sizeof(SIZES)/sizeof(int); i++){
        t = clock();
        initializeArray(SIZES[i]);
        if (mode == SERIAL) serialBubbleSort();
        else if (mode == PARALLEL) parallelBubbleSort();
        t = clock() - t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
        printf("N = %d | %f seconds\n", SIZES[i], time_taken);
    }
}
