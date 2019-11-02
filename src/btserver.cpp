#include "btserver.h"


BtServer::BtServer() : ready(false), server_thread(NULL)
{
    if((server_sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)) < 0)
    {
        printf("Error creating socket.\n");
        return;
    }

    // BDADDR_ANY is defined as (&(bdaddr_t) {{0, 0, 0, 0, 0, 0}})
    // so tmp is used in its place
    bdaddr_t tmp = { };

    addr.rc_family = AF_BLUETOOTH;
    bacpy(&addr.rc_bdaddr, &tmp);
    addr.rc_channel = htobs(BT_CHANNEL);
    socklen_t alen = sizeof(addr);

    if(bind(server_sock, (struct sockaddr *)&addr, alen) < 0)
    {
        printf("Error binding socket.\n");
        return;
    }

    running = false;
    ready = true;
}

BtServer::~BtServer()
{
    if(server_thread != NULL)
    {
        delete server_thread;
    }
}

void BtServer::start()
{
    if(!ready)
    {
        fprintf(stderr, "Warning: Attempting to start server without a valid socket. No action taken.\n");
        return;
    }
    if(server_thread == NULL)
    {
        server_thread = new std::thread(&BtServer::run, this);
    }
}

void BtServer::stop()
{
    running = false;
}

void BtServer::join()
{
    if(server_thread != NULL)
    {
        server_thread->join();
        delete server_thread;
        server_thread = NULL;
    }
}

void BtServer::run()
{
    socklen_t alen = sizeof(addr);

    struct pollfd poll_set[BT_POLL_NFDS];
    poll_set[0].fd = server_sock;
    poll_set[0].events = POLLIN;

    listen(server_sock, BT_QUEUE);

    running = true;
    while(running)
    {
        //poll sockets in poll_set every 1000 ms
        poll(poll_set, BT_POLL_NFDS, 1000);

        if(poll_set[0].revents & POLLIN)
        {
            //If there's data to be read on the server socket
            sock = accept(server_sock, (struct sockaddr *)&addr, &alen);

            char buffer[] = "Test string from Pi.";

            send(sock, buffer, strlen(buffer), 0);

            close(sock);
        }
    }
    close(server_sock);
}
