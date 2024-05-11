#include <stdio.h>
#include "csfind.h"
#include <unistd.h>
#include <termios.h>
#include "common.h"

#define MAX_OPTIONS 3

#define STD_OUT_REVERSE "\x1b[7m"
#define STD_OUT_RESET "\x1b[0m"
#define HIDE_CURSOR "\e[?25l"
#define SHOW_CURSOR "\e[?25h"
#define CLEAR_LINE "\r\033[2K"

static CommandInfo fileCommands[] = {
    {"find . -name file.txt", "Find single file in current dir + sub dirs"},
    {"find /home -name *.json", "Looks for all files ending with json in /home dir and sub-dirs"},
    {"find . -maxdepth 1 -type f -regex '.*[12].*'", "Find all files in current dir only matching regex containg 1 or 2 in name "},
    {"find / -size +100M", "Find all files with size larger than 100MB"},
    {"find / -mmin -60", "Find all files which are modified within last hour"}};

static CommandInfo dirCommands[] = {
    {"find /home -type d -name Dir1", "Find all dirs named \"Dir1\" in /home"},
    {"find . -maxdepth 1 -type d", "List all dirs in current folder"}};

void printCommands(CommandInfo *commands, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%-50s %s\n", commands[i].command, commands[i].description);
    }
}

void printFile()
{
    for (int i = 0; i < sizeof(fileCommands) / sizeof(fileCommands[0]); i++)
    {
        printf("%-50s %s\n", fileCommands[i].command, fileCommands[i].description);
    }
}

void printDir()
{
    printCommands(dirCommands, sizeof(dirCommands) / sizeof(dirCommands[0]));
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

void enableNonCanonicalMode(struct termios *old)
{
    tcgetattr(0, old);
    struct termios new;
    new = *old;
    // c_lflag = local mode
    // ICANON = canonincal mode
    // ECHO = echos input to terminal
    new.c_lflag &= ~(ICANON | ECHO);
    // TCSANOW means apply changes immediately
    tcsetattr(0, TCSANOW, &new);
}

void resetTerminalMode(struct termios *old)
{
    tcsetattr(0, TCSANOW, old);
}

void printFindCheatSheet()
{
    struct termios old;

    const char *options[MAX_OPTIONS] = {"*", "file", "dir"};
    int selectedOption = 0;
    char c;

    enableNonCanonicalMode(&old);
    printf(HIDE_CURSOR);

    while (1)
    {
        printOptions(options, selectedOption);

        // Read a single character
        if (read(STDIN_FILENO, &c, 1) == -1)
        {
            perror("read");
        }

        if (c == '\t')
        {
            selectedOption = (selectedOption + 1) % MAX_OPTIONS; // Move to next option
            printOptions(options, selectedOption);
        }
        else if (c == '\n')
        {
            // Exit loop on Enter key
            break;
        }
    }

    printf(SHOW_CURSOR);
    printf(CLEAR_LINE);
    resetTerminalMode(&old);

    // printf("\nSelected option: %s\n", options[selectedOption]);

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
