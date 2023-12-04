#ifdef WIN32
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

#if !defined(__APPLE__) && !defined(__MACH__)
#define CYCLE_MEASURE
#include <stdint.h>

uint64_t cpucycles(){
    return __rdtsc();
}

#else
uint64_t cpucycles(){
    return 0;
}
#endif
