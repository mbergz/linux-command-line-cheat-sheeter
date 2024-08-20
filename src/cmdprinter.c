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

#define ELLIPSIS "..."
#define ELLIPSIS_WITH_NEWLINE "...\n"

static int horizontalOffset = 0;

static void applyEllipsisEndOfStringWithNewline(char *str, int threshold)
{
    memcpy(str + threshold - 4, ELLIPSIS_WITH_NEWLINE, 4);
    str[threshold] = '\0';
}

static void applyEllipsisEndOfString(char *str, int threshold)
{
    memcpy(str + threshold - 3, ELLIPSIS, 3);
    str[threshold] = '\0';
}

static void printWithoutLineWrap(char *command, int isSelected)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    size_t length = strlen(command);
    size_t maxWidth = w.ws_col;
    if (isSelected == 1)
    {
        maxWidth = maxWidth + 7; // Handle the selected command "> " + bold modifiers taking space in arr
    }

    if (length > maxWidth - 1)
    {
        applyEllipsisEndOfStringWithNewline(command, maxWidth);
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

static int calculateOffset(size_t len, int threshold)
{
    int offset;
    if (len - horizontalOffset >= threshold)
    {
        offset = horizontalOffset;
    }
    else
    {
        offset = len - threshold;
    }
    return offset;
}

static char *adjustCommandForPrint(const char *command, int adjustment)
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
        int offset = calculateOffset(len, threshold);

        char *shifted = (char *)malloc(len + 1);
        if (shifted == NULL)
        {
            fprintf(stderr, "Memory allocation failed\n");
            free(modified);
            exit(EXIT_FAILURE);
        }

        strcpy(shifted, modified + offset);
        free(modified);

        if (strlen(shifted) > threshold)
        {
            applyEllipsisEndOfString(shifted, threshold);
        }

        return shifted;
    }

    return modified;
}

static char *adjustDescriptionForPrint(const char *description, int adjustment)
{
    size_t len = strlen(description);
    char *modified = (char *)malloc(len + 1);
    if (modified == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    strcpy(modified, description);

    return modified;
}

static void incrementAdjustment(int adjustment)
{
    if (adjustment == 0)
    {
        return;
    }
    if (horizontalOffset > 0 && adjustment == -1)
    {
        horizontalOffset--;
    }
    else if (adjustment == 1)
    {
        horizontalOffset++;
    }
}

// 0 = no adjustment, 1 = right, -1 = left
static void printCommandsInternal(CommandInfo *commands, int size, int selectedCommand, int adjustment)
{
    int padding = calculatePadding(commands, size);
    incrementAdjustment(adjustment);

    for (int i = 0; i < size; i++)
    {
        char *modifiedCommand = adjustCommandForPrint(commands[i].command, adjustment);
        char *modifiedDescription = adjustDescriptionForPrint(commands[i].description, adjustment);
        char buffer[1024];
        printf(CLEAR_LINE);
        if (i == selectedCommand)
        {
            snprintf(buffer, sizeof(buffer), "> \e[1m%-*s\e[m %s\n", padding - 2, modifiedCommand, modifiedDescription);
            printWithoutLineWrap(buffer, true);
        }
        else
        {
            snprintf(buffer, sizeof(buffer), "%-*s %s\n", padding, modifiedCommand, modifiedDescription);
            printWithoutLineWrap(buffer, false);
        }
        free(modifiedCommand);
        free(modifiedDescription);
    }
}

void printCommands(CommandInfo *commands, int size, int selectedCommand)
{
    printCommandsInternal(commands, size, selectedCommand, 0);
}

void printCommandsStepRight(CommandInfo *commands, int size, int selectedCommand)
{
    printCommandsInternal(commands, size, selectedCommand, 1);
}

void printCommandsStepLeft(CommandInfo *commands, int size, int selectedCommand)
{
    printCommandsInternal(commands, size, selectedCommand, -1);
}
