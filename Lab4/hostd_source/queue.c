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

//add a node to the end of the queue
node_t *push(node_t *head, proc process)
{
	printf("pushing node\n");
	node_t* newNode = NULL;
	if(head == NULL)
	{
		head = malloc(sizeof(node_t));
		head->process = process;
		printf("adding first node\n");
		head->next = NULL;
	}
	//if the head is the only node in the queue,
	//simply add a new node for head to point to.
	else if (head->next == NULL)
	{
		printf("adding second node\n");
		newNode = malloc(sizeof(node_t));
		newNode->process = process;
		newNode->next = NULL;
		head->next = newNode;
	}
	//if head isn't the only node in the queue,
	//traverse list until you get to the tail
	//and point the tail to the new node
	else
	{
		newNode = malloc(sizeof(node_t));
		printf("adding other node\n");
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
//remove the first node in the queue
node_t *pop(node_t *head)
{
	node_t* retNode = NULL;
	//if head isn't the only node,
	//set head->next as the new head
	//and return the old head value
	if(head->next != NULL)
	{
		retNode = head;
		head = head->next;
	}
	//if head is the only node,
	//return the head value and set
	//head to null, making the queue
	//empty
	else
	{
		retNode = head;
		head = NULL;
	}
	return retNode;
}
