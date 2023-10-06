#include "../include/clt_context.h"

#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>

// СОКЕТ ДЛЯ ПОДКЛЮЧЕНИЯ К СЕРВЕРУ: НУЖНО ЛИ ЕГО БИНДИТЬ К ЛОКАЛХОСТ:ЭНИ_АДДР и 
// НОРМАЛЬНО ЛИ ЧТО ОН СОЗДАЁТСЯ AF_INET (как быть с ipv6)?

int 
create_context(const char* filename, const char* servername, const char* port_str, connection_context_t* context)
{
    struct sockaddr_in local_addr;
    struct addrinfo *addr_server;
    struct addrinfo hints;
    struct stat file_stat;
    int sock;

    bzero(context, sizeof(connection_context_t));

    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_NUMERICSERV;
    if ( getaddrinfo(servername, port_str, &hints, &addr_server) != 0 ){
        perror("getaddrinfo() error");
        return -1;
    }

    if( (sock = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
        perror("socket() error");
        return -1;
    }
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = 0;
    if( bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) != 0){
        perror("bind() error");
        return -1;
    }

    struct addrinfo *addr = addr_server;
    while(addr != NULL){
        if(connect(sock, addr->ai_addr, addr->ai_addrlen) == 0){
            struct sockaddr_in* conaddr = (struct sockaddr_in*)(addr->ai_addr);
            printf("connected to %s:%d\n", inet_ntoa(conaddr->sin_addr), conaddr->sin_port);
            break;
        } else {
            perror("connect() error");
        }
        addr = addr->ai_next;
    }

    context->socket = sock;
    context->filename_length = strlen(filename);
    context->filename = malloc(context->filename_length);
    strcpy(context->filename, filename);

    if( (context->filefd = open(filename, O_RDONLY)) == -1){
        perror("fopen() error");
        return -1;
    }

    if( fstat(context->filefd, &file_stat) != 0){
        perror("stat() error");
        return -1;
    }

    context->file_size = file_stat.st_size;
    context->initialized = true;

    return 0;
}

int
free_context(connection_context_t* context)
{
    if(context->initialized){
        close(context->filefd);
        close(context->socket);
        context->initialized = false;
    } else {
        return EINVAL;
    }

    return 0;
}