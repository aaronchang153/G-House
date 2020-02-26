#include "btserver.h"

//BtServer expects some extern definitions
std::mutex logfile_mutex;

float PH_THRESH_LOW       = 5.8;
float PH_THRESH_HIGH      = 6.3;
float EC_THRESH_LOW       = 600.0;

int main()
{
    BtServer btserver;
    btserver.start();

    int n;
    printf("Enter 0 to stop: ");
    do
    {
        scanf("%d", &n);
    }while(n != 0);

    btserver.stop();
    btserver.join();

    return 0;
}
