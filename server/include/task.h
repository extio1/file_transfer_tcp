#ifndef FTTCP_TASK_H
#define FTTCP_TASK_H

#include <stdint.h>

typedef enum
{
    META = 0,
    FILE_TRANSFER,
    CONSISTENCY_CHECKING,
    DONE
} task_state_t;

typedef struct
{
    unsigned int socket;

    uint64_t file_full_size;
    uint64_t file_curr_size;

    uint16_t filename_length;
    char* filename;

    task_state_t task_state;
} task_t;

extern int init_task(task_t* task, const unsigned int socket_fd);

#endif /* FTTCP_TASK_H */