#include "ghousedefs.h"
#include "btserver.h"
#include "log.h"
#include "sensor.h"
#include "periph.h"

#include <wiringPi.h>
#include <ctime>


void *datacollectThread(void *args);
void *actuationThread(void *args);

int main()
{
	wiringPiSetup();

	Sensor sensor;

	Periph led(pinout::led::PIN);
	Periph ph_up(pinout::pump::ph_up::PIN);
	Periph ph_down(pinout::pump::ph_down::PIN);
	Periph nutrition(pinout::pump::nutrition::PIN);

	BtServer btServer;
	
	btServer.start(); // starts on its own thread

	btServer.join();

	return 0;
}

void *datacollectThread(void *args)
{
	Sensor *sensor = (Sensor *) args;
	Sensor::SensorData data;

	while(1)
	{
		data = sensor->getData();
		//This log needs to go under a mutex
		log_data(time(NULL), data.pH, data.EC, data.temperature);

		delay(DATA_SAMPLE_TIME_MS);
	}

	return NULL;
}

void *actuationThread(void *args);
