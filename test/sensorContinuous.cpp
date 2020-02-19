#include "sensor.h"
#include <stdio.h>


int main()
{
    wiringPiSetup();
    Sensor sensor;
    Sensor::SensorData data;

    while(1)
	{
		data = sensor.getData();
		printf("%f   %f   %f\n", data.pH, data.EC, data.temperature);
		delay(1000);
	}

    return 0;
}

