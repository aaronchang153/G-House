#include "ghousedefs.h"
#include "btserver.h"
#include "log.h"
#include "sensor.h"
#include "periph.h"

#include <wiringPi.h>
#include <ctime>


struct datacollectArgs
{
	Periph *led;
	Periph *ph_up;
	Periph *ph_down;
	Periph *nutrition;
	Sensor *sensor;
};

void *datacollectThread(void *argp);
void toggle(Periph *p, int duration);

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

	//TODO: Create thread for datacollect

	btServer.join();

	return 0;
}

void *datacollectThread(void *argp)
{
	struct datacollectArgs *args = (struct datacollectArgs *) argp;
	Sensor::SensorData data;

	while(1)
	{
		data = args->sensor->getData();

		//This log needs to go under a mutex
		log_data(time(NULL), data.pH, data.EC, data.temperature);

		if(data.pH < PH_THRESH_LOW)
			toggle(args->ph_up, 1);
		if(data.pH > PH_THRESH_HIGH)
			toggle(args->ph_down, 1);
		if(data.EC > EC_THRESH_LOW)
			toggle(args->nutrition, 1);

		delay(DATA_SAMPLE_TIME_MS);
	}

	return NULL;
}

void toggle(Periph *p, int duration)
{
	p->output(HIGH);
	delay(duration);
	p->output(LOW);
}
