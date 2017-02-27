/*
 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2017, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include "utility.h"
#include "myshell.h"

// Put macros or constants here using #define
#define BUFFER_LEN 256
#define SEPARATORS " \t\n"

// Put global environment variables here
char OLDPWD[BUFFER_LEN];
char PWD[BUFFER_LEN];
// Define functions declared in myshell.h here

int main(int argc, char *argv[])
{
    // Input buffer and and commands
    char buffer[BUFFER_LEN] = { 0 };
    char command[BUFFER_LEN] = { 0 };
    char arg[BUFFER_LEN] = { 0 };
    getcwd(PWD,sizeof(PWD));
	strcpy(OLDPWD,PWD);

    // Parse the commands provided using argc and argv

    // Perform an infinite loop getting command input from users
    while (fgets(buffer, BUFFER_LEN, stdin) != NULL)
    {
    	if(buffer[strlen(buffer)-1] == '\n')
    		buffer[strlen(buffer)-1] = '\0';
    	strcpy(arg,"");
        // Perform string tokenization to get the command and argument
    	char* token = strtok(buffer, SEPARATORS);
    	strcpy(command, token);
    	token = strtok(NULL, SEPARATORS);
    	while(token != NULL)
    	{
    		strcat(arg, token);
    		strcat(arg, " ");
    		token = strtok(NULL, SEPARATORS);
    	}
    	arg[strlen(arg)-1] = '\0'; //removing extra character at end of arg
    	printf("Command: %s arg: %s\n", command, arg);

        // Check the command and execute the operations for each command
        // cd command -- change the current directory
        if (strcmp(command, "cd") == 0)
        {
            // your code here
            if(strcmp(arg,"") == 0 || strcmp(arg,"~") == 0)
            {
            	strcpy(OLDPWD,PWD);
            	strcpy(PWD,getenv("HOME"));
            	cd(PWD);
            }
            else if(strcmp(arg,"..") == 0)
            {
            	while(PWD[strlen(PWD)-1] != '/')
            	{
            		PWD[strlen(PWD)-1] = '\0';
            	}
            	PWD[strlen(PWD)-1] = '\0';
            	cd(PWD);

            }
            else if(strcmp(arg,"-") == 0)
            {
            	strcpy(PWD,OLDPWD);
            	cd(PWD);
            }
            else
            {
            	char temp[sizeof(PWD)+sizeof(arg)];
            	strcpy(temp,PWD);
            	strcat(temp,"/");
            	strcat(temp,arg);
            	printf("%s\n",temp);
				if(cd(temp))
            	{
            		strcpy(OLDPWD,PWD);
            		strcpy(PWD,temp);
            	}
            	else
            		printf("Error, no such directory found");
        	}
        }

        // other commands here...
        
        // quit command -- exit the shell
        else if (strcmp(command, "quit") == 0)
        {
            return EXIT_SUCCESS;
        }
        // help command -- display README.txt
        else if (strcmp(command, "help") == 0)
        {
        	//read in the README.txt file and output it line by line
        	//to the console
        
        	help();
        }
        // echo command -- display arg to screen
        else if (strcmp(command, "echo") == 0)
        {
        	echo(arg);
        }
        // pwd command -- display PWD to screen
        else if(strcmp(command, "pwd") == 0)
        {
        	pwd(PWD);
        }
        // pause command -- pause shell operations
        else if(strcmp(command, "pause") == 0)
        {
        	pauseShell();
        }
        // clr command -- clear terminal screen
        else if(strcmp(command, "clr") == 0)
        {
        	clr();
        }
        // ls command -- print contents of PWD
        else if(strcmp(command, "ls") == 0)
        {
        	ls(PWD);
        }
        // environ command -- print environment variables
        else if(strcmp(command, "environ") == 0)
        {
        	penviron(PWD);
        }
        // dir command -- print contents of directory arg
        else if(strcmp(command, "dir") == 0)
        {
        	char direct[BUFFER_LEN];
        	strcpy(direct, PWD);
        	strcat(direct, "/");
        	strcat(direct,arg);
        	dir(direct);
        }
        // Unsupported command
        else
        {
            fputs("Unsupported command, use help to display the manual\n", stderr);
        }
    }
    return EXIT_SUCCESS;
}
