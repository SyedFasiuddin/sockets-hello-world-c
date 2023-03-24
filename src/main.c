#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/socket-c-hello.socket"

void log_err_die(const char * const s) {
    fprintf(stderr, "Errno: %d\n", errno);
    perror(s);
    exit(EXIT_FAILURE);
}

void echo_server(void) {
    int server_fd;
    struct sockaddr_un server_addr = {
        .sun_family = AF_UNIX,
    };
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    if((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
        log_err_die("Socket creation");

    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        log_err_die("Bind");

    if(listen(server_fd, 1) == -1)
        log_err_die("Listen");

    int client_fd;
    struct sockaddr_un client_addr;
    socklen_t s = sizeof(client_addr);
    if((client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &s)) == -1)
        log_err_die("Accept");

#define RECV_BUFFER_LEN 100
    char buffer[RECV_BUFFER_LEN];
    int len = recv(client_fd, buffer, RECV_BUFFER_LEN, 0);

    printf("Recieved %d bytes\nMsg: %s\n", len, buffer);

    len = send(client_fd, buffer, len, 0);
    printf("Sent %d bytes\n", len);
}

void echo_client(void) {
    int client_fd;
    struct sockaddr_un server_addr = {
        .sun_family = AF_UNIX,
    };
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    if((client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
        log_err_die("Socket creation");

    int server_fd;
    if((server_fd = connect(client_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un))) == -1)
        log_err_die("connect");

#define BUFFER_LEN 100
    char input[BUFFER_LEN];
    scanf("%s", input);

    int len = write(client_fd, input, BUFFER_LEN);
    printf("sent %d bytes\n", len);

    char buffer[BUFFER_LEN];
    len = recv(client_fd, buffer, BUFFER_LEN, 0);
    printf("Recieved %d bytes\nMsg: %s", len, buffer);
}

int main(int argc, char **argv) {
    echo_server();
    return 0;
}
