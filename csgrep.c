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
    {"grep -i word file.txt", "Search the file.txt for a line matching case insensitive word", {12, 21}},
};

void printGrepCheatSheet()
{
    printSelectableCommands(grepCommands, sizeof(grepCommands) / sizeof(grepCommands[0]));
}
