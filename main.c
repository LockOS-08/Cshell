#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

int main() {
    
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    
    while (1) {
        printf("csh> ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0) {
            continue;
        }
        
        char* saveptr;
        int i = 0;
        char *token = strtok_r(input, " \t", &saveptr);

        while (token != NULL && i < MAX_ARGS - 1) {
            args[i++] = token;
            token = strtok_r(NULL, " \t", &saveptr);
        }

        args[i] = NULL;

        if (strcmp(args[0], "exit") == 0) {
            break;
        }

        pid_t pid = fork();

        if (pid < 0) {
            perror("clone failed");
            continue;
        }

        if (pid == 0) {
            execvp(args[0], args);

            perror("command failed");
            _exit(EXIT_FAILURE);
        }

        else {
            waitpid(pid, NULL, 0);
        }
    }

    return 0;
}