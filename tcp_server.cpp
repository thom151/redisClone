#include <asm-generic/socket.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

void die(std::string functionName);
void do_something(int connfd);

int main() {

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    
    /*
        val is like an on and off switch

        setsockopt parameters:
            fd -> to let the function know which socket
            SOL_SOCKET -> applying the option to all socket not UPD or TCP only
            SO_RESUEADDR -> allowing the use of a recently closed ADDR bypassing the TIME_WAIT time 
    */
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));



    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = htonl(0);

    int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));

    if (rv) {
        die("bind()");
    }


    rv = listen(fd, SOMAXCONN);
    if (rv) { die("listen()"); }
   

    while (true) {

        //somthing to store client 
        struct sockaddr_in client_addr = {};
        socklen_t addrlen = sizeof(client_addr);

        //get the fd of the connection between you and the client
        int connfd = accept(fd, (struct sockaddr *)&client_addr,&addrlen);

        if (connfd < 0) {
            continue;
        }

        do_something(connfd);
        close(connfd);

        
    }

    return 0;
}

void do_something(int connfd) {
    char rbuf[64] = {};
    ssize_t n = read(connfd, &rbuf, sizeof(rbuf));

    if (n < 0 ) {
        std::cout << "read() error\n";
        return;
    }

    std::cout <<"client says: " <<  rbuf << "\n";


    char wbuf[] = "world";

    write(connfd, wbuf, strlen(wbuf));
}



