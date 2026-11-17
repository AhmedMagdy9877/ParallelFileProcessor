#include "processor.h"
#include <string.h>
#include <ctype.h>

void run_cli(void)
{
    char command[MAX_PATH + 64];
    char files[MAX_FILES][MAX_PATH];
    pid_t pids[MAX_FILES];

    char last_directory[MAX_PATH] = "";
    int last_count = 0;
    int has_previous_run = 0;

    printf("\n=== Parallel File Processor CLI ===\n");
    printf("Commands:\n");
    printf("process <dir_path>\n");
    printf("status\n");
    printf("exit\n");

    while (1)
    {
        printf("\nCLI> ");

        if (fgets(command, sizeof(command), stdin) == NULL)
            break;

        command[strcspn(command, "\n")] = '\0';

        char *input = command;

        while (isspace((unsigned char)*input))
            input++;

        char *end = input + strlen(input);

        while (end > input && isspace((unsigned char)*(end - 1)))
            *--end = '\0';

        if (strcmp(input, "exit") == 0)
            {
            printf("Exiting CLI. Goodbye!\n");
            break;
        }
        else if (strcmp(input, "status") == 0)
        {
            if (has_previous_run)
            {
                printf("Last directory: %s\n", last_directory);
                printf("Files processed: %d\n", last_count);
                printf("Last processing session completed.\n");
            }
            else
            {
                printf("No processing session yet.\n");
            }
        }
        else if (strncmp(input, "process ", 8) == 0)
        {
            char *dir_path = input + 8;

            while (isspace((unsigned char)*dir_path))
                dir_path++;

            if (*dir_path == '\0')
            {
                printf("Please provide a directory path.\n");
                continue;
            }

            if (strlen(dir_path) >= MAX_PATH)
            {
                printf("Directory path is too long.\n");
                continue;
            }

            printf("Scanning directory: %s\n", dir_path);

            int file_count = scan_directory(dir_path, files);

            if (file_count <= 0)
            {
                printf("No files found or directory error.\n");
                continue;
            }

            printf("Found %d file(s).\n", file_count);
            printf("Creating child processes...\n");

            int created = create_processes(files, file_count, pids);

            if (created <= 0)
            {
                printf("Failed to create processes.\n");
                continue;
            }

            strcpy(last_directory, dir_path);
            last_count = created;
            has_previous_run = 1;

            printf("Created %d child process(es).\n", created);

            monitor_processes(pids, created);

            printf("All child processes finished.\n");
        }
        else
        {
            printf("Unknown command. Try process <dir_path>, status, or exit.\n");
        }
    }
}