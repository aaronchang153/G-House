#ifndef _LOG_H_
#define _LOG_H_

#include <cstdio>
#include <ctime>
#include <unistd.h>
#include "ghousedefs.h"

void log_data(time_t t, float pH, float EC, float temp);

#endif //_LOG_H_
