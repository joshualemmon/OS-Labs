/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "queue.h"

// Define your FIFO queue functions here, these will most likely be the
// push and pop functions that you declared in your header file

//add a node to the end of the queue
void *push(node_t *head, proc process)
{
	node_t* curr = head;
	if(head->process.mbytes == NULL)
	{
		head->process = process;
		head->next = NULL;
	}
	else
	{
		while(curr->next != NULL)
			curr = curr->next;
		curr->next = malloc(sizeof(node_t));
		curr->next->process = process;
		curr->next->next = NULL;
	}
	//printf("proc num %d\n", process.proc_num);
}
//remove the first node in the queue
proc pop(node_t *head)
{
	proc p;
	if(head->next != NULL)
	{
		p = head->process;
		head = head->next;
	}
	else
	{
		p = head->process;
		free(head);
	}

	return p;
}
