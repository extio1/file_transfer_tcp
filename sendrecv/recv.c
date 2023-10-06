#include "sendrecv.h"
#include "../hnconvert.h"

#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

int
recv_file(int sockfd, int fd, int flags)
{
    char* buffer = malloc(BUFFER_SIZE);
    int red = -1;

    while(red != 0){
        if( (red = recvall(sockfd, buffer, BUFFER_SIZE, 0)) == -1 ){
            perror("recvall() error");
            return -1;
        }

        if( write(fd, buffer, red) != 0 ){
            perror("sendall() error");
            return -1;
        }
    }

    return 0;
}

int
recv_str(int fd, char* buf, size_t n, int flags)
{
    if( recvall(fd, buf, n, flags) == -1 ){
        perror("sendall() error");
        return -1;
    }

    return 0;
}

int
recv_uint64(int fd, uint64_t* num, size_t n, int flags)
{

    if( recvall(fd, num, n, flags) == -1 ){
        perror("sendall() error");
        return -1;
    }
    *num = ntohll(*num); 

    return 0;
}

int
send_uints(int fd, unsigned short* num, size_t n, int flags)
{
    if( sendall(fd, num, n, flags) == -1 ){
        perror("sendall() error");
        return -1;
    }
    
    *num = ntohs(*num); 

    return 0;
}

int
recvall(int fd, void* buf, size_t n, int flags)
{
    int counter = 0;
    int sent = 0;

    while (counter != n){
        if((sent = recv(fd, buf + counter, n, flags)) == -1){
            return -1;
        }
        counter += sent;
    }

    return counter;
}
