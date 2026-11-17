#ifndef PROCESSOR_H
#define PROCESSOR_H
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
 
#define MAX_PATH  256
#define MAX_FILES 100
 
typedef struct {
    int   id;
    char  filename[MAX_PATH];
    pid_t pid;
    int   status;
} Task;
 
int  scan_directory(const char *dir_path, char files[][MAX_PATH]);

int  create_processes(char files[][MAX_PATH], int count, pid_t pids[]);

void execute_command(char *const args[]);

void monitor_processes(pid_t pids[], int count);

void run_cli(void);
 
#endif
