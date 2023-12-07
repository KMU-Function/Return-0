// /**
//  * @file cycle.c
//  * @brief Cycle measurement functions for CPU cycles on different platforms.
//  */

// #ifdef WIN32
// #include <intrin.h>
// #else
// #include <x86intrin.h>
// #endif

// #if !defined(__APPLE__) && !defined(__MACH__)
// #define CYCLE_MEASURE
// #include <stdint.h>

//  /**
//   * @brief Measure CPU cycles using the RDTSC instruction.
//   * @return The number of CPU cycles.
//   */
// uint64_t cpucycles() {
//     return __rdtsc();
// }

// #else

//  /**
//   * @brief Dummy function for measuring CPU cycles on unsupported platforms.
//   * @return Always returns 0 on unsupported platforms.
//   */
// uint64_t cpucycles() {
//     return 0;
// }
// #endif
/**
 * @file cycle.c
 * @brief Cycle measurement functions for CPU cycles on different platforms.
 */

#if defined(WIN32)
#include <intrin.h>
#elif defined(__APPLE__) || defined(__MACH__)
#include <stdint.h>

 /**
  * @brief Dummy function for measuring CPU cycles on unsupported platforms.
  * @return Always returns 0 on unsupported platforms.
  */
unsigned long long cpucycles(void) {
  return 0;
}

#else
#include <x86intrin.h>
#include <stdint.h>
#define CYCLE_MEASURE

 /**
  * @brief Measure CPU cycles using the RDTSC instruction.
  * @return The number of CPU cycles.
  */
uint64_t cpucycles() {
  return __rdtsc();
}

#endif
