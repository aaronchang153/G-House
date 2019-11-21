#include "sensor.h"
#include <stdio.h>


int main()
{
    wiringPiSetup();
    Sensor sensor;
    Sensor::SensorData data;
    data = sensor.getData();

    printf("%f   %f   %f\n", data.pH, data.EC, data.temperature);

    return 0;
}
