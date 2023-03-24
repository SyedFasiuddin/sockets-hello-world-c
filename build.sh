[ -e /tmp/socket-c-hello.socket ] && rm /tmp/socket-c-hello.socket

cc -o build/main src/main.c
