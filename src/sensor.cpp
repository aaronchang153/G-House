#include "sensor.h"

Sensor::Sensor()
{
    if((serial_fd = serialOpen(pinout::SERIAL_DEV, pinout::adafruit_feather::BAUD_RATE)) < 0)
    {
        printf("Failed to open serial device.\n");
    }
}

Sensor::~Sensor()
{
    if(serial_fd >= 0)
    {
        serialClose(serial_fd);
    }
}

Sensor::SensorData Sensor::getData()
{
    SensorData data;

    if(serial_fd < 0)
    {
        printf("Warning: Current serial device is not valid. No action taken.\n");
        memset((void *)&data, 0, sizeof(SensorData));
        return data;
    }

    serialPutchar(serial_fd, 0);

    // May not actually work because of endianness
    // (Also it's probably not the most efficient solution)
    char *data_p = (char *)&data;
    unsigned int idx = 0;
    while(idx < sizeof(SensorData))
    {
        if((data_p[idx] = (char) serialGetchar(serial_fd)) != -1)
        {
            // If a character is successfully read from the device
            idx++;
        }
    }

    return data;
}
