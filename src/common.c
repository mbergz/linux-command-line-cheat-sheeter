#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <termios.h>
#include <signal.h>
#include "common.h"
#include "cmdprinter.h"
#include <readline/readline.h>
#include <readline/history.h>

#define STD_OUT_REVERSE "\x1b[7m"
#define STD_OUT_RESET "\x1b[0m"
#define HIDE_CURSOR "\e[?25l"
#define SHOW_CURSOR "\e[?25h"
#define CLEAR_LINE "\r\033[2K"

#define ANSI_COLOR_LIGHT_GRAY "\033[90m"
#define ANSI_COLOR_DARK_YELLOW "\033[33m"
#define ANSI_COLOR_RESET "\033[0m"

#define DEFAULT_PADDING 50
#define MAX_PADDING 100

static struct termios old;

static int readlineActive = 0;

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

static void lineHandler(char *line)
{
    if (line == NULL)
    {
        printf("\n");
        rl_callback_handler_remove();
    }
    else
    {
        readlineActive = 0;
        free(line);
    }
}

static void cleanupReadline()
{
    rl_clear_history();
    rl_deprep_terminal();
    rl_cleanup_after_signal();
    rl_free_line_state();
    rl_reset_line_state();
}

static void feedLineToReadline(const char *line)
{
    rl_replace_line(line, 0);
    rl_redisplay();
}

static int editLineAtIndex(int index, char **line, int *offset)
{
    index = index + *offset;
    printf("\r");
    printf("\033[%dC", index); // move cursor to starting pos

    rl_callback_handler_install("", lineHandler);
    feedLineToReadline(*line);

    // Set readline pointer to index
    rl_point = index;

    readlineActive = 1;

    char *lineCopy = malloc(100);
    if (lineCopy == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    strcpy(lineCopy, *line);

    while (readlineActive)
    {
        rl_callback_read_char();
        if (rl_line_buffer[0] != '\0')
        {
            printf(CLEAR_LINE);
            strcpy(lineCopy, rl_line_buffer);

            printf("%s", lineCopy);

            printf("\r");
            printf("\033[%dC", rl_point);
            fflush(stdout);
        }
    }
    rl_callback_handler_remove();
    cleanupReadline();

    printf("%s", lineCopy);

    *offset += (strlen(lineCopy) - strlen(*line));
    free(*line);
    *line = lineCopy;
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
        // system(temp);
    }
}

static char *editCommand(CommandInfo commandInfo)
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
        int result = editLineAtIndex(commandInfo.editIndexes[i], &line, &offset);
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
