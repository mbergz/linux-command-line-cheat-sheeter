#include <stdlib.h>
#include "common.h"
#include "curl.h"
#include "filewriter.h"

static CommandInfo curlCommands[] = {
    {"curl https://example.com", "Basic GET request", {24}},
    {"curl -v http://example.com", "Basic verbose GET request", {26}},
    {"curl -o file.txt https://example.com", "Save the result to a file called file.txt", {36}},
    {"curl -O https://cdn.jsdelivr.net/npm/vue/dist/vue.js", "Save the result to a file with original filename", {52}},
    {"curl -I --http2 https://www.ubuntu.com/", "Fetch only headers", {39}},
    {"curl -X POST https://example.com -d \"param1=value1&param2=value2\"", "POST with default data application/x-www-form-urlencoded", {12, 32, 64}},
    {"curl -X POST https://example.com -H \"Content-Type: application/json\" -d @test.json", "POST with json body from file", {12, 32, 67, 82}},
    {"curl --cert client.crt --key client.key --cacert ca.crt https://myserver.internal.net:443", "POST with mTLS", {22, 39, 55, 89}}};

void curlCheatSheet()
{
    char *command = getCommand(curlCommands, sizeof(curlCommands) / sizeof(curlCommands[0]));
    if (command != NULL)
    {
        writeToTmpFile(command);
        free(command);
    }
}
