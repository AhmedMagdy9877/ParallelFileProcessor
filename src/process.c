#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "processor.h"
 
int create_processes(char files[][MAX_PATH], int count, pid_t pids[])
{
    int created = 0;
 
    printf("Parent process PID: %d\n", (int)getpid());
    fflush(stdout);                 
    for (int i = 0; i < count; i++) {
        pid_t pid = fork();
 
        if (pid < 0) {              
            perror("fork failed");
            break;                  
        }
 
        if (pid == 0) {            
            printf("  [Child]  PID %d (parent PID %d) processing: %s\n",
                   (int)getpid(), (int)getppid(), files[i]);
            fflush(stdout);
 
            char *args[] = { "wc", files[i], NULL };
            execute_command(args);  
            _exit(127);             
        }

        pids[created++] = pid;      
        printf("  [Parent] created child PID %d for file: %s\n", (int)pid, files[i]);
        fflush(stdout);
    }
 
    return created;
}
 
#ifdef PROCESS_TEST
int main(void)
{
    char files[2][MAX_PATH] = { "input/a.txt", "input/b.txt" };
    pid_t pids[2];
 
    int n = create_processes(files, 2, pids);
    for (int i = 0; i < n; i++)
        waitpid(pids[i], NULL, 0);          /* reap children (no zombies) */
 
    printf("Created and reaped %d child process(es).\n", n);
    return 0;
}
#endif
 