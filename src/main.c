#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>

#define SOCKET_PATH "/tmp/socket-c-hello.socket"

void log_err_die(const char * const s) {
    fprintf(stderr, "Errno: %d\n", errno);
    perror(s);
    exit(EXIT_FAILURE);
}

void socket_file(void) {
    int fd;
    struct sockaddr_un sock_addr = {
        .sun_family = AF_UNIX,
    };
    strncpy(sock_addr.sun_path, SOCKET_PATH, sizeof(sock_addr.sun_path) - 1);

    if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
        log_err_die("Socket creation");

    if(bind(fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) == -1)
        log_err_die("Bind");

    if(listen(fd, 1) == -1)
        log_err_die("Listen");

    struct sockaddr_un client_addr;
    socklen_t s = sizeof(client_addr);
    if(accept(fd, (struct sockaddr *) &client_addr, &s) == -1)
        log_err_die("Accept");

    printf("Path: %s\nFamily: %hhu\nLen: %d", client_addr.sun_path, client_addr.sun_family, client_addr.sun_len);
}

int main(int argc, char **argv) {
    socket_file();
    return 0;
}
