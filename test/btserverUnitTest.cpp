#include "btserver.h"


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
