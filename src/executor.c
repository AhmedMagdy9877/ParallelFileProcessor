#include <stdio.h>
#include <unistd.h>
#include "processor.h"

void execute_command(char *const args[])
{
    if (args == NULL || args[0] == NULL) {
        fprintf(stderr, "execute_command: no command given\n");
        _exit(127);
    }

    execvp(args[0], args);

    perror("execvp failed");
    _exit(127);     
}

#ifdef EXECUTOR_TEST
int main(void)
{
    char *good[] = { "wc", "input/a.txt", NULL };
    char *bad[]  = { "no_such_program", NULL };
    char **cmds[] = { good, bad };

    for (int i = 0; i < 2; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            execute_command(cmds[i]);
            _exit(1);                       
        }
        int status;
        waitpid(pid, &status, 0);
        printf("Child %d (%s) exited with status %d\n",
               (int)pid, cmds[i][0], WEXITSTATUS(status));
    }
    return 0;
}
#endif