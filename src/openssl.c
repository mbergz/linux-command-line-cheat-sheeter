#include <stdlib.h>
#include "common.h"
#include "openssl.h"
#include "filewriter.h"
#include "malloc_manager.h"

static CommandInfo opensslCommands[] = {
    {"openssl x509 -in mycert.pem -text -noout", "View x509 certificate", {28}},
    {"openssl s_client -showcerts -connect www.example.com:443 </dev/null", "View x509 cert from website", {52}},
    {"openssl s_client -showcerts -servername www.example.com -connect www.example.com:443 </dev/null", "View x509 cert from website with SNI", {}},
    {"openssl req -in file.csr -text -noout", "View CSR", {24}},
    {"openssl genrsa -out my_private_key.key 2048", "Generate private key", {}},
    {"openssl rsa -in my_private_key.key -pubout > my_public_key.pub", "Generate public key with private key", {}},
    {"openssl req -x509 -new -key my_private_key.key -days 365 -out mycert.pem", "Generate self-signed certificate", {}},
};

void opensslCheatSheet()
{
    char *command = getCommand(opensslCommands, sizeof(opensslCommands) / sizeof(opensslCommands[0]));
    if (command != NULL)
    {
        writeToTmpFile(command);
        freePointer(command);
    }
}
