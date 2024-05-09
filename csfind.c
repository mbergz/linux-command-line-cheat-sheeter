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


void printOptions(const char *options[], int selectedOption) {
    printf("\r\033[2K"); // Clear entire line

    // Print options with current selection highlighted
    for (int i = 0; i < MAX_OPTIONS; ++i) {
        if (i == selectedOption) {
            printf("\x1b[7m*%s\x1b[0m ", options[i]); // Print highlighted option
        } else {
            printf("%s ", options[i]); // Print regular option
        }
    }

    fflush(stdout); // Flush output buffer to ensure it's printed immediately
}

void printFindCheatSheet()
{
    const char *options[MAX_OPTIONS] = {"all", "file", "dir"};
    int selectedOption = 0;
    char c;

    // Set terminal into non-canonical mode
    struct termios old;
    struct termios new;
    tcgetattr(0, &old);
    new = old;
    new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &new);

    printf("find\n");

    while (1) {
        printOptions(options, selectedOption);

        // Read a single character
        if (read(STDIN_FILENO, &c, 1) == -1) {
            perror("read");
            //exit(1);
        }

        // Process user input
        if (c == '\t') {
            selectedOption = (selectedOption + 1) % MAX_OPTIONS; // Move to next option
            printOptions(options, selectedOption);
        } else if (c == 'a') {
            printf("The 'a' key was pressed.\n");
        } else if (c == '\n') {
            // Exit loop on Enter key
            break;
        }
    }

    // Reset terminal
    tcsetattr(0, TCSANOW, &old);
    printf("\nSelected option: %s\n", options[selectedOption]);

}
