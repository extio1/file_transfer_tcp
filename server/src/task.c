#include "../include/task.h"

#include <strings.h>

int
init_task(task_t* task, const unsigned int sockfd)
{
    bzero(task, sizeof(task_t));

    task->socket = sockfd;
    task->task_state = META;
}