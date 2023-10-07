#ifndef FTTCP_EXECUTOR_H
#define FTTCP_EXECUTOR_H 1

#include "task.h"

#include <arpa/inet.h>
#include <sys/socket.h>

extern int execute(int socket, struct sockaddr sockaddr, socklen_t socklen);

#endif /* FTTCP_EXECUTOR_H */