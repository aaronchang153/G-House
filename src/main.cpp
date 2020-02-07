#include "ghousedefs.h"
#include "btserver.h"
#include "log.h"
#include "sensor.h"
#include "periph.h"

#include <wiringPi.h>
#include <ctime>
#include <thread>
#include <mutex>


std::mutex logfile_mutex;

void datacollect(Sensor &sensor, Periph &led, Periph &ph_up, Periph &ph_down, Periph &nutrition);
void toggle(Periph p, int duration);

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

    std::thread datacollectThread([&]()
    {
        datacollect(sensor, led, ph_up, ph_down, nutrition);
    });

	btServer.join();
	datacollectThread.join();

	return 0;
}

void datacollect(Sensor &sensor, Periph &led, Periph &ph_up, Periph &ph_down, Periph &nutrition)
{
	Sensor::SensorData data;

	while(1)
	{
		data = sensor.getData();

		//This log needs to go under a mutex
		logfile_mutex.lock();
		log_data(time(NULL), data.pH, data.EC, data.temperature);
		logfile_mutex.unlock();

		if(data.pH < PH_THRESH_LOW)
			toggle(ph_up, 1);
		if(data.pH > PH_THRESH_HIGH)
			toggle(ph_down, 1);
		if(data.EC > EC_THRESH_LOW)
			toggle(nutrition, 1);

		delay(DATA_SAMPLE_TIME_MS);
	}
}

void toggle(Periph p, int duration)
{
	p.output(HIGH);
	delay(duration);
	p.output(LOW);
}
