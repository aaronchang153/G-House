#ifndef _BTSERVER_H_
#define _BTSERVER_H_

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>

#include <cstdlib>
#include <thread>

#define BT_CHANNEL   1
#define BT_QUEUE     1
#define BT_POLL_NFDS 1


class BtServer
{
private:
    int server_sock;
    int sock;
    struct sockaddr_rc addr;
    bool running;
    std::thread *server_thread;
public:
    BtServer();
    ~BtServer();
    void start();
    void stop();
    void join();
protected:
    void run();
};

#endif //_BTSERVER_H_
