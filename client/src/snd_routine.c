#include "../include/snd_routine.h"
#include "../../sendrecv/sendrecv.h"
#include "../../protocol.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

// write же тоже может недозаписать массив байт???

int
cp_file_to(connection_context_t* context)
{
    if(sizeof(context->file_size) > SIZE_OF_FILE){
        printf("Too big file size.\n");
        return -1;
    }
    
    if(sizeof(context->filename_length) > LEN_OF_FILENAME){
        printf("Too filename\n");
        return -1;
    }

    // FILE SIZE
    if( send_uint64(context->socket, &context->file_size, SIZE_OF_FILE, 0) != 0 ){
        perror("send_uint64() error");
        return -1;
    }

    // FILENAME LENGTH
    if( send_uints(context->socket, &context->filename_length, LEN_OF_FILENAME, 0) != 0 ){
        perror("sendall() error");
        return -1;
    }

    // FILENAME
    if( send_str(context->socket, context->filename, context->filename_length, 0) != 0 ){
        perror("sendall() error");
        return -1;
    }

    // FILE
    if( send_file(context->socket, context->filefd, 0) != 0){
        perror("send_file() error");
        return -1;
    }

    return 0;
}
