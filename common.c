#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <termios.h>
#include <signal.h>
#include "common.h"
#include <readline/readline.h>
#include <readline/history.h>

#define STD_OUT_REVERSE "\x1b[7m"
#define STD_OUT_RESET "\x1b[0m"
#define HIDE_CURSOR "\e[?25l"
#define SHOW_CURSOR "\e[?25h"
#define CLEAR_LINE "\r\033[2K"

#define ANSI_COLOR_LIGHT_GRAY "\033[90m"
#define ANSI_COLOR_RESET "\033[0m"

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

void insertString(char *line, int index, char *strToInsert)
{
    int lineLen = strlen(line);
    int insertLen = strlen(strToInsert);

    for (int j = lineLen; j >= index; j--)
    {
        line[j + insertLen] = line[j];
    }

    strncpy(line + index, strToInsert, insertLen);
}

void handleTabPress(char *line, int *index, int *newOffset)
{
    resetTerminalMode();

    char *input = NULL;
    char prompt[100];

    strcpy(prompt, ANSI_COLOR_LIGHT_GRAY);
    strcat(prompt, "> ");
    strcat(prompt, ANSI_COLOR_RESET);

    input = readline(prompt);

    enableNonCanonicalMode();
    printf("\033[1A");
    printf(CLEAR_LINE);

    insertString(line, *index, input);
    *newOffset += strlen(input);
    *index += strlen(input);
    free(input);
}

void handleArrowKeys(char *line, int *index)
{
    printf(CLEAR_LINE);
    char next = getchar();
    if (next == 68)
    { // left
        if (*index > 0)
        {
            (*index)--;
        }
    }
    else if (next == 67)
    { // right
        if (*index < strlen(line))
        {
            (*index)++;
        }
    }
}

int editLineAtIndex(int index, char *line, int *offset)
{
    int newOffset = 0;
    index = index + *offset;
    printf("\r");
    printf("\033[%dC", index); // move cursor to starting pos

    char c;
    while (1)
    {
        c = getchar();
        if (c == '\n')
        {
            break;
        }
        if (c == 27)
        {
            if (getchar() == 91)
            {
                handleArrowKeys(line, &index);
            }
            else
            { // only ESC key pressed
                return -1;
            }
        }
        else if (c == '\b' || c == '\x7F')
        {
            printf(CLEAR_LINE);
            for (int i = index; i < strlen(line); i++)
            {
                line[i - 1] = line[i];
            }
            line[strlen(line) - 1] = (char)0;
            index--;
            newOffset--;
        }
        else if (c == '\t')
        { // Tab pressed
            printf(CLEAR_LINE);
            handleTabPress(line, &index, &newOffset);
        }
        else
        {
            printf(CLEAR_LINE);
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
    return 0;
}

void executeCommand(char *line)
{
    printf("%s\n", line);
    char temp[strlen(line) + 1];
    strcpy(temp, line);
    free(line);
    printf("\033[31mExecute? [y or enter]\033[0m\n");

    char c;
    scanf("%c", &c);
    c = toupper(c);

    if (c == 'Y' || c == '\n')
    {
        if (c == 'Y')
        {
            printf("\n");
        }
        system(temp);
    }
}

char *editCommand(CommandInfo commandInfo)
{
    char *line = (char *)malloc(100 * sizeof(char));
    if (line == NULL)
    {
        perror("malloc");
        return NULL;
    }
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
        int result = editLineAtIndex(commandInfo.editIndexes[i], line, &offset);
        if (result == -1)
        {
            printf(CLEAR_LINE);
            resetTerminalMode();
            printf("%s\n", line);
            return NULL;
        }
    }
    printf(CLEAR_LINE);
    resetTerminalMode();

    return line;
}

char *getCommand(CommandInfo *commands, int size)
{
    enableNonCanonicalMode();
    printf(HIDE_CURSOR);
    int selectedLine = 0;
    char input[4];
    while (1)
    {
        printCommands(commands, size, selectedLine);
        memset(input, 0, sizeof(input));

        if (read(STDIN_FILENO, input, sizeof(input)) == -1)
        {
            perror("read");
            resetTerminalMode();
            return NULL;
        }

        if (input[0] == '\n')
        {
            break;
        }
        if (input[0] == '\033' && input[1] == '\0')
        {
            // Only escape key pressed
            printf("\033[%dA", size);          // move cursor up x lines
            printCommands(commands, size, -1); // -1 to not print >
            resetTerminalMode();
            return NULL;
        }
        printf("\033[%dA", size); // move cursor up x lines
        if (input[0] == '\t')
        {
            selectedLine = (selectedLine + 1) % size;
        }
        if (input[0] == '\033' && input[1] == '[')
        {
            if (input[2] == 'B')
            {
                // Down key pressed
                selectedLine = (selectedLine + 1) % size;
            }
            else if (input[2] == 'A' || input[2] == 'Z')
            {
                // Up or shift+tab key pressed
                selectedLine = (selectedLine - 1 + size) % size;
            }
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

    char *edited = editCommand(commands[selectedLine]);
    return edited;
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
