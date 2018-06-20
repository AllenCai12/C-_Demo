#ifndef C_SOCKET_H
#define C_SOCKET_H

    typedef int (Handle *)(void *arg);  

    int socket_init(int *sfd);
    int socket_init_recv_handle(int *cfd, Handle handle);
    int socket_send(int cfd, void *arg);

#endif
