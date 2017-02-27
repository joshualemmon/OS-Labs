/*
 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2017, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include "utility.h"

// Define your utility functions here, these will most likely be functions that you call
// in your myshell.c source file

void echo(char* arg)
{
	printf("%s\n",arg);
}

void help(void)
{
	char line[256];
        	FILE* file = fopen("README.txt", "r");
        	if (file)
        	{
        		while((fgets(line, sizeof(line), file)))
        			printf("%s",line);
        		printf("\n");
        		fclose(file);
        	}
}

void pwd(char* wd)
{
	printf("%s\n",wd);
}

void pauseShell()
{
	printf("PAUSED\n");
	char c = getchar();
	while(c != 10)
	{
		c = getchar();
	}
	printf("UNPAUSED\n");
}

int cd(char* dir)
{
	return chdir(dir);
}