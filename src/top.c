#include <stdlib.h>
#include "common.h"
#include "top.h"
#include "filewriter.h"
#include "malloc_manager.h"

static CommandInfo topCommands[] = {
    {"top -o %MEM", "Sort processes by memory usage", {}},
    {"top -o TIME+", "Sort processes by longest total cpu time", {}},
    {"top -o PID", "Sort processes by PID in ascending order", {}},
    {"top -u root", "Show all processes by specific user", {11}}};

void topCheatSheet()
{
    char *command = getCommand(topCommands, sizeof(topCommands) / sizeof(topCommands[0]));
    if (command != NULL)
    {
        writeToTmpFile(command);
        freePointer(command);
    }
}
