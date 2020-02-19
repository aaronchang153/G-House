#include "periph.h"
#include "ghousedefs.h"
#include <stdio.h>

#define NUMBER_OF_DEVICES 4

int main()
{
    wiringPiSetup();

    Periph *periph[NUMBER_OF_DEVICES];

    periph[0] = new Periph(pinout::led::PIN);
    periph[1] = new Periph(pinout::pump::ph_up::PIN);
    periph[2] = new Periph(pinout::pump::ph_down::PIN);
    periph[3] = new Periph(pinout::pump::nutrition::PIN);

    printf("Testing %d total devices\n", NUMBER_OF_DEVICES);

    for(int i = 0; i < NUMBER_OF_DEVICES; i++)
    {
        if(periph[i] != NULL)
        {
            printf("Testing device %d on pin %d\n", i, periph[i]->getPin());

            printf("Device %d: Output high for 1 second...\n", i);
            periph[i]->output(HIGH);
            delay(1000);

            printf("Device %d: Output low for 1 second...\n", i);
            periph[i]->output(LOW);
            delay(1000);

            printf("Device %d: Done\n", i);

            delete periph[i];
            periph[i] = NULL;
        }
        else
        {
            printf("Error: Device %d could not be created.\n", i);
            return 1;
        }
    }
    return 0;
}
