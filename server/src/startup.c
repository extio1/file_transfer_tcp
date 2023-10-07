#include "../include/routines_private.h"

#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int 
main(int argc, char** argv)
{
    const char* working_dir = "./uploads";
    unsigned int listen_port;

    if( argc < 2 ){
        printf("Not enough arguments were passed.\n");
        return -1;
    }

    if( mkdir(working_dir, 0744) != 0  && errno != EEXIST){
        perror("mkdir(./uploads) error.");
        return -1;
    }
/*
    if( chroot(working_dir) != 0 ){
        perror("chroot() error");
        return -1;
    }
*/
    if( chdir(working_dir) != 0 ){
        perror("chroot() error");
        return -1;
    }

    listen_port = atol(argv[1]);

    if(listen_port <= 0 || listen_port >= 65536) {
        printf("Please enter the port number correctly. In range (0, 65536).\n");
        return -1;
    }

    if( listen_routine(listen_port) != 0 ){
        perror("listen_routine() error");
        return -1;
    }

    return 0;
}