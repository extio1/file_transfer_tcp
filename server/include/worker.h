#ifndef FTTCP_WORKER_H
#define FTTCP_WORKER_H

#include "task.h"

#include <sys/socket.h>

extern int create_worker(task_t* task);
extern void* worker_routine(void* args);

#endif /* FTTCP_WORKER_H */