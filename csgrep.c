#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include "csfind.h"
#include "common.h"

#define MAX_OPTIONS 1

static CommandInfo grepCommands[] = {
    {"grep boo file.txt", "Search the file.txt for a line matching boo", {8, 17}},
    {"grep -i \"word\" file.txt", "Search the file.txt for a line matching case insensitive word", {14, 23}},
    {"grep -c \"word\" file.txt", "Find number of lines that match word in file.txt", {14, 23}},
    {"grep -n \"word\" file.txt", "Show line numberof liines that match word in file.txt", {14, 23}},
    {"grep -l \"word\" *", "Find all the files match word in current dir", {14, 16}},
    {"grep -rl \"word\" *", "Find all the files match word in current dir + recursive", {15, 17}},
};

void printGrepCheatSheet()
{
    printSelectableCommands(grepCommands, sizeof(grepCommands) / sizeof(grepCommands[0]));
}
