#ifndef _GHOUSEDEFS_H_
#define _GHOUSEDEFS_H_

namespace pinout
{
    static const char SERIAL_DEV[] = "/dev/ttyUSB0";

    namespace adafruit_feather
    {
        static const int BAUD_RATE = 9600;
        static const unsigned char REQUEST_SIG = 0;
    };

    namespace led
    {
        static const int PIN = 20; // placeholder
        static const int ON = 1;
        static const int OFF = 0;
    };

    namespace pump
    {
        namespace ph_up
        {
            static const int PIN = 21; // placeholder
            static const int ON = 1;
            static const int OFF = 0;
        };
        namespace ph_down
        {
            static const int PIN = 22; // placeholder
            static const int ON = 1;
            static const int OFF = 0;
        };
        namespace nutrition
        {
            static const int PIN = 23; // placeholder
            static const int ON = 1;
            static const int OFF = 0;
        };
    };
};

#endif //_GHOUSEDEFS_H_
