#include <sim/grid.hpp>
#include <sim/grid_display.hpp>
#include <sim/random.hpp>

#include <SFML/Graphics.hpp>

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

int main(int argc, char *argv[])
{
    const int width = 1000;
    const int height = 1000;
    sf::RenderWindow window(sf::VideoMode(width, height), "Simulator");
    window.setFramerateLimit(30);

    sim::Random random;
    sim::Grid grid(width, height);
    grid.fillRandom(random, 10000);

    std::atomic<bool> done(false);
    std::thread updateThread([&grid, &random, &done]() {
        for (int iteration = 0;; iteration++)
        {
            auto start = std::chrono::high_resolution_clock::now();
            grid.iterate(random);
            auto end = std::chrono::high_resolution_clock::now();
            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            if (done)
                break;
            std::cout << iteration << std::endl;
            std::cout << (diff.count() / static_cast<double>(width * height)) << std::endl;
        }
    });

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        grid.draw(window);
        window.display();
    }

    done = true;
    updateThread.join();

    return 0;
}
