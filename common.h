#ifndef COMMON_H
#define COMMON_H

struct CommandInfo
{
    const char *command;
    const char *description;
    int editIndexes[5];
};

typedef struct CommandInfo CommandInfo;

void storeCurrentTerminalMode();
void resetTerminalMode();
void handleSigint(int sig);
void enableNonCanonicalMode();
void executeCommand(char *line);
char *getCommand(CommandInfo *commands, int size);
void printOptions(const char *options[], int selectedOption, int maxOptions);

#endif