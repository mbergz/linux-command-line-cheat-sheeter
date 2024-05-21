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
void editLineAtIndex(int index, char *line, int *offset);
void editCommand(CommandInfo commandInfo);
void printSelectableCommands(CommandInfo *commands, int size);
void printOptions(const char *options[], int selectedOption, int maxOptions);

#endif