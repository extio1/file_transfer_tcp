#include "../../protocol.h"
#include "routines_private.h"
#include "../../hnconvert.h"
#include "../../sendrecv/sendrecv.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

int 
meta(task_t* task)
{
    struct ftcontext ftcontext = task->file_transfer_context;
    if( recv(ftcontext.socket, &ftcontext.file_full_size, SIZE_OF_FILE, MSG_WAITALL) != SIZE_OF_FILE){
        perror("Getting file size error");
        return -1;
    }
    ftcontext.file_full_size = ntohll(ftcontext.file_full_size);

    if( recv(ftcontext.socket, &ftcontext.filename_length, LEN_OF_FILENAME, MSG_WAITALL) != LEN_OF_FILENAME){
        perror("Getting filename length error");
        return -1;
    }
    ftcontext.filename_length = ntohs(ftcontext.filename_length);

    ftcontext.filename = malloc(ftcontext.filename_length);
    for(int i = 0; i < ftcontext.filename_length; ){
        int read_counter = 0;
        if( ( read_counter = recv(ftcontext.socket, &ftcontext.filename[i], ftcontext.filename_length-i, 0)) == 0 ){
            printf("EOF() reached\n");
            return -1;
        }
        i += read_counter;
    }

    printf("GOT %ld %d %s\n", ftcontext.file_full_size, ftcontext.filename_length, ftcontext.filename);

    return 0;
}

int 
file(task_t* task)
{
    struct ftcontext ftcontext = task->file_transfer_context;

    

    printf("FILE TRANSFER\n");
    return 0;
}

int 
consist(task_t* task)
{
    printf("CONSISTENCY CHECKING\n");
    return 0;
}

int 
finish(task_t* task)
{
    printf("FINISHING\n");
    return 0;
}
