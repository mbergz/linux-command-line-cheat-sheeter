#include <stdio.h>
#include <string.h>
#include <stdbool.h>
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

void enableNonCanonicalMode(struct termios *old, bool disableEcho)
{
    tcgetattr(0, old);
    struct termios new;
    new = *old;
    // c_lflag = local mode
    // ICANON = canonincal mode
    new.c_lflag &= ~ICANON;
    if (disableEcho)
    {
        // ECHO = echos input to terminal
        new.c_lflag &= ~ECHO;
    }
    // TCSANOW means apply changes immediately
    tcsetattr(0, TCSANOW, &new);
}

void resetTerminalMode(struct termios *old)
{
    tcsetattr(0, TCSANOW, old);
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

void printSelectableCommands(CommandInfo *commands, int size)
{
    struct termios old;
    enableNonCanonicalMode(&old, true);
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
    printf("Selected line: %s\n", commands[selectedLine].command);
    resetTerminalMode(&old);
}

void editLineAtIndex(int index, char *line)
{
    printf("\r");
    printf("\033[%dC", index); // move cursor to starting pos

    char c;
    while ((c = getchar()) != '\n')
    {
        printf(CLEAR_LINE);
        for (int j = strlen(line); j >= index; j--)
        {
            line[j + 1] = line[j];
        }
        line[index++] = c;

        printf("%s", line);
        printf("\r");
        printf("\033[%dC", index);
    }
}

void testEditString()
{
    char original_content[100] = "Now test line";
    printf("%s", original_content);
    fflush(stdout);

    struct termios old;
    enableNonCanonicalMode(&old, false);

    editLineAtIndex(6, original_content);
    printf("\n%s\n", original_content);

    resetTerminalMode(&old);
}

void printFile()
{
    printSelectableCommands(fileCommands, sizeof(fileCommands) / sizeof(fileCommands[0]));
    // testEditString();
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
    struct termios old;

    const char *options[MAX_OPTIONS] = {"*", "file", "dir"};
    int selectedOption = 0;
    char c;

    enableNonCanonicalMode(&old, true);
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
            selectedOption = (selectedOption + 1) % MAX_OPTIONS; // Move to next option
            // printOptions(options, selectedOption);
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
