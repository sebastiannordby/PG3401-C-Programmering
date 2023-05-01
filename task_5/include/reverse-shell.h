#ifndef __REVERSE_SHELL_H__
#define __REVERSE_SHELL_H__

#define MAX_COMMAND_LENGTH 100
#define MAX_CLIENT_RESPOSE 4092
#define PING_MESSAGE "PING"
#define PONG_MESSAGE "PONG"

typedef struct client_command {
    int content_length;
    char content_body[MAX_COMMAND_LENGTH];
} client_command;

typedef struct client_response {
    int content_length;
    char content_body[MAX_CLIENT_RESPOSE];
} client_response;
#endif