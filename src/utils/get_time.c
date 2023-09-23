#include "get_time.h"

#if defined(_WIN32) || defined(_WIN64)
#include <sys/timeb.h>
uint64_t system_current_time_millis() {
    struct _timeb timebuffer;
    _ftime(&timebuffer);
    return (uint64_t)(((timebuffer.time * 1000) + timebuffer.millitm));
}
#else
#include <sys/time.h>
uint64_t system_current_time_millis() {
    struct timeval time;
    gettimeofday(&time, 0);
    uint64_t t1 = (uint64_t)(time.tv_sec) * 1000;
    uint64_t t2 = (time.tv_usec / 1000);
    return t1 + t2;
}
#endif