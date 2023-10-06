#ifndef FTTCP_SEND_H
#define FTTCP_SEND_H

#include <stdint.h>
#include <stddef.h>

/*
 *
 * Recieve and send buffer fully.
 * Return 0 if succeed, -1 if recv()/send() error occured
 * 
*/

extern int send_uint64(int fd, uint64_t* num, size_t n, int flags);
extern int send_str(int fd, char* num, size_t n, int flags);
extern int send_uints(int fd, unsigned short* num, size_t n, int flags);
extern int send_file(int sockfd, int fd, int flags);

extern int recv_file(int sockfd, int fd, int flags);
extern int recv_uint64(int fd, uint64_t* num, size_t n, int flags);
extern int recv_str(int fd, char* num, size_t n, int flags);
extern int recv_uints(int fd, unsigned short* num, size_t n, int flags);

#endif /* FTTCP_SEND_H */