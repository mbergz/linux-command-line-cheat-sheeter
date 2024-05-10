#include <stdio.h>
#include "csfind.h"
#include <unistd.h>
#include <termios.h>

#define MAX_OPTIONS 3

#define STD_OUT_REVERSE "\x1b[7m"
#define STD_OUT_RESET "\x1b[0m"
#define HIDE_CURSOR "\e[?25l"
#define SHOW_CURSOR "\e[?25h"

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
            printf("%s%s%s ", STD_OUT_REVERSE, options[i], STD_OUT_RESET); // Print highlighted option
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
    printf(HIDE_CURSOR);

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
        else if (c == '\n')
        {
            // Exit loop on Enter key
            break;
        }
    }

    printf(SHOW_CURSOR);
    resetTerminalMode(&old);

    printf("\nSelected option: %s\n", options[selectedOption]);
}
