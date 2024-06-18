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
void printCommands(CommandInfo *commands, int size, int selectedCommand);
void insertString(char *line, int index, char *strToInsert);
int editLineAtIndex(int index, char *line, int *offset);
void executeCommand(char *line);
char *editCommand(CommandInfo commandInfo);
char *getCommand(CommandInfo *commands, int size);
void printOptions(const char *options[], int selectedOption, int maxOptions);

#endif