
#define SIM_X_SIZE 256
#define SIM_Y_SIZE 128

#define GEN_IDX(__idx) ((__idx) / 32)
#define GEN_SIZE(__s) GEN_IDX((__s) + 31)


static inline int get_gen_value(const int * gen, int x, int y) {
    x = (x % SIM_X_SIZE + SIM_X_SIZE) % SIM_X_SIZE;
    y = (y % SIM_Y_SIZE + SIM_Y_SIZE) % SIM_Y_SIZE;

    const unsigned idx = y * SIM_X_SIZE + x;
    return (gen[GEN_IDX(idx)] >> (idx % 32)) & 1;
}

static inline void set_gen_value(int * gen, int x, int y, int v) {
    x = (x % SIM_X_SIZE + SIM_X_SIZE) % SIM_X_SIZE;
    y = (y % SIM_Y_SIZE + SIM_Y_SIZE) % SIM_Y_SIZE;

    const int idx = y * SIM_X_SIZE + x;

    if (v) {
        gen[GEN_IDX(idx)] |= 1 << (idx % 32);
    } else {
        gen[GEN_IDX(idx)] &= ~(1 << (idx % 32));
    }
}

static void draw_gen(const int * gen) {
    for (int y = 0; y < SIM_Y_SIZE; y++) {
        for (int x = 0; x < SIM_X_SIZE; x++) {
            if (get_gen_value(gen, x, y)) {
                sim_pixel_on(x, y);
            } else {
                sim_pixel_off(x, y);
            }
        }
    }

    sim_flush();
}

static unsigned calc_gen(int * next_gen, const int * prev_gen) {
    int has_changes = 0;
    int has_living = 0;

	for (int y = 0; y < SIM_Y_SIZE; ++y) {
	    for (int x = 0; x < SIM_X_SIZE; ++x) {
            int neighbours = 0;

            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dx == 0 && dy == 0) {
                        continue;
                    }

                    neighbours += get_gen_value(prev_gen, x + dx, y + dy);
                }
            }

            const int old_value = get_gen_value(prev_gen, x, y);
            const int next_value = old_value
                ? neighbours == 2 || neighbours == 3
                : neighbours == 3;

            if (old_value != next_value) {
                has_changes = 1;
            }

            if (next_value) {
                has_living = 1;
            }

            set_gen_value(next_gen, x, y, next_value);
        }
    }

    return has_changes || has_living;
}

int main(void) {
    int gen1[GEN_SIZE(SIM_Y_SIZE * SIM_X_SIZE)];
    int gen2[GEN_SIZE(SIM_Y_SIZE * SIM_X_SIZE)];
    int * next_gen = gen1;
    int * prev_gen = gen2;

    for (int y = 0; y < SIM_Y_SIZE; y++) {
        for (int x = 0; x < SIM_X_SIZE; x++) {
            set_gen_value(prev_gen, x, y, sim_rand() % 2);
        }
    }

    draw_gen(prev_gen);

    while (calc_gen(next_gen, prev_gen)) {
        draw_gen(next_gen);

        int * const tmp = prev_gen;
        prev_gen = next_gen;
        next_gen = tmp;
    }

    return 0;
}
