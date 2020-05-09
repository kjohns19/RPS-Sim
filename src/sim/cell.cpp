#include <sim/cell.hpp>

namespace sim {

sf::Color Cell::color() const
{
    static sf::Color s_colors[] = {
        sf::Color::Black,
        sf::Color::Red,
        sf::Color::Yellow,
        sf::Color::Blue,
    };
    return s_colors[d_value];
}

}  // close namespace sim
