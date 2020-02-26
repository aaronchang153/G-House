#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "ghousedefs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mutex>

#include <wiringPi.h>
#include <wiringSerial.h>

class Sensor
{
private:
    int usb_fd;
    int uart_fd;
    const float co2_multiplier = 10.0;
public:
    typedef struct
    {
        float pH;
        float EC;
        float temperature;
        float CO2;
    } SensorData;

    Sensor();
    ~Sensor();
    Sensor::SensorData getData();
};

#endif //_SENSOR_H_
