#include <asm-generic/socket.h>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "helpers.cpp"

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr={};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);


    int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv) {
        die("connect()");
    }

    char msg[] = "hello";
    write(fd, msg, sizeof(msg));

    char rbuf[64] = {};
    ssize_t n = read(fd, rbuf, sizeof(rbuf));

    if (n < 0) {
        die("read");
    }

    std::cout << "server says: " << rbuf <<"\n";
    close(fd);

}
