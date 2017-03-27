/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "utility.h"

// Define your utility functions here, you will likely need to add more...

int alloc_mem(resources res, int size)
{
	int index = -1;
	//begin iterating through memory
	for(int i = 0; i < MEMORY; i++)
	{
		//if an unused memory index is found, sum all consecutive 
		//unused indexes
		if(res.memory_array[i] == 0)
		{
			int j = i;
			int block_size = 0;
			while(res.memory_array[j++] == 0)
				block_size++;
			//check if block of memory is big enough for process
			//if it is allocate the process to that memory
			if (block_size >= size)
			{
				for(int k = i; k < i+size; k++)
					res.memory_array[k] = 1;
				index = i;
				return index;
			}
			else
				i = j;
		}
	}
	return index;
}

void free_mem(resources res, int index, int size)
{
	//free the memory after process is finished
	for(int i = index; i < index + size;i++)
		res.memory_array[i] = 0;
}

void load_dispatch(char *dispatch_file, node_t *queue)
{
	FILE *f = fopen(dispatch_file, "r");
	proc process;
	//load process from file into queue
	while(fscanf(f,"%d, %d, %d, %d, %d, %d, %d, %d\n", &process.arrival_time, &process.priority, &process.proc_time, &process.mbytes, 
													   &process.num_printers, &process.num_cds, &process.num_scanners, &process.num_modems) > 0)
	{
		process.memory_address = -1;
		push(queue, process);
	}
}
