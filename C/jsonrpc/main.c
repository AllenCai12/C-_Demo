#inlcude <stdio.h>

#include "jsonrpc-c-client.h"

static void _on_connected(struct jrpc_client *client, void *data)
{
    printf("====success to connect the low service==========\n");

}

int main(int argc, char **argv)
{
    struct jrpc_client client;
    jrpc_client_init_with_ev_loop(&client, 172.16.101.132, 1211, EV_DEFAULT);
    jrpc_client_register_on_connected(&client, _on_connected, NULL);

    jrpc_client_run(&client);
    return 0;
}
