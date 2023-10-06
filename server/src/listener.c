#include "../include/routines_private.h"
#include "../include/executor.h"

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <pthread.h>
#include <netinet/in.h>
#include <unistd.h>

// !!!!!!!!!!!!!!!!!!!!! ??????????????????????

// КАК НА ОДНОМ СОКЕТЕ ПРИНИМАТЬ И ipv6 и ipv4
// УТОЧНИТЬ ПРО BIND, мы биндим сокет к локальному интерфейсу? То есть
// если мы прибиндим сокет AF_INET на port и IPADDR_ANY, то он будет
// соединен с выбранным системой адресом интерфейса и с портом port
// но при этом принимать сообщения он может с любого адреса, в том числе 
// и с ipv6?

// !!!!!!!!!!!!!!!!!!!!! ??????????????????????

// ГДЕ ВЫЛЕЗАЕТ ВТОРОЙ ПАРАМЕТР listen? КОТОРЫЙ про размер очереди
// подключаемых клиентов.

int
listen_routine(const unsigned int lst_port)
{
    int clt_sock;
    struct sockaddr_in addr;
    struct sockaddr in_addr;
    socklen_t in_addr_socklen;

    int lst_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(lst_socket < 0){
        perror("listen socket socket() error:");
        return -1;
    }

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = lst_port;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //INADDR_ANY;

    if( bind(lst_socket, (struct sockaddr *)&addr, sizeof(addr)) != 0){
        perror("bind(lst_socket) error");
        return -1;
    }
    
    if( listen(lst_socket, 5) != 0){
        perror("listen(lst_socket) error");
        return -1;
    }

    while(1){
        printf("Wait on %s:%d\n", inet_ntoa(addr.sin_addr), addr.sin_port);
        if( (clt_sock = accept(lst_socket, &in_addr, &in_addr_socklen)) == -1){
            perror("accept() err");
            continue;
        }

        if( execute(clt_sock, in_addr, in_addr_socklen) != 0){
            perror("create_worker() err");
            continue;
        }
    }
}