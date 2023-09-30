#include "../include/routines_private.h"

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <pthread.h>
#include <netinet/in.h>
#include <unistd.h>

static int create_worker(const unsigned int clt_sock);

int
listen_routine(const unsigned int lst_port)
{
    int clt_sock;
    struct sockaddr_in addr;
    int lst_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(lst_socket < 0){
        perror("listen socket socket() error:");
        return -1;
    }

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = lst_port;
    addr.sin_addr.s_addr = INADDR_ANY;

    if( bind(lst_socket, &addr, sizeof(addr)) != 0){
        perror("bind(lst_socket) error");
        return -1;
    }
    
    if( listen(lst_socket, 1) != 0){
        perror("listen(lst_socket) error");
        return -1;
    }

    while(1){
        if( (clt_sock = accept(lst_socket, NULL, NULL)) == -1){
            perror("accept() err");
            continue;
        }

        if( create_worker(clt_sock) != 0){
            perror("create_worker() err");
            continue;
        }
    }
}

int 
create_worker(const unsigned int clt_sock)
{
    pthread_t tid;
    pthread_attr_t attr;

    if( pthread_attr_init(&attr) != 0 ){
        perror("pthread_attr_init() err");
        return -1;
    }
    if( pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0 ){
        perror("pthread_atte_setdetachstate() err");
        return -1;
    }

    if( pthread_create(&tid, &attr, serving_routine, &socket) != 0 ){
        perror("pthread_create() error");
        return -1;
    }

    return 0;
}