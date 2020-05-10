#include <sim/grid.hpp>
#include <sim/grid_display.hpp>
#include <sim/random.hpp>

#include <SFML/Graphics.hpp>

#include <atomic>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

int main(int argc, char *argv[])
{
    const int width = 1000;
    const int height = 1000;
    sf::RenderWindow window(sf::VideoMode(width, height), "Simulator");
    window.setFramerateLimit(30);

    sim::Random random(0);
    sim::Grid grid(width, height);
    grid.fillRandom(random, 10000);

    std::atomic<bool> done(false);
    std::thread updateThread([&grid, &random, &done]() {
        for (int iteration = 0;; iteration++)
        {
            grid.iterate(random);
            if (done)
                break;
            std::cout << iteration << std::endl;
            if (iteration % 10 == 0)
            {
                std::ostringstream ss;
                ss << "images/img" << std::setfill('0') << std::setw(4) << iteration / 10 << ".png";
                std::cout << "Saving " << ss.str() << std::endl;
                grid.save(ss.str());
            }
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
