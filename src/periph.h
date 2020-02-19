#ifndef _PERIPH_H_
#define _PERIPH_H_

#include <wiringPi.h>

class Periph
{
public:
    Periph(int pin);
    ~Periph();
    void output(int sig);
    int getPin();
private:
    int pin;
};

#endif //_PERIPH_H_
