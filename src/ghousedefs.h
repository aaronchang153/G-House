#ifndef _GHOUSEDEFS_H_
#define _GHOUSEDEFS_H_

namespace pinout
{
    static const char SERIAL_DEV[] = "/dev/ttyUSB0";

    namespace adafruit_feather
    {
        static const int BAUD_RATE = 9600;
        static const int TX_PIN = 15;
        static const int RX_PIN = 16;
        static const int TX_LEN = 1;
        static const int RX_LEN = 8;
    };
};

#endif //_GHOUSEDEFS_H_
