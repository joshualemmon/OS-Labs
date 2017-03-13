/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

// Define your FIFO queue functions here, these will most likely be the
// push and pop functions that you declared in your header file

node_t *push(node_t *head, proc process)
{
	node_t* newNode = NULL;
	if (head->next == NULL)
	{
		newNode->process = process;
		newNode->next = NULL;
		head->next = newNode;
	}
	else
	{
		node_t* curr;
		curr = head;
		while(curr->next != NULL)
			curr=curr->next;
		newNode->process = process;
		newNode->next = NULL;
		curr->next = newNode;
	}
	return head;
}

node_t *pop(node_t *head)
{
	node_t* retNode = NULL;
	if(head->next != NULL)
	{
		retNode = head;
		head = head->next;
	}
	else
	{
		retNode = head;
		head = NULL;
	}
	return retNode;
}
