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
};

#endif //_GHOUSEDEFS_H_
