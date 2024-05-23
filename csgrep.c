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

#define HIDE_CURSOR "\e[?25l"
#define SHOW_CURSOR "\e[?25h"
#define CLEAR_LINE "\r\033[2K"

static CommandInfo grepCommands[] = {
    {"grep boo file.txt", "Search the file.txt for a line matching boo", {8, 17}},
};

void printGrepCheatSheet()
{
    storeCurrentTerminalMode();
    signal(SIGINT, handleSigint);
    const char *options[MAX_OPTIONS] = {"*"};
    int selectedOption = 0;
    char c;

    enableNonCanonicalMode();
    printf(HIDE_CURSOR);

    while (1)
    {
        printOptions(options, selectedOption, MAX_OPTIONS);

        if (read(STDIN_FILENO, &c, 1) == -1)
        {
            perror("read");
        }

        if (c == '\t')
        {
            selectedOption = (selectedOption + 1) % MAX_OPTIONS;
        }
        else if (c == '\n')
        {
            // Exit loop on Enter key
            break;
        }
    }

    printf(SHOW_CURSOR);
    printf(CLEAR_LINE);
    resetTerminalMode();

    switch (selectedOption)
    {
    case 0:
        printSelectableCommands(grepCommands, sizeof(grepCommands) / sizeof(grepCommands[0]));
        break;
    default:
        break;
    }
}
