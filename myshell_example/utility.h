/*
 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2017, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#ifndef UTILITY_H_
#define UTILITY_H_

// Include your relevant functions declarations here they must start with the 
// extern keyword such as in the following example:
// extern void display_help(void);

extern void echo(char* arg);
extern void help(void);
extern void pwd(char* wd);
extern void pauseShell(void);
extern int cd(char* dir);

#endif /* UTILITY_H_ */