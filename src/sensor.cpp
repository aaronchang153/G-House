#include "sensor.h"

Sensor::Sensor()
{
    if((usb_fd = serialOpen(pinout::SERIAL_DEV, pinout::adafruit_feather::BAUD_RATE)) < 0)
    {
        printf("Failed to open usb device.\n");
    }
    if((uart_fd = serialOpen(pinout::UART_DEV, pinout::co2::BAUD_RATE)) < 0)
    {
        printf("Failed to open uart device.\n");
    }
}

Sensor::~Sensor()
{
    if(usb_fd >= 0)
    {
        serialClose(usb_fd);
    }
    if(uart_fd >= 0)
    {
        serialClose(uart_fd);
    }
}

Sensor::SensorData Sensor::getData()
{
    SensorData data;

    if(usb_fd < 0 || uart_fd < 0)
    {
        printf("Warning: Current serial device is not valid. No action taken.\n");
        memset((void *)&data, 0, sizeof(SensorData));
        return data;
    }

    //Get pH, EC, temperature readings
    printf("Sending request byte to Adafruit Feather\n");
    serialPutchar(usb_fd, pinout::adafruit_feather::REQUEST_SIG);

    char *data_p = (char *)&data;
    unsigned int idx = 0;
    printf("Waiting for response from Adafruit Feather\n");
    while(idx < sizeof(SensorData))
    {
        while(serialDataAvail(usb_fd) && idx < sizeof(SensorData))
        {
            data_p[idx++] = (char) serialGetchar(usb_fd);
        }
    }

    //Get CO2 reading
    char co2_str[6];
    char string_buffer[10];
    strcpy(string_buffer, "M 4\r\n");
    serialPuts(uart_fd, string_buffer); //set display mode to show only CO2
    strcpy(string_buffer, "K 2\r\n");
    serialPuts(uart_fd, string_buffer); //set operating mode

    serialFlush(uart_fd);

    for(int i = 0; i < 10; i++)
    {
        if((string_buffer[i] = (char)serialGetChar(uart_fd)) == -1)
        {
            //on error, zero out buffer
            memset(string_buffer, 0, 10 * sizeof(char));
        }
    }

    strncpy(co2_str, string_buffer+2, 6);
    data.CO2 = (float) atof(co2_str);

    printf("Sensor data received\n");

    return data;
}
