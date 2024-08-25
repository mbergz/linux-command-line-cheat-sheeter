#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include "find.h"
#include "common.h"
#include "filewriter.h"
#include "malloc_manager.h"

#define MAX_OPTIONS 3

#define STD_OUT_REVERSE "\x1b[7m"
#define STD_OUT_RESET "\x1b[0m"
#define HIDE_CURSOR "\e[?25l"
#define SHOW_CURSOR "\e[?25h"
#define CLEAR_LINE "\r\033[2K"

static CommandInfo fileCommands[] = {
    {"find . -name file.txt", "Find single file in current dir + sub dirs", {6, 21}},
    {"find /home -name '*.json'", "Looks for all files ending with json in /home dir and sub-dirs", {10, 25}},
    {"find . -maxdepth 1 -type f -regex '.*[12].*'", "Find all files in current dir only matching regex containg 1 or 2 in name ", {6, 18, 26, 44}},
    {"find / -size +100M", "Find all files with size larger than 100MB", {6, 18}},
    {"find / -mmin -60", "Find all files which are modified within last hour", {6, 16}}};

static CommandInfo dirCommands[] = {
    {"find /home -type d -name Dir1", "Find all dirs named \"Dir1\" in /home", {10, 18, 29}},
    {"find . -maxdepth 1 -type d", "List all dirs in current folder", {6, 18, 26}}};

void handleFile()
{
    char *command = getCommand(fileCommands, sizeof(fileCommands) / sizeof(fileCommands[0]));
    if (command != NULL)
    {
        writeToTmpFile(command);
        freePointer(command);
    }
}

void handleDir()
{
    char *command = getCommand(dirCommands, sizeof(dirCommands) / sizeof(dirCommands[0]));
    if (command != NULL)
    {
        writeToTmpFile(command);
        freePointer(command);
    }
}

void handleAll()
{
    int fileCommandsSize = sizeof(fileCommands) / sizeof(fileCommands[0]);
    int dirCommandsSize = sizeof(dirCommands) / sizeof(dirCommands[0]);
    int totalSize = fileCommandsSize + dirCommandsSize;

    CommandInfo allCommands[totalSize];
    int index = 0;
    for (int i = 0; i < fileCommandsSize; i++)
    {
        allCommands[index++] = fileCommands[i];
    }
    for (int i = 0; i < dirCommandsSize; i++)
    {
        allCommands[index++] = dirCommands[i];
    }

    char *command = getCommand(allCommands, totalSize);
    if (command != NULL)
    {
        writeToTmpFile(command);
        freePointer(command);
    }
}

void findCheatSheet()
{
    const char *options[MAX_OPTIONS] = {"*", "file", "dir"};
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
        handleAll();
        break;
    case 1:
        handleFile();
        break;
    case 2:
        handleDir();
        break;
    default:
        break;
    }
}
