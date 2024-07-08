#include <stdio.h>
#include <stdlib.h>

#define TMP_COMMAND_OUTPUT_FILE_PATH "/tmp/cs_command_output.txt"

void writeToTmpFile(char *commandLine)
{
    FILE *fp;
    fp = fopen(TMP_COMMAND_OUTPUT_FILE_PATH, "w+");

    if (fp != NULL)
    {
        fputs(commandLine, fp);
        fclose(fp);
    }
}