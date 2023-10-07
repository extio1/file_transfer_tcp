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
    task_t* task = (task_t*) malloc(sizeof(task_t));
    struct cltaddr cltaddr = {.addr = addr, .addrlen = socklen};
    init_task(task, socket, &state_function, &cltaddr);

    if( create_worker(task) != 0){
        perror("create_worket() error");
        return -1;
    }

    return 0;
}