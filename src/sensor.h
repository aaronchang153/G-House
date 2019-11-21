#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "ghousedefs.h"

#include <stdio.h>
#include <string.h>

#include <wiringPi.h>
#include <wiringSerial.h>

class Sensor
{
private:
    int serial_fd;
public:
    typedef struct
    {
        float pH;
        float EC;
        float temperature;
    } SensorData;

    Sensor();
    ~Sensor();
    Sensor::SensorData getData();
};

#endif //_SENSOR_H_
