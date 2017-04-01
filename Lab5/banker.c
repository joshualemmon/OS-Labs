/*
 * Banker's Algorithm for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include "banker.h"
#include "time.h"

// Put any other macros or constants here using #define
// May be any values >= 0
#define NUM_CUSTOMERS 5
#define NUM_RESOURCES 3


// Put global environment variables here
// Available amount of each resource
int available[NUM_RESOURCES];

// Maximum demand of each customer
int maximum[NUM_CUSTOMERS][NUM_RESOURCES];

// Amount currently allocated to each customer
int allocation[NUM_CUSTOMERS][NUM_RESOURCES];

// Remaining need of each customer
int need[NUM_CUSTOMERS][NUM_RESOURCES];

sem_t sem;

bool is_safe()
{
    int work[NUM_RESOURCES];
    for(int i = 0; i < NUM_RESOURCES; i++)
        work[i] = available[i];
    int finish[NUM_CUSTOMERS]= { 0 };
    for(int i = 0; i < NUM_CUSTOMERS; i++)
    {
        int need_less = 0;
        for(int j = 0; j < NUM_CUSTOMERS; j++)
        {
            if(need[i][j] <= work[j])
            {
                need_less = 1;
            }
            else
            {
                need_less = 0;
                break;
            }
        }
        if(!finish[i] && need_less)
        {
            for(int j = 0; j < NUM_CUSTOMERS; j++)
            {

                work[j]+=allocation[i][j];
            }
            finish[i] = 1;
        }
    }
    int safe = 0;
    for(int i = 0; i < NUM_CUSTOMERS; i++)
    {
        if(finish[i] == 0)
            break;
        else
            safe = 1; 
    }
    return safe;
}

// Define functions declared in banker.h here
bool request_res(int n_customer, int request[])
{
    bool req = 0;
    int request_less = 0;
    sem_wait(&sem);
    for(int i = 0; i < NUM_CUSTOMERS; i++)
    {
        if(request[i] <= need[n_customer][i])
        {
            request_less = 1;
        }
        else
        {
            request_less = 0;
            break;
        }
    }
    sem_post(&sem);
    if(!request_less)
        return 0;
    while(1)
    {
        sem_wait(&sem);
        for(int i = 0; i < NUM_CUSTOMERS; i++)
        {
            if(request[i] <= available[i])
            {
                request_less = 1;
            }
            else
            {
                request_less = 0;
                break;
            }
        }
        if(!request_less)
        {
            sem_post(&sem);
            sem_wait(&sem);
        }

        for(int i = 0; i < NUM_RESOURCES; i++)
        {
            available[i] -= request[i];
            allocation[n_customer][i] += request[i];
            need[n_customer][i] -= request[i];
        }
        if(is_safe())
            return 1;
        else
            release_res(n_customer,request);
        sem_post(&sem);
    }

    
    return req;
}

// Release resources, returns true if successful
bool release_res(int n_customer, int release[])
{
    bool rel = 0;
    for(int i = 0; i < NUM_RESOURCES; i++)
    {
        available[i] += release[i];
        allocation[n_customer][i] -= release[i];
        need[n_customer][i] += release[i];
    }
    return rel;
}

void* thread_start(void *param)
{
    int n = (int)param;
    int request[NUM_RESOURCES];

    while(1)
    {
        for(int i = 0; i < NUM_RESOURCES; i++)
            request[i] = rand()%maximum[n][i]+1;
        int req = request_res(n, request);
        if(req)
        {
            printf("Customer %d\t Allocated\t ", n);
            for(int i = 0; i < NUM_RESOURCES; i++)
                printf("%d ",request[i]);
            printf("\n");
        }
        else
        {
            printf("Customer %d\t Denied\t ", n);
            for(int i = 0; i < NUM_RESOURCES; i++)
                printf("%d ",request[i]);
            printf("\n");
        }

    }
}


int main(int argc, char *argv[])
{
    // ==================== YOUR CODE HERE ==================== //
    srand(time(NULL));
    // Read in arguments from CLI, NUM_RESOURCES is the number of arguments
    for(int i = 0; i < argc; i++)
        available[i] = (int)argv[i];   
    
    for(int i = 0; i < NUM_CUSTOMERS; i++)
        for(int j = 0; j < NUM_RESOURCES; j++)
            maximum[i][j] = j+2;
    // Allocate the available resources

    // Initialize the pthreads, locks, mutexes, etc.
    sem_init(&sem, 0, 1);
    pthread_t threads[NUM_CUSTOMERS];
    for(int i = 0; i < NUM_CUSTOMERS; i++)
        pthread_create(&threads[i],NULL,(void*)thread_start,(void*)(i+1));

    // Run the threads and continually loop
    //for(int i = 0; i < NUM_CUSTOMERS; i++)
    //    pthread_join(threads[i], NULL);
    // The threads will request and then release random numbers of resources

    // If your program hangs you may have a deadlock, otherwise you *may* have
    // implemented the banker's algorithm correctly
    
    // If you are having issues try and limit the number of threads (NUM_CUSTOMERS)
    // to just 2 and focus on getting the multithreading working for just two threads
    printf("\n");
    return EXIT_SUCCESS;
}
