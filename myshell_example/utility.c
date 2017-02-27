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
#include <termios.h>

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

void pwd(char* PWD)
{
	printf("%s\n",PWD);
}

void pauseShell()
{
	printf("PAUSED\n");
	char c = getch();
	while(c != 10)
	{
		c = getch();
	}
	printf("UNPAUSED\n");
}

int cd(char* dir)
{
	return chdir(dir);
}

void clr(void)
{
	//escape code to replace characters on screen with spaces and to set
	//cursor at top left position
	printf("\e[1;1H\e[2J");
}

void ls(char* PWD)
{
	DIR* currdir = opendir((const char*)PWD);
	struct dirent* dirptr = NULL;
	unsigned int i;
	if(currdir != NULL)
	{
		for (i = 0; NULL !=(dirptr = readdir(currdir)); i++)
		{
			printf("%s  ", dirptr->d_name);
		}
		printf("\n%d files displayed.\n",i);
	} 
	else
		printf("Error opening directory\n");
}

void dir(char* dir)
{
	printf("%s\n", dir);
	DIR* currdir = opendir((const char*)dir);
	struct dirent* dirptr = NULL;
	unsigned int i;
	if(currdir != NULL)
	{
		for (i = 0; NULL !=(dirptr = readdir(currdir)); i++)
		{
			printf("%s  ", dirptr->d_name);
		}
		printf("\n%d files displayed.\n",i);
	} 
	else
		printf("Error opening directory\n");
}

void penviron(char* PWD)
{
	printf("$HOME: %s\n", getenv("HOME"));
	printf("$PATH: %s\n", PWD);
}

int getch()
{
	struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}