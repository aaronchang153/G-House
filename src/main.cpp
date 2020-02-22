#include "ghousedefs.h"
#include "btserver.h"
#include "log.h"
#include "sensor.h"
#include "periph.h"

#include <wiringPi.h>
#include <ctime>
#include <csignal>
#include <thread>
#include <mutex>

#include <signal.h>


std::mutex logfile_mutex;

float PH_THRESH_LOW       = 5.8;
float PH_THRESH_HIGH      = 6.3;
float EC_THRESH_LOW       = 600.0;
//float EC_THRESH_HIGH      = 1000.0;

static int   DATA_SAMPLE_TIME_MS = 60000;

void datacollect(Sensor &sensor, Periph &led, Periph &ph_up, Periph &ph_down, Periph &nutrition);
void actuate(Sensor::SensorData data, Periph &led, Periph &ph_up, Periph &ph_down, Periph &nutrition);
void toggle(Periph &p, int duration);

BtServer btServer;

bool running;

void signal_handler(int signum)
{
	digitalWrite(pinout::led::PIN, LOW);
	digitalWrite(pinout::pump::ph_up::PIN, LOW);
	digitalWrite(pinout::pump::ph_down::PIN, LOW);
	digitalWrite(pinout::pump::nutrition::PIN, LOW);

	exit(signum);
}

int main()
{
	wiringPiSetup();

	signal(SIGINT, signal_handler);

	running = true;

	Sensor sensor;

	Periph led(pinout::led::PIN);
	Periph ph_up(pinout::pump::ph_up::PIN);
	Periph ph_down(pinout::pump::ph_down::PIN);
	Periph nutrition(pinout::pump::nutrition::PIN);

	led.output(HIGH);
	
	btServer.start(); // starts on its own thread

	std::thread datacollectThread([&]()
	{
		datacollect(sensor, led, ph_up, ph_down, nutrition);
	});

	btServer.join();
	datacollectThread.join();

	led.output(LOW);
	ph_up.output(LOW);
	ph_down.output(LOW);
	nutrition.output(LOW);

	return 0;
}

void datacollect(Sensor &sensor, Periph &led, Periph &ph_up, Periph &ph_down, Periph &nutrition)
{
	Sensor::SensorData data;

	while(running)
	{
		data = sensor.getData();

		//This log needs to go under a mutex
		logfile_mutex.lock();
		log_data(time(NULL), data.pH, data.EC, data.temperature, data.CO2);
		logfile_mutex.unlock();

		actuate(data, led, ph_up, ph_down, nutrition);

		delay(DATA_SAMPLE_TIME_MS);
	}
}

void actuate(Sensor::SensorData data, Periph &led, Periph &ph_up, Periph &ph_down, Periph &nutrition)
{
	if(data.pH < PH_THRESH_LOW)
		toggle(ph_up, 1000);
	if(data.pH > PH_THRESH_HIGH)
		toggle(ph_down, 1000);
	if(data.EC < EC_THRESH_LOW)
		toggle(nutrition, 1000);
}

void toggle(Periph &p, int duration)
{
	p.output(HIGH);
	delay(duration);
	p.output(LOW);
}
