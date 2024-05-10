#include <stdio.h>
#include "csfind.h"
#include <unistd.h>
#include <termios.h>

#define MAX_OPTIONS 3
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

void printAll();
void printFile();
void printDir();

void printAll()
{
    printf("printing all options");
}
void printFile()
{
    printf("printing file");
}
void printDir()
{
    printf("printing dir");
}

void printOptions(const char *options[], int selectedOption)
{
    printf("\r\033[2K"); // Clear entire line

    // Print options with current selection highlighted
    for (int i = 0; i < MAX_OPTIONS; ++i)
    {
        if (i == selectedOption)
        {
            printf("\x1b[7m*%s\x1b[0m ", options[i]); // Print highlighted option
        }
        else
        {
            printf("%s ", options[i]); // Print regular option
        }
    }

    fflush(stdout); // Flush output buffer to ensure it's printed immediately
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

    const char *options[MAX_OPTIONS] = {"all", "file", "dir"};
    int selectedOption = 0;
    char c;

    enableNonCanonicalMode(&old);

    while (1)
    {
        printOptions(options, selectedOption);

        // Read a single character
        if (read(STDIN_FILENO, &c, 1) == -1)
        {
            perror("read");
            // exit(1);
        }

        // Process user input
        if (c == '\t')
        {
            selectedOption = (selectedOption + 1) % MAX_OPTIONS; // Move to next option
            printOptions(options, selectedOption);
        }
        else if (c == 'a')
        {
            printf("The 'a' key was pressed.\n");
        }
        else if (c == '\n')
        {
            // Exit loop on Enter key
            break;
        }
    }

    resetTerminalMode(&old);
    printf("\nSelected option: %s\n", options[selectedOption]);
}
