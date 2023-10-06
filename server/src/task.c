#include "../include/task.h"

#include <strings.h>
#include <string.h>

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