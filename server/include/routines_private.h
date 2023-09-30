#ifndef FTTCP_ROUTINES_H
#define FTTCP_ROUTINES_H

extern int listen_routine(const unsigned int lst_port);
extern void* serving_routine(void* args);

#endif /* FTTCP_ROUTINES_H */