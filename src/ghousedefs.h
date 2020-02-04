#ifndef _GHOUSEDEFS_H_
#define _GHOUSEDEFS_H_

static const char DATA_LOG_FILE[] = "data.log";

static const float PH_THRESH_LOW = 5.8;
static const float PH_THRESH_HIGH = 6.3;

static const float EC_THRESH_LOW = 600.0;
static const float EC_THRESH_HIGH = 1000.0;

static const int DATA_SAMPLE_TIME_MS = 60000;

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
        static const int PIN = 26; // placeholder
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
