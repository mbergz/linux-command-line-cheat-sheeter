#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <sys/ioctl.h>
#include "common.h"
#include "cmdprinter.h"

#define CLEAR_LINE "\r\033[2K"

static void printWithoutLineWrap(char *command, int isSelected)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    // printf ("lines %d\n", w.ws_row);
    // printf ("columns %d\n", w.ws_col);

    size_t length = strlen(command);
    size_t maxWidth = w.ws_col;
    if (isSelected == 1)
    {
        maxWidth = maxWidth + 7; // Handle the selected command "> " + bold modifiers taking space in arr
    }

    if (length > maxWidth - 1)
    {
        command[maxWidth - 4] = '.';
        command[maxWidth - 3] = '.';
        command[maxWidth - 2] = '.';
        command[maxWidth - 1] = '\n';
        command[maxWidth] = '\0';
    }

    printf("%s", command);
}

static int calculatePadding(CommandInfo *commands, int size)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int longest = 0;
    for (int i = 0; i < size; i++)
    {
        size_t length = strlen(commands[i].command);
        if (length >= longest)
        {
            longest = length;
        }
    }

    int threshold = (int)(w.ws_col * 65 / 100); // 65% of terminal width

    if (longest < (threshold - 10))
    {
        threshold = longest + 10;
    }

    return threshold;
}

static char *adjustCommandForPrint(const char *command)
{
    size_t len = strlen(command);
    char *modified = (char *)malloc(len + 1);
    if (modified == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    strcpy(modified, command);
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int threshold = (int)(w.ws_col * 65 / 100); // 65% of terminal width
    if (len >= threshold)
    {
        int cutOffIndex = threshold;

        modified[cutOffIndex - 3] = '.';
        modified[cutOffIndex - 2] = '.';
        modified[cutOffIndex - 1] = '.';
        modified[cutOffIndex] = '\0';
    }

    return modified;
}

void printCommands(CommandInfo *commands, int size, int selectedCommand)
{
    int padding = calculatePadding(commands, size);

    for (int i = 0; i < size; i++)
    {
        char *modifiedCommand = adjustCommandForPrint(commands[i].command);
        char buffer[1024];
        printf(CLEAR_LINE);
        if (i == selectedCommand)
        {
            snprintf(buffer, sizeof(buffer), "> \e[1m%-*s\e[m %s\n", padding - 2, modifiedCommand, commands[i].description);
            printWithoutLineWrap(buffer, true);
        }
        else
        {
            snprintf(buffer, sizeof(buffer), "%-*s %s\n", padding, modifiedCommand, commands[i].description);
            printWithoutLineWrap(buffer, false);
        }
        free(modifiedCommand);
    }
}
