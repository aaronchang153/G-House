#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define CHANNEL 1


int main()
{
    int sock, client;
    socklen_t alen;
    struct sockaddr_rc addr;

    if((sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)) < 0)
    {
        printf("Error creating socket.\n");
        return 1;
    }

    // BDADDR_ANY is defined as (&(bdaddr_t) {{0, 0, 0, 0, 0, 0}})
    // so tmp is used in its place
    bdaddr_t tmp = { };

    addr.rc_family = AF_BLUETOOTH;
    bacpy(&addr.rc_bdaddr, &tmp);
    addr.rc_channel = htobs(CHANNEL);
    alen = sizeof(addr);

    if(bind(sock, (struct sockaddr *)&addr, alen) < 0)
    {
        printf("Error binding socket.\n");
        return 1;
    }

    listen(sock, 1);
    printf("Waiting for connection...\n");
    client = accept(sock, (struct sockaddr *)&addr, &alen);

    char buffer[] = "Test string from Pi.";

    send(client, buffer, strlen(buffer), 0);

    close(client);
    close(sock);

    return 0;
}
