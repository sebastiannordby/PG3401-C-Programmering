// Note:
// Remember to have permissions to run this application.
// Try running with sudo og change permissions with chmod.
// This application also just works on one client at the time
// becuase its single threaded.

#include "reverse-shell.h"
#include "server.h"
#include "client.h"
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

int start_server(char *argv[]) {
    printf("Starting server...\r\n");
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

    return create_server_socket("127.0.0.1", atoi(port));
}

int start_client(char *argv[]) {
    printf("Starting client...\r\n");

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

    return create_client_socket(host, atoi(port));
}

int main(int argc, char *argv[]) {
    int mode, server_socket, client_socket;

    if(argc != 4) {
        printf("Invalid number of arguments.\r\n");
        printf("Usage server: rev-shell -listen -port <port>\r\n");
        printf("Usage client: rev-shell -server <host> -port <port>\r\n");
    }

    if(strcmp(argv[1], "-listen") == 0) {
        server_socket = start_server(argv);
        mode = SERVER_MODE;
    } else {
        client_socket = start_client(argv);
        mode = CLIENT_MODE;
    }

    return 0;
}