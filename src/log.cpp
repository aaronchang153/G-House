#include "log.h"

void log_data(time_t t, float pH, float EC, float temp, float co2)
{
    FILE *fp = NULL;
    fp = fopen(DATA_LOG_FILE, "a");
    if(fp != NULL)
    {
        fprintf(fp, "%d,%f,%f,%f,%f\n", (int) t, pH, EC, temp, co2);
        fclose(fp);
    }
}
