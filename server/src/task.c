#include "../include/task.h"
#include "../include/executor.h"

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <fcntl.h>

void
init_task(task_t* task, const int socket, const struct state_function* state_function, const struct cltaddr* cltaddr)
{
    bzero(task, sizeof(task_t));

    memcpy(&task->state_function, state_function, sizeof(struct state_function));

    task->cltaddr.addr = cltaddr->addr;
    task->cltaddr.addrlen = cltaddr->addrlen;
    task->file_transfer_context.socket = socket;
    task->task_state = META;
}

void 
free_task(task_t* task)
{
    free(task->file_transfer_context.filename);
    free(task);
}

int
create_file(task_t* task, const char* filename)
{
    int fd = 0;
    if( (fd = creat(filename, 0700)) == -1){
        perror("creat() error");
        return -1;
    }

    task->file_transfer_context.fd = fd;

    return 0;
}
