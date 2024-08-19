#ifndef CMDPRINTER_H
#define CMDPRINTER_H

void printCommands(CommandInfo *commands, int size, int selectedCommand);
void printCommandsStepRight(CommandInfo *commands, int size, int selectedCommand);
void printCommandsStepLeft(CommandInfo *commands, int size, int selectedCommand);

#endif