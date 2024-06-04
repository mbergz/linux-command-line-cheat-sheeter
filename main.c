#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "find.h"
#include "grep.h"
#include "common.h"

int main(int argc, char *argv[])
{
    storeCurrentTerminalMode();
    signal(SIGINT, handleSigint);

    char *argument;
    if (argc > 1)
    {
        argument = argv[1];
        if (argument != NULL)
        {
            if (strcmp(argument, "find") == 0)
            {
                findCheatSheet();
            }
            else if (strcmp(argument, "grep") == 0)
            {
                grepCheatSheet();
            }
        }
    }
    else
    {
        printf("No arguments provided\n");
    }
    return 0;
}