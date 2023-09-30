#ifndef FTTCP_PROTOCOL_H
#define FTTCP_PROTOCOL_H

/*
 *
 *  Protocol:
 *  0--------------------0-----------------------0----------0---------------0
 *  | FILE SIZE (64 bit) | FILENAME LEN (16 bit) | FILENAME | RAW FILE DATA |
 *  0--------------------0-----------------------0----------0---------------0
 * /_________________________META___________________________/
 * 
*/

#define SIZE_OF_FILE 8
#define LEN_OF_FILENAME 2

#endif /* FTTCP_PROTOCOL_H */