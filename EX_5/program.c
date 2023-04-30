// Remember to have permissions to run this application.
// Try running with sudo og change permissions with chmod.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define SERVER_FL_LISTEN_ARGV_NR 1
#define SERVER_FL_PORT_ARGV_NR 2
#define SERVER_PORT_ARGV_NR 3

#define CLIENT_HOST_ARGV_NR 1
#define CLIENT_PORT_ARGV_NR 3

#define SERVER_MODE 1
#define CLIENT_MODE 2

#define MAX_COMMAND_LENGTH 100
#define MAX_CLIENT_RESPOSE 4092

struct command_header {
    int command_length;
};

typedef struct client_command {
    struct command_header header;
    char command_string[MAX_COMMAND_LENGTH];
} client_command;

int create_server_socket(const char *host, int port);
bool read_string(char *str, int maxLength);
client_command create_command(char* command_str);
void send_command_to_client(int client_socket, char *terminal_command);
bool receive_command(int server_socket, client_command *cmd);
int create_client_socket(const char *server_host, int server_port);

int create_server_socket(const char *host, int port) {
    printf("Creating server at: %s:%d\r\n", host, port);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    int client_socket;

    if(server_socket < 0) {
        perror("Error opening server socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in socket_addr = {0};
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons(port);
    socket_addr.sin_addr.s_addr = inet_addr(host);

    // Bind the server socket to the address.
    if(bind(server_socket, (struct sockaddr *) 
        &socket_addr, sizeof(socket_addr)) < 0) {
        perror("Error binding");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming, but only one client 
    // connection allowed.
    if(listen(server_socket, 1) < 0) {
        perror("Fail to listen");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    if((client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_addr_len)) < 0) {
        perror("Error accepting client");
        exit(EXIT_FAILURE);
    }
    
    char *terminal_cmd = malloc(sizeof(char) * MAX_COMMAND_LENGTH);
    bool still_exec = true;

    do{
        printf("Enter command to execute on the client: ");
        still_exec = read_string(terminal_cmd, MAX_COMMAND_LENGTH);
        printf("Sending command: \n\"%s\"\n to client.\r\n", terminal_cmd);
        send_command_to_client(client_socket, terminal_cmd);
    } while(still_exec);

    free(terminal_cmd);

    // send(client_socket, "Hello", 5, 0);
    printf("Hello sent to the client\r\n");
    if(close(client_socket) < 0) {
        perror("Error closing client connection");
    }

    if(close(server_socket) < 0) {
        perror("Error closing server");
        exit(EXIT_FAILURE);
    }

    return server_socket;
}

bool read_string(char *str, int maxLength) {
    int current, counter = 0;

    while(counter < maxLength && (current = getchar()) != '\n') {
        if(current == EOF)
            return false;

        str[counter] = current;
        counter++;
    }

    str[counter] = '\0';
    return true;
}

client_command create_command(char* command_str) {
    client_command cmd;
    cmd.header.command_length = strlen(command_str);
    strncpy(cmd.command_string, command_str, cmd.header.command_length);
    return cmd;
}

void send_command_to_client(int client_socket, char *terminal_command) {
    client_command cmd = create_command(terminal_command);
    send(client_socket, &cmd.header, sizeof(cmd.header), 0);
    send(client_socket, cmd.command_string, cmd.header.command_length, 0);
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

    return true;
}

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

    // While loop pings the server to check if its still open.
    while(send(client_socket, NULL, 0, 0) != -1) {
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

int main(int argc, char *argv[]) {
    int mode, server_socket, client_socket;

    if(argc == 4) {
        if(strcmp(argv[1], "-listen") == 0) {
            printf("Starting server.\r\n");
            char *port = argv[SERVER_PORT_ARGV_NR];
            char *listen_flag = argv[SERVER_FL_LISTEN_ARGV_NR];
            char *port_flag = argv[SERVER_FL_PORT_ARGV_NR];

            if(strcmp(listen_flag, "-listen") != 0) {
                printf("Listen flag not specified.\r\n");
                exit(EXIT_FAILURE);
            }

            if(strcmp(port_flag, "-port") != 0) {
                printf("Port flag not specified.\r\n");
                exit(EXIT_FAILURE);
            }

            if(port == NULL || strlen(port) == 0) {
                printf("No port provided\r\n");
                exit(EXIT_FAILURE);
            }

            mode = SERVER_MODE;
            server_socket = create_server_socket("127.0.0.1", atoi(port));
        } else {
            printf("Starting client.\r\n");

            char *host = argv[CLIENT_HOST_ARGV_NR];
            char *port = argv[CLIENT_PORT_ARGV_NR];

            if(host == NULL || strlen(host) == 0) {
                printf("No host provided\r\n");
                exit(EXIT_FAILURE);
            }

            if(port == NULL || strlen(port) == 0) {
                printf("No port provided\r\n");
                exit(EXIT_FAILURE);
            }

            client_socket = create_client_socket(host, atoi(port));
            mode = CLIENT_MODE;
        }
    } else {
        printf("Too few/many arguments.\r\n");
        exit(EXIT_FAILURE);
    }    

    return 0;
}