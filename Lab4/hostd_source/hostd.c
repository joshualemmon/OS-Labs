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
    //res.memory_array = { 0 }; giving error
    for(int i = 0; i < MEMORY; i++)
        res.memory_array[i] = 0;
    res.scanners = 1;
    res.printers = 2;
    res.cds = 2;
    res.modems = 1;
    //declaring queues
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
    //iterate throught dispatch list
    while(curr != NULL || rtq != NULL || pq1 != NULL || pq2 != NULL || pq3 != NULL)
    {
        //output current time step
        printf("Time step %d:\n", time);
        //iterate through dispatch list until all arrival times equal to 
        //current timestep are dealt with.
        if(curr != NULL)
        {
            while(curr->process.arrival_time == time)
            {
                proc p = pop(curr);
                //push process onto its respective priority queue
                if(p.priority == 0)
                {
                    printf("Process %d added to RT queue.\n",p.proc_num);
                    push(rtq,p);
                }
                else if(p.priority == 1)
                {
                    printf("Process %d added to P1 queue.\n",p.proc_num);
                    push(pq1,p);
                }
                else if(p.priority == 2)
                {
                    printf("Process %d added to P2 queue.\n",p.proc_num);
                    push(pq2,p);
                }
                else
                {
                    printf("Process %d added to P3 queue.\n",p.proc_num);
                    push(pq3,p);
                }    
                curr=curr->next;
            }
        }
        //prioritize the real time queue first
        //if the rtq isnt empty
        if(rtq != NULL)
        {
            node_t* n = rtq;
            //iterate through rtq
            while(n != NULL)
            {
                proc p = n->process;
                //if rtq process hasnt been allocated yet, allocate it
                if(p.memory_address == -1)
                {
                    p.memory_address = alloc_mem(res,p.mbytes);
                    if(p.memory_address != -1)
                        printf("Process %d allocated %d MB memory at address %d, %d printers, %d scanners, %d modems, %d CDs\n",p.proc_num,p.mbytes, p.memory_address,p.num_printers,p.num_scanners,p.num_modems,p.num_cds);
                }
                else
                {
                    //decrement runtime for process
                    p.proc_time--;
                    //if the run tim is complete, remove process from memory
                    if(p.proc_time == 0)
                    {
                        free_mem(res, p.memory_address,p.mbytes);
                        printf("Process %d finished, freed %d MB memory, %d printers, %d scanners, %d modems, %d CDs\n",p.proc_num,p.mbytes,p.num_printers,p.num_scanners,p.num_modems,p.num_cds);
                        node_t* temp = rtq;
                        if(temp == n)
                        {
                            free(n);
                        }
                        else
                        {
                            while(temp->next != n)
                                temp = temp->next;
                            temp->next = n->next;
                            n = temp;
                        }
                    }
                    else
                        printf("Process %d, %d time steps remaining\n",p.proc_num,p.proc_time);
                }
                n->process = p;
                n = n->next;
            }
        }
        //if the rtq is empty move onto pq1
        else
        {
            //if the priority 1 queue isn't empty
            if(pq1 != NULL)
            {
                node_t* n = pq1;
                //iterate through pq1
                while(n != NULL)
                {
                    proc p = n->process;
                    //allocate pq1 process is its unallocated
                    if(p.memory_address == -1)
                    {
                        p.memory_address = alloc_mem(res,p.mbytes);
                        if(p.memory_address != -1)
                            printf("Process %d allocated %d MB memory at address %d, %d printers, %d scanners, %d modems, %d CDs\n",p.proc_num,p.mbytes, p.memory_address,p.num_printers,p.num_scanners,p.num_modems,p.num_cds);
                    }
                    else
                    {   
                        //decrement runtime
                        p.proc_time--;
                        //if runtime is complete, free process from memory
                        if(p.proc_time == 0)
                        {
                            free_mem(res, p.memory_address,p.mbytes);
                            printf("Process %d finished, freed %d MB memory, %d printers, %d scanners, %d modems, %d CDs\n",p.proc_num,p.mbytes,p.num_printers,p.num_scanners,p.num_modems,p.num_cds);
                            node_t* temp = pq1;
                            if(temp == n)
                            {
                                free(n);
                            }
                            else
                            {
                                while(temp->next != n)
                                    temp = temp->next;
                                temp->next = n->next;
                                n = temp;
                            }
                        }
                        //put p1 process onto pq2
                        else
                        {
                            printf("Process %d, %d time steps remaining\n",p.proc_num,p.proc_time);
                            node_t* temp = pq1;
                            if(temp == n)
                            {
                                free(n);
                            }
                            else
                            {
                                while(temp->next != n)
                                    temp = temp->next;
                                temp->next = n->next;
                                n = temp;
                            }
                            push(pq2,p);
                        }
                }
                n->process = p;
                n = n->next;
                }
            }
            //if pq2 is not empty
            if(pq2 != NULL)
            {
                node_t* n = pq2;
                //iterate through pq2
                while(n != NULL)
                    {
                        proc p = n->process;
                        //allocate pq2 process if unallocated
                        if(p.memory_address == -1)
                        {
                            p.memory_address = alloc_mem(res,p.mbytes);
                            if(p.memory_address != -1)
                                printf("Process %d allocated %d MB memory at address %d, %d printers, %d scanners, %d modems, %d CDs\n",p.proc_num,p.mbytes, p.memory_address,p.num_printers,p.num_scanners,p.num_modems,p.num_cds);
                        }
                        else
                        {
                            //decrement runtime
                            p.proc_time--;
                            //if runtime is finished, free memory
                            if(p.proc_time == 0)
                            {
                                free_mem(res, p.memory_address,p.mbytes);
                                printf("Process %d finished, freed %d MB memory, %d printers, %d scanners, %d modems, %d CDs\n",p.proc_num,p.mbytes,p.num_printers,p.num_scanners,p.num_modems,p.num_cds);
                                node_t* temp = pq2;
                                if(temp == n)
                                {
                                    free(n);
                                }
                                else
                                {
                                    while(temp->next != n)
                                        temp = temp->next;
                                    temp->next = n->next;
                                    n = temp;
                                }
                            }
                            //put pq2 process onto pq3
                            else
                            {
                                printf("Process %d, %d time steps remaining\n",p.proc_num,p.proc_time);
                                node_t* temp = pq2;
                                if(temp == n)
                                {
                                    free(n);
                                }
                                else
                                {
                                    while(temp->next != n)
                                        temp = temp->next;
                                    temp->next = n->next;
                                    n = temp;
                                }
                                push(pq3,p);
                            }
                        }
                        n->process = p;
                        n = n->next;
                    }
                }
                //if pq3 is not empty
                if(pq3 != NULL)
                {
                    node_t* n = pq3;
                    //iterate through pq3
                    while(n != NULL)
                    {
                        proc p = n->process;
                        //if pq3 process is unallocated, allocate it
                        if(p.memory_address == -1)
                        {
                            p.memory_address = alloc_mem(res,p.mbytes);
                            if(p.memory_address != -1)
                                printf("Process %d allocated %d MB memory at address %d, %d printers, %d scanners, %d modems, %d CDs\n",p.proc_num,p.mbytes, p.memory_address,p.num_printers,p.num_scanners,p.num_modems,p.num_cds);
                        }
                        else
                        {
                            //decrement runtime
                            p.proc_time--;
                            //if runtime is complete, free memory
                            if(p.proc_time == 0)
                            {
                                free_mem(res, p.memory_address,p.mbytes);
                                printf("Process %d finished, freed %d MB memory, %d printers, %d scanners, %d modems, %d CDs\n",p.proc_num,p.mbytes,p.num_printers,p.num_scanners,p.num_modems,p.num_cds);
                                node_t* temp = pq3;
                                if(temp == n)
                                {
                                    free(n);
                                }
                                else
                                {
                                    while(temp->next != n)
                                        temp = temp->next;
                                    temp->next = n->next;
                                    n = temp;
                                }
                            }
                            //put pq3 process at end of pq3
                            else
                            {
                                printf("Process %d, %d time steps remaining\n",p.proc_num,p.proc_time);
                                node_t* temp = pq3;
                                if(temp == n)
                                {
                                    free(n);
                                }
                                else
                                {
                                    while(temp->next != n)
                                        temp = temp->next;
                                    temp->next = n->next;
                                    n = temp;
                                }
                                push(pq3,p);
                            }
                    }
                    n->process = p;
                    n = n->next;
                }
            }      
        }
    time++;
}

    // Allocate the resources for each process before it's executed

    // Execute the process binary using fork and exec

    // Perform the appropriate signal handling / resource allocation and de-alloaction

    // Repeat until all processes have been executed, all queues are empty
     
    return EXIT_SUCCESS;
}
