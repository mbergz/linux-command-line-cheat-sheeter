#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include "common.h"

#define STD_OUT_REVERSE "\x1b[7m"
#define STD_OUT_RESET "\x1b[0m"
#define HIDE_CURSOR "\e[?25l"
#define SHOW_CURSOR "\e[?25h"
#define CLEAR_LINE "\r\033[2K"

static struct termios old;

void storeCurrentTerminalMode()
{
    // Store default terminos settings
    tcgetattr(0, &old);
}

void resetTerminalMode()
{
    printf(SHOW_CURSOR);
    tcsetattr(0, TCSANOW, &old);
}

void handleSigint(int sig)
{
    resetTerminalMode(&old);
    exit(EXIT_FAILURE);
}

void enableNonCanonicalMode()
{
    struct termios new;
    tcgetattr(0, &new);
    // c_lflag = local mode
    // ICANON = canonincal mode
    // ECHO = echos input to terminal
    new.c_lflag &= ~(ICANON | ECHO);
    // TCSANOW means apply changes immediately
    tcsetattr(0, TCSANOW, &new);
}

void printCommands(CommandInfo *commands, int size, int selectedCommand)
{
    for (int i = 0; i < size; i++)
    {
        if (i == selectedCommand)
        {
            printf("> \e[1m%-50s\e[m %s\n", commands[i].command, commands[i].description);
        }
        else
        {
            printf("%-50s %s\n", commands[i].command, commands[i].description);
        }
    }
}

void editLineAtIndex(int index, char *line, int *offset)
{
    int newOffset = 0;
    index = index + *offset;
    printf("\r");
    printf("\033[%dC", index); // move cursor to starting pos

    char c;
    while ((c = getchar()) != '\n')
    {
        printf(CLEAR_LINE);
        if (c == '\b' || c == '\x7F')
        {
            for (int i = index; i < strlen(line); i++)
            {
                line[i - 1] = line[i];
            }
            line[strlen(line) - 1] = (char)0;
            index--;
            newOffset--;
        }
        else
        {
            for (int j = strlen(line); j >= index; j--)
            {
                line[j + 1] = line[j];
            }
            line[index++] = c;
            newOffset++;
        }

        printf("%s", line);
        printf("\r");
        printf("\033[%dC", index);
    }
    *offset = *offset + newOffset;
}

void editCommand(CommandInfo commandInfo)
{
    char line[100];
    strcpy(line, commandInfo.command);
    printf("%s", line);
    fflush(stdout);

    enableNonCanonicalMode();

    int offset = 0;
    for (int i = 0; i < sizeof(commandInfo.editIndexes) / sizeof(commandInfo.editIndexes[0]); i++)
    {
        if (commandInfo.editIndexes[i] == 0)
        {
            break;
        }
        editLineAtIndex(commandInfo.editIndexes[i], line, &offset);
    }
    printf(CLEAR_LINE);

    printf("%s\n", line);
    resetTerminalMode();
}

void printSelectableCommands(CommandInfo *commands, int size)
{
    enableNonCanonicalMode();
    printf(HIDE_CURSOR);
    int selectedLine = 0;
    char input;
    while (1)
    {
        printCommands(commands, size, selectedLine);
        if (read(STDIN_FILENO, &input, 1) == -1)
        {
            perror("read");
        }
        if (input == '\t')
        {
            selectedLine = (selectedLine + 1) % size;
            printf("\033[%dA", size); // move cursor up x lines
        }
        if (input == '\n')
        {
            break;
        }
    }
    printf(SHOW_CURSOR);
    // Now erase all lines
    for (int i = 0; i < size; i++)
    {
        printf("\033[%dA", 1);
        printf(CLEAR_LINE);
    }
    resetTerminalMode();
    editCommand(commands[selectedLine]);
}

void printOptions(const char *options[], int selectedOption, int maxOptions)
{
    printf(CLEAR_LINE);

    for (int i = 0; i < maxOptions; ++i)
    {
        if (i == selectedOption)
        {
            printf("%s%s%s ", STD_OUT_REVERSE, options[i], STD_OUT_RESET); // Print highlighted option
        }
        else
        {
            printf("%s ", options[i]);
        }
    }

    fflush(stdout);
}