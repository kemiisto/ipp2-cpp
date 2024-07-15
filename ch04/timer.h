#ifndef TIMER_H
#define TIMER_H

#include <time.h>

inline double get_time() {
    struct timespec t;
    timespec_get(&t, TIME_UTC);
    return t.tv_sec + t.tv_nsec/1e9;
}

#endif // TIMER_H
