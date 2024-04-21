#include "sim.h"

#include <random>


static SimContext ctx;

void sim_set_pixel(int32_t x, int32_t y, int32_t value) {
    ctx.set_pixel(x, y, value);
}

void sim_flush() {
    ctx.flush();
}

int32_t sim_rand() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int32_t> distrib(0);

    return distrib(gen);
}
