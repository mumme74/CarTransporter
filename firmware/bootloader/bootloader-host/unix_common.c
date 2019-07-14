
#include <time.h>
#include <sys/time.h>
#include "unix_common.h"


uint32_t timeGetTime()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return (uint32_t)((now.tv_sec * 1000) + (now.tv_usec / 1000));
}
