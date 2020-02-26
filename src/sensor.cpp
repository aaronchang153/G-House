#include "sensor.h"

std::mutex sensor_mutex;


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

    sensor_mutex.lock();

    //Get pH, EC, temperature readings
    printf("Sending request byte to Adafruit Feather\n");
    serialPutchar(usb_fd, pinout::adafruit_feather::REQUEST_SIG);

    char *data_p = (char *)&data;
    unsigned int idx = 0;
    printf("Waiting for response from Adafruit Feather\n");
    while(idx < 3 * sizeof(float))
    {
        while(serialDataAvail(usb_fd))
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

	//Takes about 3 readings on average to get good data
	for(int i = 0; i < 3; i++)
	{
		strcpy(string_buffer, "Z\r\n");
		serialPuts(uart_fd, string_buffer);

		for(int i = 0; i < 10; i++)
		{
			if((string_buffer[i] = (char)serialGetchar(uart_fd)) == 0xFF)
			{
			    //serialGetchar() times out after 10 seconds
			    printf("Warning: Could not get CO2 reading.\n");
			    data.CO2 = 0.0;
			    return data;
			}
		}
		delay(1000);
	}

    strncpy(co2_str, string_buffer+2, 6);
    data.CO2 = (float) atof(co2_str) * co2_multiplier;

    printf("Sensor data received\n");

    sensor_mutex.unlock();

    return data;
}
