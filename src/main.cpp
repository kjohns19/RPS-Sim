#include <sim/grid.hpp>
#include <sim/grid_display.hpp>
#include <sim/random.hpp>

#include <SFML/Graphics.hpp>

#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>

int main(int argc, char *argv[])
{
    const int width = 1000;
    const int height = 1000;
    sf::RenderWindow window(sf::VideoMode(width, height), "Simulator");

    std::mutex updateMutex;
    window.setFramerateLimit(30);

    sim::Random random;
    sim::Grid grid(width, height);
    grid.fillRandom(random, 10000);

    std::atomic<bool> done(false);
    std::thread updateThread([&updateMutex, &grid, &random, &done]() {
        int iteration = 0;
        while (1)
        {
            for (int i = 0; i < width * height / 50 && !done; i++)
            {
                std::lock_guard<std::mutex> guard(updateMutex);
                for (int j = 0; j < 50; j++)
                    grid.updateRandomCell(random);
            }
            if (done)
                break;
            std::cout << iteration << std::endl;
            iteration++;
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
        {
            std::lock_guard<std::mutex> guard(updateMutex);
            grid.draw(window);
        }
        window.display();
    }

    done = true;
    updateThread.join();

    return 0;
}
