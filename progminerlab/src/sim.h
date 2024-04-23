#pragma once

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif


#define SIM_X_SIZE 256
#define SIM_Y_SIZE 128

#ifdef __cplusplus
#include "sim.hpp"
#endif


#ifdef __progminerlab__

static inline void sim_set_pixel(int32_t x, int32_t y, int32_t value) {
    if (value) {
        sim_pixel_on(x, y);
    } else {
        sim_pixel_off(x, y);
    }
}

#else

#ifdef __cplusplus
extern "C" {
#endif

extern void sim_set_pixel(int32_t x, int32_t y, int32_t value);
extern void sim_flush(void);
extern int32_t sim_rand(void);

extern void dump(int32_t label, int32_t value);

#ifdef __cplusplus
}
#endif

#endif
