#include "hnconvert.h"

#include <netinet/in.h>

uint64_t 
ntohll(uint64_t a)
{
    uint32_t left = 0 + (a >> 32);
    uint32_t right = 0 + (a ^ ((uint64_t)left << 32));

    uint32_t conv_left  = ntohl(left);
    uint32_t conv_right = ntohl(right);

    if( conv_left != left || conv_right != right){
        return ((uint64_t) conv_right << 32) | conv_left; 
    } else {
        return ((uint64_t) conv_left << 32) | conv_right;
    }
}

uint64_t 
htonll(uint64_t a)
{
    uint32_t left = 0 + (a >> 32);
    uint32_t right = 0 + (a ^ ((uint64_t)left << 32));

    uint32_t conv_left  = htonl(left);
    uint32_t conv_right = htonl(right);

    if( conv_left != left || conv_right != right){
        return ((uint64_t) conv_right << 32) | conv_left; 
    } else {
        return ((uint64_t) conv_left << 32) | conv_right;
    }
}