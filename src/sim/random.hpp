#ifndef INCLUDED_SIM__RANDOM_HPP
#define INCLUDED_SIM__RANDOM_HPP

#include <random>
#include <type_traits>

namespace sim {

class Random
{
  public:
    Random();
    Random(unsigned seed);

    void seed(unsigned seed);

    static Random &instance();

    template<typename Integer, std::enable_if_t<std::is_integral<Integer>::value, int> = 0>
    Integer randInt(Integer min, Integer max);

    template<typename Float, std::enable_if_t<std::is_floating_point<Float>::value, int> = 0>
    Float randFloat(Float min, Float max);

    template<typename Float, std::enable_if_t<std::is_floating_point<Float>::value, int> = 0>
    Float randFloat();

    std::mt19937 &generator();

  private:
    std::mt19937 d_engine;
};

inline Random::Random() : d_engine(std::random_device()()) {}
inline Random::Random(unsigned seed) : d_engine(seed) {}

inline void Random::seed(unsigned seed)
{
    d_engine.seed(seed);
}

inline Random &Random::instance()
{
    static Random random;
    return random;
}

template<typename Integer, std::enable_if_t<std::is_integral<Integer>::value, int>>
Integer Random::randInt(Integer min, Integer max)
{
    std::uniform_int_distribution<Integer> dist(min, max);
    return dist(d_engine);
}

template<typename Float, std::enable_if_t<std::is_floating_point<Float>::value, int>>
Float Random::randFloat(Float min, Float max)
{
    std::uniform_real_distribution<Float> dist(min, max);
    return dist(d_engine);
}

template<typename Float, std::enable_if_t<std::is_floating_point<Float>::value, int>>
Float Random::randFloat()
{
    return randFloat<Float>(static_cast<Float>(0), static_cast<Float>(1));
}

inline std::mt19937 &Random::generator()
{
    return d_engine;
}

}  // close namespace sim

#endif  // INCLUDED_SIM__RANDOM_HPP
