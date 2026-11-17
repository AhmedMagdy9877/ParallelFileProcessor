#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include "processor.h"

static int has_txt_extension(const char *name)
{
    size_t len = strlen(name);
    return len > 4 && strcasecmp(name + len - 4, ".txt") == 0;
}

static int compare_paths(const void *a, const void *b)
{
    return strcmp((const char *)a, (const char *)b);
}

int scan_directory(const char *dir_path, char files[][MAX_PATH])
{
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir failed");
        return -1;
    }

    size_t dir_len = strlen(dir_path);
    const char *sep = (dir_len > 0 && dir_path[dir_len - 1] == '/') ? "" : "/";

    int count = 0;
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (!has_txt_extension(entry->d_name))
            continue;

        char path[MAX_PATH];
        int written = snprintf(path, sizeof(path), "%s%s%s", dir_path, sep, entry->d_name);
        if (written < 0 || written >= (int)sizeof(path)) {
            fprintf(stderr, "Skipping '%s': path too long\n", entry->d_name);
            continue;
        }

        struct stat st;
        if (stat(path, &st) != 0 || !S_ISREG(st.st_mode))
            continue;

        if (count >= MAX_FILES) {
            fprintf(stderr, "Warning: more than %d files, extra files ignored\n", MAX_FILES);
            break;
        }

        strcpy(files[count], path);   
        count++;
    }

    closedir(dir);

    qsort(files, (size_t)count, MAX_PATH, compare_paths);
    return count;
}

#ifdef SCANNER_TEST
int main(int argc, char *argv[])
{
    const char *dir = (argc > 1) ? argv[1] : "input";
    char files[MAX_FILES][MAX_PATH];

    int n = scan_directory(dir, files);
    if (n < 0)
        return 1;

    printf("Found %d text file(s) in '%s':\n", n, dir);
    for (int i = 0; i < n; i++)
        printf("  %d. %s\n", i + 1, files[i]);
    return 0;
}
#endif