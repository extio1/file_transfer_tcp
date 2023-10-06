#include "../include/worker.h"
#include "../include/task.h"

#include <stdbool.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

void* 
worker_routine(void* args)
{
    bool finished = false;

    task_t* task = (task_t *)args;
    // SO_RCVBUF option to setsockopt doesn't set for MSG_WAITALL reads
    
    while(!finished){
        switch (task->task_state)
        {
        case META:
            task->task_state = (task->state_function.meta(task) == 0) ? FILE_TRANSFER : DIE;
            break;
        case FILE_TRANSFER:
            task->task_state = (task->state_function.ftrans(task) == 0) ? CONSISTENCY_CHECKING : DIE;
            break;
        case CONSISTENCY_CHECKING:
            task->task_state = (task->state_function.consist(task) == 0) ? DIE : DIE;
            break;
        case DIE:
            task->state_function.fin(task);
            finished = true;
            break;
        }        
    }

    printf("File transfer finished\n");
    return 0;
}

int 
create_worker(task_t* task)
{
    pthread_t tid;
    pthread_attr_t attr;

    if( pthread_attr_init(&attr) != 0 ){
        perror("pthread_attr_init() err");
        return -1;
    }
    if( pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0 ){
        perror("pthread_atte_setdetachstate() err");
        return -1;
    }

    if( pthread_create(&tid, &attr, worker_routine, task) != 0 ){
        perror("pthread_create() error");
        return -1;
    }

    return 0;
}