#include "btserver.h"

extern std::mutex logfile_mutex;


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
            send_file();
            close(sock);
        }
    }
    close(server_sock);
}

void BtServer::send_file()
{
    FILE *fp = NULL;

    //Hold mutex the whole time we're using the file
    logfile_mutex.lock();

    fp = fopen(DATA_LOG_FILE, "rb");
    if(fp == NULL)
    {
        printf("Error: BtSever could not open log file for reading.\n");
    }
    else
    {
        unsigned int size, amt_read;
        unsigned char buffer[FILE_BUFFER_SIZE];
        //Get file size
        fseek(fp, 0L, SEEK_END);
        size = ftell(fp);
        rewind(fp);
        //Send file size
        send(sock, (void *)&size, sizeof(int), 0);
        //Send file
        while((amt_read = fread(buffer, 1, FILE_BUFFER_SIZE, fp)) > 0)
        {
            send(sock, buffer, amt_read, 0);
        }
    }
    fclose(fp);
    fp = NULL; //Just to be safe

    logfile_mutex.unlock();
}
