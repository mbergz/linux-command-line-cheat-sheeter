#include <stdlib.h>
#include "common.h"
#include "filewriter.h"

static CommandInfo sedCommands[] = {
    {"sed 's/unix/linux/' file.txt", "Replace the FIRST word occurence of unix with linux in file.txt", {11, 17, 28}},
    {"sed 's/unix/linux/g' file.txt", "Replace ALL word occurence of unix with linux in file.txt", {11, 17, 29}},
    {"sed 's/unix/linux/3' file.txt", "Replace the 3 word occurence of unix with linux in file.txt", {11, 17, 19, 29}},
};

void sedCheatSheet()
{
    char *command = getCommand(sedCommands, sizeof(sedCommands) / sizeof(sedCommands[0]));
    if (command != NULL)
    {
        writeToTmpFile(command);
    }
}
