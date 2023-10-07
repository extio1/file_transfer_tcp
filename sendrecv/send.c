#include "sendrecv.h"
#include "../../hnconvert.h"

#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

static int sendall(int fd, void* buf, size_t n, int flags);

int
send_file(int sockfd, int fd, int flags)
{
    char* buffer = malloc(BUFFER_SIZE);
    int red = -1;

    while(red != 0){
        if( (red = read(fd, buffer, BUFFER_SIZE)) == -1 ){
            perror("read() error");
            return -1;
        }

        if( sendall(sockfd, buffer, red, flags) == -1 ){
            perror("sendall() error");
            return -1;
        }
    }

    free(buffer);
    return 0;
}

int
send_str(int fd, char* buf, size_t n, int flags)
{
    if( sendall(fd, buf, n, flags) == -1 ){
        perror("sendall() error");
        return -1;
    }

    return 0;
}

int
send_uint64(int fd, uint64_t* num, int flags)
{
    uint64_t netnum = htonll(*num);
    if( sendall(fd, &netnum, sizeof(uint64_t), flags) == -1 ){
        perror("sendall() error");
        return -1;
    }

    return 0;
}

int
send_uints(int fd, unsigned short* num, int flags)
{
    unsigned short netnum = htons(*num);
    if( sendall(fd, &netnum, sizeof(unsigned short), flags) == -1 ){
        perror("sendall() error");
        return -1;
    }

    return 0;
}

int 
sendall(int fd, void* buf, size_t n, int flags)
{
    int counter = 0;
    int sent = 0;

    while (counter != n){
        usleep(100000000 /  (1 + rand()%200000));
        if((sent = send(fd, buf + counter, n, flags)) == -1){
            return -1;
        }
        counter += sent;
    }

    return counter;
}
