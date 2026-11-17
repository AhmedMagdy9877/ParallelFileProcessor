#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "processor.h"

void monitor_processes(pid_t pids[], int count) {
   int status;
   pid_t child_pid;
   for (int i = 0; i < count; i++) {
      child_pid = waitpid(pids[i], &status, 0);
      if (child_pid == -1) { 
        perror("waitpid failed"); 
        continue; 
    }
      if (WIFEXITED(status)) {
         int exit_status = WEXITSTATUS(status);
         printf("Child PID %d finished with status %d.\n", child_pid, exit_status);
      } else {
         printf("Child PID %d did not exit normally (terminated by signal).\n", child_pid);
      }
   }
}

#ifdef MONITOR_TEST
int main() {
   pid_t pids[2];
   for (int i = 0; i < 2; i++) {
      pids[i] = fork();
      if (pids[i] == 0) { 
        sleep(1); 
        exit(i); 
    }
   }
   monitor_processes(pids, 2);
   return 0;
}
#endif