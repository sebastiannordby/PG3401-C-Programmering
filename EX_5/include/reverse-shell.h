#ifndef __REVERSE_SHELL_H__
#define __REVERSE_SHELL_H__

#define MAX_COMMAND_LENGTH 100
#define MAX_CLIENT_RESPOSE 4092

typedef struct command_header {
    int command_length;
} command_header;

typedef struct client_command {
    command_header header;
    char command_string[MAX_COMMAND_LENGTH];
} client_command;

#endif