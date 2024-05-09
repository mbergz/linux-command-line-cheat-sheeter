#include <stdio.h>
#include <ncurses.h>
#include "csfind.h"

#define WINDOW_WIDTH 50
#define MAX_INPUT_LENGTH 20
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

void printOptions(int highlight, char *options[])
{
    int x, y, i;

    clear();
    x = 0;
    y = 0;

    for (i = 0; i < MAX_OPTIONS; ++i)
    {
        if (highlight == i + 1)
        {
            attron(A_REVERSE);
            mvprintw(y, x, "%s", options[i]);
            attroff(A_REVERSE);
        }
        else
        {
            mvprintw(y, x, "%s", options[i]);
        }
        ++y;
    }

    refresh();
}

void printFindCheatSheet()
{
    char userSelection[MAX_INPUT_LENGTH];
    char *options[MAX_OPTIONS] = {"all", "file", "dir"};
    int selectedOption = 0;
    int highlight = 1;
    int choice = 0;
    int c;

    initscr();
    clear();
    noecho();
    cbreak(); // Line buffering disabled. pass on everything

    printOptions(highlight, options);

    while (1)
    {
        c = getch();
        switch (c)
        {
        case '\t': // Tab key
            if (highlight == MAX_OPTIONS)
                highlight = 1;
            else
                ++highlight;
            break;
        case KEY_BTAB: // Shift+Tab
            if (highlight == 1)
                highlight = MAX_OPTIONS;
            else
                --highlight;
            break;
        case 10: // Enter key
            choice = highlight;
            break;
        default:
            refresh();
            break;
        }
        printOptions(highlight, options);
        if (choice != 0) // User made a choice; break out of loop
            break;
    }
    mvprintw(MAX_OPTIONS + 1, 0, "You chose option %d with choice string %s\n", choice, options[choice - 1]);
    getch();
    endwin();
    //return 0;
    /*
        for (int i = 0; i < MAX_OPTIONS; i++)
        {
            if (i == selectedOption)
            {
                printf(ANSI_COLOR_GREEN);
                printf("*");
            }
            printf("%s", options[i]);
            printf(ANSI_COLOR_RESET);
            printf(" ");
        }
        printf("\r");

        while (1)
        {

            int key = getchar();
            if (key == '\t') // Tab pressed
            {
                selectedOption = (selectedOption + 1) % MAX_OPTIONS;
                for (int i = 0; i < MAX_OPTIONS; i++)
                {
                    if (i == selectedOption)
                    {
                        printf(ANSI_COLOR_GREEN);
                        printf("*");
                    }
                    printf("%s", options[i]);
                    printf(ANSI_COLOR_RESET);
                    printf(" ");
                }
            }
            else if (key == '\n')
            {
                // Enter pressed
                break;
            }
            while (getchar() != '\n')
                ;
        }
        printf("Selected option: %s\n", options[selectedOption]);
    */
    // scanf("%s", userSelection);
    // printf("%s\n", userSelection);
}
