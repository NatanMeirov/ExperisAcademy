#include <string.h> /* strcmp, strlen */
#include <stdio.h> /* printf */
#include <stdlib.h> /* exit, system */
#include <stddef.h> /* size_t */
#include <unistd.h> /* fork, execvp */
#include <sys/wait.h> /* waitpid */

#include "NMShell.h"


#define TRUE 1
#define ARG_MAX (128*1024)
#define ARGS_COUNT 128


void ShowPrompt(void);
void ReadCommand(char* buffer);
int IsQuit(const char* command);
int IsHelp(const char* command);
void ShowHelp(void);
void MapCommandToArgs(char* command, char** arguments);


int RunNMShell(void)
{
    int waitStatus = 0;
    char command[ARG_MAX];
    char* arguments[ARGS_COUNT] = {0};


    system("clear");
    while(TRUE)
    {
        ShowPrompt();
        ReadCommand(command);
        if(IsQuit(command))
        {
            break;
        }

        if(IsHelp(command))
        {
            ShowHelp();
            continue;
        }

        MapCommandToArgs(command, arguments);

        if(fork() != 0)
        {
            /* Parent Code */
            waitpid(-1, &waitStatus, 0);
        }
        else
        {
            /* Child Code */
            execvp(arguments[0], arguments);

            /* If execution fails, this command will be triggered (because when exec is successfully triggered - all the child proccess will delete the parent's code and launch the triggered executable code): */
            printf("Command not found...\n");
            exit(1);
        }
    }

    return 0;
}

void ShowPrompt(void)
{
    fflush(stdout);
    printf("root@root:~$ ");
    fflush(stdout);
}

int IsQuit(const char* command)
{
    return (strcmp(command, "quit") == 0 || strcmp(command, "q") == 0 || strcmp(command, "exit") == 0) ? 1 : 0;
}

int IsHelp(const char* command)
{
    return (strcmp(command, "help") == 0 || strcmp(command, "h") == 0) ? 1 : 0;
}

void ShowHelp(void)
{
    printf("How to use the NMShell:"
           "\n- Use \"q\" / \"quit\" / \"exit\" commands to exit the NMShell"
           "\n- Use \"h\" / \"help\" commands to show help message of the NMShell"
           "\n- All commands and their arguments should be space separated"
           "\n- Maximum input length is (128*1024) = 131072 characters"
           "\n- Maximum args count is 128\n");
}

void ReadCommand(char* buffer)
{
    size_t bufSize;

    fflush(stdin);
    fgets(buffer, ARG_MAX, stdin);
    bufSize = strlen(buffer);

    if(buffer[bufSize - 1] == '\n')
    {
        buffer[bufSize - 1] = '\0';
    }
    fflush(stdin);
}

void MapCommandToArgs(char* command, char** arguments)
{
    size_t tokens = 0;

    arguments[tokens++] = strtok(command, " \n"); /* First token */

    while(tokens < ARGS_COUNT - 1 && (arguments[tokens] = strtok(NULL, " \n")))
    {
        tokens++;
    }

    if(tokens == ARGS_COUNT - 1)
    {
        arguments[tokens] = NULL;
    }
}
