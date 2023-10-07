#include "../../protocol.h"
#include "routines_private.h"
#include "../../sendrecv/sendrecv.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int 
meta(task_t* task)
{
    struct ftcontext *ftcontext = &task->file_transfer_context;

    if( recv_uint64(ftcontext->socket, &ftcontext->file_full_size, 0) != 0 ){
        perror("Getting file size error");
        return -1;
    }

    if( recv_uints(ftcontext->socket, &ftcontext->filename_length, 0) != 0 ){
        perror("Getting filename length error");
        return -1;
    }

    ftcontext->filename = (char*)malloc(ftcontext->filename_length);
    if( recv_str(ftcontext->socket, ftcontext->filename, ftcontext->filename_length, 0) != 0 ) {
        perror("Getting filename error");
        return -1;
    }

    if( create_file(task, ftcontext->filename) != 0){
        perror("create_file() error");
        return -1;
    }

    printf("<<< SRV got file <%s>(%d filename lenght) size of %ld\n", ftcontext->filename, ftcontext->filename_length, ftcontext->file_full_size);

    return 0;
}

int 
file(task_t* task)
{
    if( recv_file(task, 0) != 0){
        perror("recv_file() error");
        return -1;
    }

    return 0;
}

int 
consist(task_t* task)
{
    if( task->file_transfer_context.file_curr_size != task->file_transfer_context.file_full_size ){
        task->succeed = 1;
        printf("File <%s> is corrupted. Deleting..\n", task->file_transfer_context.filename);
    }

    return 0;
}

int 
finish(task_t* task)
{
    if( send_uints(task->file_transfer_context.socket, &task->succeed, 0) != 0){
        perror("send_uints() error (file download status)");
    }

    printf("File <%s> transfer finished.\n", task->file_transfer_context.filename);
    free_task(task);
    return 0;
}

void 
delete_file(const char *filename)
{
    if(unlink(filename) != 0){
        perror("unlink() error");
    }
} 