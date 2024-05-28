#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
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
    {"grep -i --color=always \"word\" *.c", "Find all the files match word in current dir + recursive", {15, 17}},
};

void grepCheatSheet()
{
    char *command = getCommand(grepCommands, sizeof(grepCommands) / sizeof(grepCommands[0]));
    if (command != NULL)
    {
        printf("%s\n", command);
        printf("\033[90mAdd color to output? [y or enter] \033[0m ");
        char c;
        scanf("%c", &c);
        c = toupper(c);

        if (c == 'Y' || c == '\n')
        {
            // replace grep with grep --color=always
            const char *grepWithColor = "grep --color=always";
            size_t newLength = strlen(grepWithColor) + strlen(command) - 3;
            char *modified = malloc(newLength);
            if (modified == NULL)
            {
                perror("malloc");
                return;
            }
            strcpy(modified, grepWithColor);
            strcat(modified, command + 4); // Move pointer 4 steps
            free(command);

            executeCommand(modified);
        }
        else
        {
            executeCommand(command);
        }
    }
}
