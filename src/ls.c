#include <stdlib.h>
#include "common.h"
#include "filewriter.h"
#include "malloc_manager.h"

static CommandInfo lsCommands[] = {
    {"ls -a", "List files and dir including hidden (\"ll -a\")", {}},
    {"ls -d */", "List only dirs in current dir (\"ll -d */\")", {}},
    {"ls -1", "List files and dir simple format one per row", {}},
    {"ls -R", "List files and dirs recursively", {}},
    {"ls -l", "List files and dir with long format", {}},
    {"ls -lh", "Alias for \"ll\".List files and dir with long format in human-readable format", {}},
    {"ls -t", "Lists all files and dirs sorted by modification time (\"ll -t\")", {}},
    {"ls -tr", "Lists all files and dirs sorted by modification time in reversed order", {}},
};

void lsCheatSheet()
{
    char *command = getCommand(lsCommands, sizeof(lsCommands) / sizeof(lsCommands[0]));
    if (command != NULL)
    {
        writeToTmpFile(command);
        freePointer(command);
    }
}
