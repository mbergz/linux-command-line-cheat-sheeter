#include <stdlib.h>
#include "common.h"
#include "filewriter.h"
#include "malloc_manager.h"

static CommandInfo sedCommands[] = {
    {"sed 's/unix/linux/' file.txt", "View file.txt if the FIRST word occurence of unix is replaced with linux", {11, 17, 28}},
    {"sed -i 's/unix/linux/' file.txt", "Perform replace in file.txt (-i) the FIRST word occurence of unix with linux", {11, 17, 28}},
    {"sed 's/unix/linux/i' file.txt", "View file.txt if the FIRST word occurence of unix is replaced (ignore case sensitive) with linux", {11, 17, 29}},
    {"sed 's/unix/linux/g' file.txt", "View file.txt if ALL word occurences of unix is replaced with linux", {11, 17, 29}},
    {"sed 's/unix/linux/3' file.txt", "View file.txt if the first 3 word occurence of unix is replaced with linux", {11, 17, 19, 29}},
    {"sed -n '/unix/,/linux/p' file.txt", "Only print the lines matching and between unix and linux in file.txt", {14, 20, 32}},
    {"sed -n '3,5p' file.txt", "Print lines 3-5 in file.txt", {9, 11, 22}},
    {"sed '/delete-me/d' file.txt", "View delete pattern matched 'delete-me' in file.txt", {15, 27}},
    {"sed -i '/delete-me/d' file.txt", "Perform delete pattern matched 'delete-me' in file.txt", {18, 30}},
};

void sedCheatSheet()
{
    char *command = getCommand(sedCommands, sizeof(sedCommands) / sizeof(sedCommands[0]));
    if (command != NULL)
    {
        writeToTmpFile(command);
        freePointer(command);
    }
}
