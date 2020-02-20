#include "sensor.h"
#include <stdio.h>


int main()
{
    wiringPiSetup();
    Sensor sensor;
    Sensor::SensorData data;
    data = sensor.getData();

    printf("%f   %f   %f   %f\n", data.pH, data.EC, data.temperature, data.CO2);

    return 0;
}
