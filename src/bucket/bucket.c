/*
This program implements serial and parallel versions of bucket sort. Bucket sort involves hashing all values of the array into separate buckets where each bucket i corresponds to the ith percentile of the range of values. Each bucket is then sorted to arrive at the final sorted list. For the parallel implementation, the set of buckets is split among two threads, which each sort the set of buckets assigned to them.

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

typedef struct node {
    int x;
    struct node * next;
    struct node * prev;
} Node;

typedef struct indices{
    int upper;
    int lower;
} Indices;

int N_threads = 0;
int SIZES [3] = {9, 10, 11};
int ARRAY [100000000];
Node * BUCKET_ARRAY[100000000];
int GO_FLAGS [100000000];
int ARRAY_SIZE = 0;
int NO_SWAP = 1;
int BUCKETS = 0;
int MIN = 0;
int MAX = 0;
int RANGE = 0;


void findRange(){
    MIN = MAX = ARRAY[0];

    for (int i = 1; i < ARRAY_SIZE; i++){
        if (ARRAY[i] < MIN) MIN = ARRAY[i];
        if (ARRAY[i] > MAX) MAX = ARRAY[i];
    }

    RANGE = MAX-MIN;

}


void printArray(){
    for (int i = 0; i < ARRAY_SIZE; i++){
        printf("%d ", ARRAY[i]);
    }
    printf("\n");
}

void createBuckets(){

    for (int i = 0; i < BUCKETS+1; i++) BUCKET_ARRAY[i] = NULL;

    for (int i = 0; i < BUCKETS; i++){
        Node * new_node = NULL;
       BUCKET_ARRAY[i] = new_node;
    }
}

int hash(int ele){
    int offset = floor(RANGE/BUCKETS);
    int x = floor(ele/offset);
    if (x > BUCKETS - 1) return BUCKETS-1;
    if (x < 0) return 0;
    return x;

}


void bucketize(){
    int key;
    for (int i = 0; i < ARRAY_SIZE; i++){
        key = hash(ARRAY[i]);
        if (BUCKET_ARRAY[key] == NULL){
            Node * new_node = (Node *) malloc (sizeof(Node));
            new_node -> x = ARRAY[i];
            new_node -> next = NULL;
            new_node -> prev = NULL;
            BUCKET_ARRAY[key] = new_node;
        } else{
            Node * temp = BUCKET_ARRAY[key];
            while (temp->next!=NULL){
                temp = temp->next;
            }
            Node * new_node = (Node *) malloc (sizeof(Node));
            new_node -> x = ARRAY[i];
            new_node -> next = NULL;
            new_node -> prev = temp;
            temp->next = new_node;


        }


    }


}

void debucketize(){
    int count = 0;
    for (int i = 0; i < BUCKETS; i++){
        Node * temp = BUCKET_ARRAY[i];
        while (temp!=NULL){
            ARRAY[count] = temp->x;
            temp=temp->next;
            count++;
        }
    }
}


void sortBucket(void * input){
    int* index_ptr = (int*) input;
    int i = *index_ptr;
    int no_swap;
    while(1){
        Node * temp = BUCKET_ARRAY[i];
        no_swap = 1;
        while(temp!=NULL && temp->next!=NULL){
            if(temp-> x > temp->next->x){
                no_swap = 0;
                int num = temp->x;
                temp->x = temp->next->x;
                temp->next->x = num;

            }
            temp=temp->next;
        }
        if (no_swap) break;

    }
}

void sortBucketsAssigned(void * indices){
    Indices *my_indices = (Indices*)indices;
    int lower = (*my_indices).lower;
    int upper = (*my_indices).upper;

    int no_swap;
    for (int i = lower; i < upper; i++){
        while(1){
            Node * temp = BUCKET_ARRAY[i];
            no_swap = 1;
            while(temp!=NULL && temp->next!=NULL){
                if(temp-> x > temp->next->x){
                    no_swap = 0;
                    int num = temp->x;
                    temp->x = temp->next->x;
                    temp->next->x = num;

                }
                temp=temp->next;
            }
            if (no_swap) break;

        }
    }
    free (indices);
}


void parallelSortBuckets(){
    pthread_t tids[2];
    int offset = floor(BUCKETS/2);
    for (int i = 0 ; i < 2; i++){
        Indices *indices;
        indices = malloc(sizeof(Indices));
        (*indices).lower = i * offset;
        (*indices).upper = i * offset + offset;
        pthread_t tid;
        pthread_create(&tid, NULL, (void *) sortBucketsAssigned, (void *) indices);
        tids[i] = tid;
    }

    for (int i = 0; i < 2; i++) pthread_join(tids[i], NULL);

}

void serialSortBuckets(){
    for (int i = 0 ; i < BUCKETS; i++){
        int no_swap;
        while(1){
            Node * temp = BUCKET_ARRAY[i];
            no_swap = 1;
            while(temp!=NULL && temp->next!=NULL){
                if(temp-> x > temp->next->x){
                    no_swap = 0;
                    int num = temp->x;
                    temp->x = temp->next->x;
                    temp->next->x = num;
                }
                temp=temp->next;
            }
            if (no_swap){
                break;
            }
        }
    }
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

void serialBucketSort(){
    BUCKETS = floor(sqrt(ARRAY_SIZE));
    initializeArray(ARRAY_SIZE);
    findRange();
    createBuckets();
    bucketize();
    int t=clock();
    serialSortBuckets();
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
    printf("N = %d | %f seconds\n", ARRAY_SIZE, time_taken);
    debucketize();
}

void parallelBucketSort(){
    BUCKETS = floor(sqrt(ARRAY_SIZE));
    initializeArray(ARRAY_SIZE);
    findRange();
    createBuckets();
    bucketize();
    int t=clock();
    parallelSortBuckets();
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
    printf("N = %d | %f seconds\n", ARRAY_SIZE, time_taken);
    debucketize();
}
int main(void){
    srand(time(0));
    int t;
    int mode;
    printf("-------------------BUCKET SORT----------------------\n");
    printf("SERIAL\n");
    for (int i = 0; i < sizeof(SIZES)/sizeof(int); i++){
        ARRAY_SIZE = SIZES[i];
        serialBucketSort();
    }

    printf("PARALLEL\n");
    for (int i = 0; i < sizeof(SIZES)/sizeof(int); i++){
        ARRAY_SIZE = SIZES[i];
        parallelBucketSort();
    }
}


