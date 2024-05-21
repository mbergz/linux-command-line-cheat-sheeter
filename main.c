#include <stdio.h>
#include <string.h>
#include "csfind.h"
#include "csgrep.h"

int main(int argc, char *argv[])
{
    char *argument;
    if (argc > 1)
    {
        argument = argv[1];
    }
    if (argument != NULL)
    {
        if (strcmp(argument, "find") == 0)
        {
            printFindCheatSheet();
        }
        else if (strcmp(argument, "grep") == 0)
        {
            printGrepCheatSheet();
        }
    }
    else
    {
        printf("No arguments provided");
    }
    return 0;
}