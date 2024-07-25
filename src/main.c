#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "find.h"
#include "grep.h"
#include "sed.h"
#include "top.h"
#include "ls.h"
#include "curl.h"
#include "common.h"
#include "filewriter.h"

#define NBR_OF_COMMANDS 6

struct LinuxCommand
{
    const char *argument;
    void (*runCheatSheeter)(void);
};

typedef struct LinuxCommand LinuxCommand;

LinuxCommand commands[NBR_OF_COMMANDS] = {
    {"find", &findCheatSheet},
    {"grep", &grepCheatSheet},
    {"sed", &sedCheatSheet},
    {"ls", &lsCheatSheet},
    {"top", &topCheatSheet},
    {"curl", &curlCheatSheet}};

void printHelp()
{
    printf("find\n");
    printf("grep\n");
    printf("sed\n");
    printf("ls\n");
    printf("top\n");
    printf("curl\n");
}

int main(int argc, char *argv[])
{
    char *argument;
    if (argc > 1)
    {
        argument = argv[1];
        if (argument != NULL)
        {
            // Clear the content of tmp file so script doesnt ask to execute old command
            writeToTmpFile("");

            if (strcmp(argument, "-h") == 0 || strcmp(argument, "help") == 0)
            {
                printHelp();
                return 0;
            }

            storeCurrentTerminalMode();
            signal(SIGINT, handleSigint);

            for (int i = 0; i < NBR_OF_COMMANDS; i++)
            {
                if (strcmp(argument, commands[i].argument) == 0)
                {
                    commands[i].runCheatSheeter();
                    return 0;
                }
            }
            printf("Cannot recognize argument: %s\n", argument);
            return 1;
        }
    }
    else
    {
        printf("No arguments provided\n");
        printf("Run with \"-h\" or \"help\" flag for all available commands\n");
    }
    return 0;
}