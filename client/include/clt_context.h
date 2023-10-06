#ifndef FTTPC_CLT_TYPES_H
#define FTTPC_CLT_TYPES_H 1

#include <stdbool.h>
#include <stdio.h>

typedef struct
{
    int socket;
    int filefd;

    size_t file_size;
    unsigned short filename_length;
    char* filename;

    bool initialized;
} connection_context_t;

extern int create_context(const char* filename, const char* servername, const char* port, connection_context_t* context);
extern int free_context(connection_context_t* context);

#endif /* FTTPC_CLT_TYPES_H */