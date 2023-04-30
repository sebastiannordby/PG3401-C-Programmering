#ifndef __REVERSE_SHELL_H__
#define __REVERSE_SHELL_H__

#define MAX_COMMAND_LENGTH 100
#define MAX_CLIENT_RESPOSE 4092

typedef struct client_command_header {
    int command_length;
} client_command_header;

typedef struct client_command {
    client_command_header header;
    char command_string[MAX_COMMAND_LENGTH];
} client_command;

typedef struct client_response_header {
    int response_length;
} client_response_header;

typedef struct client_response {
    client_response_header header;
    char response[MAX_CLIENT_RESPOSE];
} client_response;
#endif