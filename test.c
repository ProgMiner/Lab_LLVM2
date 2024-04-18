
#define SIM_X_SIZE 256
#define SIM_Y_SIZE 128


static void sim_set_pixel(int x, int y, int v) {
    if (v) {
        sim_pixel_on(x, y);
    } else {
        sim_pixel_off(x, y);
    }
}

int main() {
    for (int i = 0; i < 100; ++i) {
        const int s = sim_rand();

        for (int y = 0; y < SIM_Y_SIZE; ++y) {
            for (int x = 0; x < SIM_X_SIZE; ++x) {
                sim_set_pixel(x, y, (x + y) % s);
            }
        }

        sim_flush();
    }
}
