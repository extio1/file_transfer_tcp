#include "sendrecv.h"
#include "../hnconvert.h"

#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define BUFFER_SIZE 4096
#define TIMEWRITE_TIMEOUT 3

static int recvall(int fd, void* buf, size_t n, int flags);

int
recv_file(task_t* task, int flags)
{
    struct ftcontext* ftcontext = &task->file_transfer_context;
    char* buffer = (char*) malloc(BUFFER_SIZE);
    int red = -1;
    time_t prevtime = time(NULL);
    time_t nowtime;
    time_t deltatime;

    while(red != 0){
        int size = ftcontext->file_full_size - ftcontext->file_curr_size;
        size = (size > BUFFER_SIZE) ? BUFFER_SIZE : size;
        if( (red = recvall(ftcontext->socket, buffer, size, 0)) == -1 ){
            perror("recvall() error");
            return -1;
        }

        ftcontext->file_curr_size += red;

        if( (deltatime = ((nowtime = time(NULL)) - prevtime)) >= TIMEWRITE_TIMEOUT ){
            time_t instant_time = (ftcontext->file_curr_size-ftcontext->file_prev_size)/deltatime;
            ftcontext->avg_speed = ((ftcontext->avg_speed * ftcontext->n_delta) + instant_time) / (ftcontext->n_delta + 1);
            printf("<%s> instantaneous speed %ld byte/sec (%ld byte/sec avg.)\n", 
                ftcontext->filename, instant_time, ftcontext->avg_speed);
            prevtime = nowtime;
            ftcontext->file_prev_size = ftcontext->file_curr_size;
            ++ftcontext->n_delta;
        }

        if( write(ftcontext->fd, buffer, red) == -1 ){
            perror("write() error");
            return -1;
        }
    }

    free(buffer);
    return 0;
}

int
recv_str(int fd, char* buf, size_t n, int flags)
{
    if( recvall(fd, buf, n, flags) == -1 ){
        perror("recvall() error");
        return -1;
    }

    return 0;
}

int
recv_uint64(int fd, uint64_t* num, int flags)
{

    if( recvall(fd, num, sizeof(uint64_t), flags) == -1 ){
        perror("recvall() error");
        return -1;
    }
    *num = ntohll(*num); 

    return 0;
}

int
recv_uints(int fd, unsigned short* num, int flags)
{
    if( recvall(fd, num, sizeof(unsigned short), flags) == -1 ){
        perror("recvall() error");
        return -1;
    }

    *num = ntohs(*num); 

    return 0;
}

int
recvall(int fd, void* buf, size_t n, int flags)
{
    size_t counter = 0;
    int recved = -1;

    while (counter != n && recved != 0){
        if((recved = recv(fd, buf + counter, n, flags)) == -1){
            perror("recv() error");
            return -1;
        }
        counter += recved;
    }

    return counter;
}
