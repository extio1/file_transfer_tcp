#include "../include/executor.h"
#include "../include/worker.h"
#include "../include/task.h"

#include "state.c"

#include <stdlib.h>

const struct state_function state_function = {.meta = meta,
                                              .ftrans = file,
                                              .consist = consist,
                                              .fin = finish};

int
execute(int socket, struct sockaddr addr, socklen_t socklen)
{
    task_t* task = malloc(sizeof(task_t));
    init_task(task, socket, &state_function, &(struct cltaddr){.addr = addr, .addrlen = socklen});

    if( create_worker(task) != 0){
        perror("create_worket() error");
        return -1;
    }

    return 0;
}