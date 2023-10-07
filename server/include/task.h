#ifndef FTTCP_TASK_H
#define FTTCP_TASK_H

#include <sys/socket.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct task task_t;

typedef int (*meta_f_t)(task_t* task);
typedef int (*file_trans_f_t)(task_t* task);
typedef int (*consistency_f_t)(task_t* task);
typedef int (*finisher_f_t)(task_t* task);

struct state_function
{
    meta_f_t meta;
    file_trans_f_t ftrans;
    consistency_f_t consist;
    finisher_f_t fin;
};

struct ftcontext
{
    uint64_t file_full_size;
    uint64_t file_curr_size;
    uint64_t file_prev_size;

    unsigned int n_delta;
    uint64_t avg_speed;

    uint16_t filename_length;

    unsigned int socket;
    unsigned int fd;

    char* filename;    
};

struct cltaddr
{
    struct sockaddr addr;
    socklen_t addrlen;
};

typedef enum
{
    META = 0,
    FILE_TRANSFER,
    CONSISTENCY_CHECKING,
    DIE
} task_state_t;

typedef struct task
{
    struct ftcontext file_transfer_context;
    struct state_function state_function;
    struct cltaddr cltaddr;

    unsigned short succeed;
    task_state_t task_state;
} task_t;


extern void init_task(task_t* task, const int socket, const struct state_function* state_function, const struct cltaddr* cltaddr);
extern void free_task(task_t* task);


extern int create_file(task_t* task, const char* filename);

#endif /* FTTCP_TASK_H */