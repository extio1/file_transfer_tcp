#include "sendrecv.h"
#include "../hnconvert.h"

#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define BUFFER_SIZE 4096
#define TIMEWRITE_TIMEOUT 3
#define TIMEOUT_MAX 3

static int recvall(int fd, void* buf, size_t n, int flags);

int
recv_file(task_t* task, int flags)
{
    struct ftcontext* ftcontext = &task->file_transfer_context;
    char* buffer = (char*) malloc(BUFFER_SIZE);
    int red = 1;
    int timeout_counter = 0;
    time_t prevtime = time(NULL);
    time_t nowtime;
    time_t deltatime;
    static struct timeval timeout = {.tv_sec = TIMEWRITE_TIMEOUT, .tv_usec = 0};
    static struct timeval blocking = {.tv_sec= 0, .tv_usec = 0};      

    if (setsockopt (ftcontext->socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0){
        perror("setsockopt() failed\n");
    }

    while(1){
        int size = ftcontext->file_full_size - ftcontext->file_curr_size;
        size = (size > BUFFER_SIZE) ? BUFFER_SIZE : size;

        if( (red = recv(ftcontext->socket, buffer, size, 0)) <= 0){
            if( red == 0 ){
                break;
            } else if (errno == EAGAIN) {
                if( timeout_counter >= TIMEOUT_MAX ){
                    break;
                }
                ++timeout_counter;
            } else {
                perror("recv() error");
                break;
            }
        } else {
            timeout_counter = 0;
            ftcontext->file_curr_size += red;
        }

        if( (deltatime = ((nowtime = time(NULL)) - prevtime)) >= TIMEWRITE_TIMEOUT ){
            time_t instant_time = (ftcontext->file_curr_size-ftcontext->file_prev_size)/deltatime;
            ftcontext->avg_speed = ((ftcontext->avg_speed * ftcontext->n_delta) + instant_time) / (ftcontext->n_delta + 1);
            printf("<%s> instantaneous speed %ld byte/sec (%ld byte/sec avg.)\n", 
                ftcontext->filename, instant_time, ftcontext->avg_speed);
            prevtime = nowtime;
            ftcontext->file_prev_size = ftcontext->file_curr_size;
            ++ftcontext->n_delta;
        }

        if(red > 0)
            if( write(ftcontext->fd, buffer, red) == -1 ){
                perror("write() error");
                break;
            }
    }

    if (setsockopt (ftcontext->socket, SOL_SOCKET, SO_RCVTIMEO, &blocking, sizeof(blocking)) < 0){
        perror("setsockopt() failed\n");
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
