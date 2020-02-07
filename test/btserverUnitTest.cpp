#include "btserver.h"

//Just because BtServer expects there to be an extern mutex
std::mutex logfile_mutex;

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
