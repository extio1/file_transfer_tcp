#include "../include/task.h"
#include "../include/protocol.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024

static uint64_t ntohll(uint64_t); 

static 
uint64_t 
ntohll(uint64_t a)
{
    uint32_t left = 0 + (a >> 32);
    uint32_t right = 0 + (a ^ ((uint64_t)left << 32));

    uint32_t conv_left  = ntohl(left);
    uint32_t conv_right = ntohl(right);

    if( conv_left != left || conv_right != right){
        return (conv_right << 32) | conv_left; 
    } else {
        return (conv_left << 32) | conv_right;
    }
}

void* 
serving_routine(void* args)
{
    bool finished = false;

    task_t task;
    init_task(&task, *((int*)args));

    // SO_RCVBUF option to setsockopt doesn't set for MSG_WAITALL reads

    char* buffer = malloc(BUFFER_SIZE);
    while(!finished){
        switch (task.task_state)
        {
        case META:
            if( recv(task.socket, task.file_full_size, SIZE_OF_FILE, MSG_WAITALL) != SIZE_OF_FILE){
                perror("Getting file size error");
                return;
            }
            task.file_full_size = ntohll(task.file_full_size);

            if( recv(task.socket, task.filename_length, LEN_OF_FILENAME, MSG_WAITALL) != SIZE_OF_FILE){
                perror("Getting filename length error");
                return;
            }
            task.filename_length = ntohs(task.filename_length);

            task.filename = malloc(task.filename_length);
            for(int i = 0; i < task.filename_length; ){
                int read_counter = 0;
                if( ( read_counter = recv(task.socket, task.filename[i], task.filename_length-i, 0)) == 0 ){
                    printf("EOF() reached\n");
                    return 0;
                }
                i += read_counter;
            }

            ++task.task_state;
            break;
        default:
            finished = true;
            break;
        }        
    }

    printf("File transfer finished\n");
    return 0;
}