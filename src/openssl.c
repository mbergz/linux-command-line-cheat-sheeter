#include <stdlib.h>
#include "common.h"
#include "openssl.h"
#include "filewriter.h"
#include "malloc_manager.h"

static CommandInfo opensslCommands[] = {
    {"openssl x509 -in mycert.pem -text -noout", "View x509 certificate", {28}},
    {"openssl s_client -showcerts -connect www.example.com:443 </dev/null", "View x509 cert from website", {52}},
    {"openssl s_client -showcerts -connect www.example.com:443 -servername www.example.com </dev/null", "View x509 cert from website with SNI", {52, 84}},
    {"openssl s_client -showcerts -connect google.com:443 2>&1 < /dev/null | openssl x509 > cert.pem", "Download x509 certificate from website", {47, 94}},
    {"openssl s_client -showcerts -connect test.com:443 2>&1 < /dev/null | sed -n '/-----BEGIN/,/-----END/p' > chain.pem", "Download chain of x509 cert from website", {46, 114}},
    {"openssl req -in file.csr -text -noout", "View CSR", {24}},
    {"openssl genrsa -out my_private_key.key 2048", "Generate private key", {34, 43}},
    {"openssl rsa -in my_private_key.key -pubout > my_public_key.pub", "Generate public key with private key", {34, 62}},
    {"openssl req -x509 -new -key my_private_key.key -days 365 -out mycert.pem", "Generate self-signed certificate", {46, 56, 72}},
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
