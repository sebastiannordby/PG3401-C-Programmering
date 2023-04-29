// Remember to have permissions to run this application.
// Try running with sudo og change permissions with chmod.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SERVER_FL_LISTEN_ARGV_NR 1
#define SERVER_FL_PORT_ARGV_NR 2
#define SERVER_PORT_ARGV_NR 3

#define CLIENT_HOST_ARGV_NR 2
#define CLIENT_PORT_ARGV_NR 4

#define SERVER_MODE 1
#define CLIENT_MODE 2

int create_server_socket(const char *host, int port) {
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

    if((client_socket = accept(server_socket, 
        (struct sockaddr *) &socket_addr, (socklen_t *) sizeof(socket_addr))) < 0) {
        perror("Error accepting client");
        exit(EXIT_FAILURE);
    }

    send(client_socket, "Hello", 5, 0);
    printf("Hello sent to the client\r\n");

    return server_socket;

}


int create_client_socket(const char *server_host, int server_port) {
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
}


int main(int argc, char *argv[]) {
    int mode, server_socket, client_socket;

    if(argc == 4) {
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
    } else if(argc == 3) {
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

        mode = CLIENT_MODE;
    } else {
        printf("Too few/many arguments.\r\n");
        exit(EXIT_FAILURE);
    }    

    return 0;
}