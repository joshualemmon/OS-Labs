/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "queue.h"
#include "utility.h"
#include "hostd.h"

// Put macros or constants here using #define
#define MEMORY 1024

// Put global environment variables here

// Define functions declared in hostd.h here

int main(int argc, char *argv[])
{
    // ==================== YOUR CODE HERE ==================== //
    // Load the dispatchlist
    resources res;
    res.memory_ array = { 0 };
    res.scanners = 1;
    res.printers = 2;
    res.cds = 2;
    res.modems = 1;
    node_t *dispatch_queue = malloc(sizeof(node_t));
    node_t *rtq = malloc(sizeof(node_t));
    node_t *pq1 = malloc(sizeof(node_t));
    node_t *pq2 = malloc(sizeof(node_t));
    node_t *pq3 = malloc(sizeof(node_t));
    load_dispatch(argv[1], dispatch_queue);

    // Add each process structure instance to the job dispatch list queue

    // Iterate through each item in the job dispatch list, add each process
    // to the appropriate queues
    node_t* curr = dispatch_queue;
    int time = 0;
    int proc_num = 1;
    while(curr != NULL)
    {
        printf("Time step %d:\n", time);
        while(curr->process.arrival_time == time && curr != NULL)
        {
            proc p = pop(curr);
            if(p.priority == 0)
            {
                printf("Process %d added to RT queue.\n",proc_num++);
                push(rtq,p);
            }
            else if(p.priority == 1)
            {
                printf("Process %d added to P1 queue.\n",proc_num++);
                push(pq1,p);
            }
            else if(p.priority == 2)
            {
                printf("Process %d added to P2 queue.\n",proc_num++);
                push(pq2,p);
            }
            else
            {
                printf("Process %d added to P3 queue.\n",proc_num++);
                push(pq3,p);
            }
            curr=curr->next;
        }
        if(rtq != NULL)
        {
            node_t* n = rtq;
            proc p = n->process;

            printf("Process 1 allocated %d MB memory at address %d, %d printers, %d scanners, %d modems, %d CDs\n",p.mbytes, p.memory_address,p.num_printers,p.num_scanners,p.num_modems,p.num_cds);
        }
        time++;
    }

    // Allocate the resources for each process before it's executed

    // Execute the process binary using fork and exec

    // Perform the appropriate signal handling / resource allocation and de-alloaction

    // Repeat until all processes have been executed, all queues are empty
     
    return EXIT_SUCCESS;
}
