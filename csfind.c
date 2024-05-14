#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include "csfind.h"
#include "common.h"

#define MAX_OPTIONS 3

#define STD_OUT_REVERSE "\x1b[7m"
#define STD_OUT_RESET "\x1b[0m"
#define HIDE_CURSOR "\e[?25l"
#define SHOW_CURSOR "\e[?25h"
#define CLEAR_LINE "\r\033[2K"

static CommandInfo fileCommands[] = {
    {"find . -name file.txt", "Find single file in current dir + sub dirs", {6, 21}},
    {"find /home -name *.json", "Looks for all files ending with json in /home dir and sub-dirs", {10, 23}},
    {"find . -maxdepth 1 -type f -regex '.*[12].*'", "Find all files in current dir only matching regex containg 1 or 2 in name ", {6, 18, 26, 44}},
    {"find / -size +100M", "Find all files with size larger than 100MB", {6, 18}},
    {"find / -mmin -60", "Find all files which are modified within last hour", {6, 16}}};

static CommandInfo dirCommands[] = {
    {"find /home -type d -name Dir1", "Find all dirs named \"Dir1\" in /home", {10, 18, 29}},
    {"find . -maxdepth 1 -type d", "List all dirs in current folder", {6, 18, 26}}};

static struct termios old;

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

    printf("\n%s\n", line);

    resetTerminalMode();
}

void printSelectableCommands(CommandInfo *commands, int size)
{
    enableNonCanonicalMode();
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
    // Now erase all lines
    for (int i = 0; i < size; i++)
    {
        printf("\033[%dA", 1);
        printf(CLEAR_LINE);
    }
    resetTerminalMode();
    editCommand(commands[selectedLine]);
}

void printFile()
{
    printSelectableCommands(fileCommands, sizeof(fileCommands) / sizeof(fileCommands[0]));
}

void printDir()
{
    printSelectableCommands(dirCommands, sizeof(dirCommands) / sizeof(dirCommands[0]));
}

void printAll()
{
    printFile();
    printDir();
}

void printOptions(const char *options[], int selectedOption)
{
    printf(CLEAR_LINE);

    for (int i = 0; i < MAX_OPTIONS; ++i)
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

void printFindCheatSheet()
{
    // Store default terminos settings
    tcgetattr(0, &old);
    signal(SIGINT, handleSigint);
    const char *options[MAX_OPTIONS] = {"*", "file", "dir"};
    int selectedOption = 0;
    char c;

    enableNonCanonicalMode();
    printf(HIDE_CURSOR);

    while (1)
    {
        printOptions(options, selectedOption);

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
        printAll();
        break;
    case 1:
        printFile();
        break;
    case 2:
        printDir();
        break;
    default:
        break;
    }
}
