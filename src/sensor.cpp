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

    printf("Sending request byte to Adafruit Feather\n");
    serialPutchar(serial_fd, pinout::adafruit_feather::REQUEST_SIG);

    // May not actually work because of endianness
    // (Also it's probably not the most efficient solution)
    char *data_p = (char *)&data;
    unsigned int idx = 0;
    printf("Waiting for response from Adafruit Feather\n");
    while(idx < sizeof(SensorData))
    {
        while(serialDataAvail(serial_fd) && idx < sizeof(SensorData))
        {
            data_p[idx++] = (char) serialGetchar(serial_fd);
        }
    }
    printf("Sensor data received\n");

    return data;
}
