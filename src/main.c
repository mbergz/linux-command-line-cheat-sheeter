#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "find.h"
#include "grep.h"
#include "sed.h"
#include "common.h"
#include "filewriter.h"

void printHelp()
{
    printf("find\n");
    printf("grep\n");
    printf("sed\n");
}

int main(int argc, char *argv[])
{
    // Clear the content of tmp file so script doesnt ask to execute old command
    writeToTmpFile("");

    storeCurrentTerminalMode();
    signal(SIGINT, handleSigint);

    char *argument;
    if (argc > 1)
    {
        argument = argv[1];
        if (argument != NULL)
        {
            if (strcmp(argument, "-h") == 0 || strcmp(argument, "help") == 0)
            {
                printHelp();
            }
            if (strcmp(argument, "find") == 0)
            {
                findCheatSheet();
            }
            else if (strcmp(argument, "grep") == 0)
            {
                grepCheatSheet();
            }
            else if (strcmp(argument, "sed") == 0)
            {
                sedCheatSheet();
            }
        }
    }
    else
    {
        printf("No arguments provided\n");
        printf("Run with \"-h\" or \"help\" flag for all available commands\n");
    }
    return 0;
}