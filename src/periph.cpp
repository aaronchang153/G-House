#include "periph.h"

Periph::Periph(int pin)
{
    this->pin = pin;
    pinMode(this->pin, OUTPUT);
    digitalWrite(this->pin, LOW);
}

Periph::~Periph()
{
    // output LOW (default state) when the device is deleted
    digitalWrite(this->pin, LOW);
}

void Periph::output(int sig)
{
    digitalWrite(pin, sig);
}

int Periph::getPin()
{
    return pin;
}
