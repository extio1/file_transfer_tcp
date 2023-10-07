#include "../include/clt_context.h"
#include "../include/snd_routine.h"

#include <stdio.h>

int
main(int argc, char** argv)
{
    connection_context_t context;
    if(argc < 4){
        printf("Please enter arguments <filepath> <ip/servername> <port>.\n");
        return -1;
    }

    if( create_context(argv[1], argv[2], argv[3], &context) != 0){
        printf("Parsing input error. Please enter arguments <filepath> <ip/servername> <port>.\n");
        return -1;
    }

    printf(">>> CLT'll send file <%s> sizeof %ld with name length %d\n", context.filename, context.file_size, context.filename_length);

    if( cp_file_to(&context) != 0){
        printf("File copy to server error\n");
        return -1;
    }

    free_context(&context);

    return 0;
}

