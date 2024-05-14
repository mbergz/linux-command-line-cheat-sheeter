#ifndef COMMON_H
#define COMMON_H

struct CommandInfo
{
    const char *command;
    const char *description;
    int editIndexes[5];
};

typedef struct CommandInfo CommandInfo;

#endif