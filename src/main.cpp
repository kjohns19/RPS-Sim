#include <sim/grid.hpp>
#include <sim/grid_display.hpp>
#include <sim/random.hpp>

#include <SFML/Graphics.hpp>

#include <atomic>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <thread>

#include <getopt.h>
#include <unistd.h>

namespace {

struct CommandlineArgs
{
    int size = 1000;
    int initCount = -1;

    unsigned seed = 0;
    bool seeded = false;

    std::string saveImagePrefix;
    int saveIterationRate = 10;
};

int parseArgs(CommandlineArgs *args, int argc, char *argv[]);

}  // close anonymous namespace

int main(int argc, char *argv[])
{
    CommandlineArgs args;
    if (parseArgs(&args, argc, argv))
        return 1;

    const int width = args.size;
    const int height = args.size;
    sf::RenderWindow window(sf::VideoMode(width, height), "Simulator");
    window.setFramerateLimit(30);

    unsigned seed;
    if (args.seeded)
        seed = args.seed;
    else
    {
        sim::Random random;
        seed = random.randInt<unsigned>(
            std::numeric_limits<unsigned>::min(), std::numeric_limits<unsigned>::max());
        std::cout << "Using seed: " << seed << std::endl;
    }
    sim::Random random(seed);

    sim::Grid grid(width, height);
    grid.fillRandom(random, args.initCount);

    std::atomic<bool> done(false);
    std::thread updateThread([&args, &grid, &random, &done]() {
        for (int iteration = 0;; iteration++)
        {
            grid.iterate(random);
            if (done)
                break;
            std::cout << iteration << std::endl;
            if (!args.saveImagePrefix.empty() && iteration % args.saveIterationRate == 0)
            {
                std::ostringstream ss;
                ss << args.saveImagePrefix << std::setfill('0') << std::setw(4)
                   << iteration / args.saveIterationRate << ".png";
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

namespace {

int parseArgs(CommandlineArgs *args, int argc, char *argv[])
{
    struct option longOptions[] = {
        {"size", required_argument, nullptr, 'n'},
        {"init-count", required_argument, nullptr, 0},
        {"seed", required_argument, nullptr, 's'},
        {"save-image-prefix", required_argument, nullptr, 0},
        {"save-iteration-rate", required_argument, nullptr, 0},
    };
    const char *shortOptions = "n:s:";
    int opt;
    int longIndex;
    std::string optionName;

    int rc = 0;

    while ((opt = getopt_long(argc, argv, shortOptions, longOptions, &longIndex)) != -1)
    {
        switch (opt)
        {
        case 0:
            optionName = longOptions[longIndex].name;
            if (optionName == "init-count")
                args->initCount = std::atoi(optarg);
            else if (optionName == "save-image-prefix")
                args->saveImagePrefix = optarg;
            else if (optionName == "save-iteration-rate")
                args->saveIterationRate = std::atoi(optarg);
            break;
        case 'n':
            args->size = std::atoi(optarg);
            break;
        case 's':
            args->seed = static_cast<unsigned>(std::atoll(optarg));
            args->seeded = true;
            break;
        case '?':
        default:
            rc = 1;
            break;
        }
    }

    if (args->initCount == -1)
        args->initCount = args->size * args->size / 10;

    std::cout << "Size: " << args->size << std::endl;
    std::cout << "Init Count: " << args->initCount << std::endl;

    return rc;
}

}  // close anonymous namespace
