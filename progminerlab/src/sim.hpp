#pragma once

#include <iostream>
#include <thread>
#include <array>
#include <mutex>

#include <SFML/Graphics.hpp>


#define STEP_DELAY (std::chrono::milliseconds { 100 })


class SimContext {

    sf::RenderWindow window { sf::VideoMode(SIM_X_SIZE, SIM_Y_SIZE), "ProgMinerLab simulator" };
    sf::Thread thread { &SimContext::rendering_thread, this };

    std::array<std::array<uint32_t, SIM_X_SIZE>, SIM_Y_SIZE> buffer;
    std::mutex buffer_mtx;

    std::array<std::array<uint32_t, SIM_X_SIZE>, SIM_Y_SIZE> next_buffer;

public:

    explicit SimContext() {
        window.setActive(false);
        thread.launch();
    }

    void set_pixel(int32_t x, int32_t y, bool value) {
        next_buffer[y][x] = 0xFF000000 + 0xFF00 * value;
    }

    void flush() {
        {
            std::scoped_lock lock { buffer_mtx };
            buffer = next_buffer;
        }

        std::this_thread::sleep_for(STEP_DELAY);
    }

private:

    void rendering_thread() {
        window.setActive(true);

        sf::Texture texture;
        if (!texture.create(SIM_X_SIZE, SIM_Y_SIZE)) {
            std::cerr << "Texture wasn't created" << std::endl;
            return;
        }

        sf::Sprite sprite { texture };

        while (window.isOpen()) {
            sf::Event event;

            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            {
                std::scoped_lock lock { buffer_mtx };

                texture.update(reinterpret_cast<uint8_t *>(&buffer));
            }

            window.clear();
            window.draw(sprite);
            window.display();
        }
    }
};
