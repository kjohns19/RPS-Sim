#include <sim/cell.hpp>

#include <iterator>

namespace sim {

sf::Color Cell::color() const
{
    static sf::Color s_colors[] = {
        sf::Color::Black,
        sf::Color::Red,
        sf::Color::Yellow,
        sf::Color::Blue,
    };
    static_assert(std::size(s_colors) == CELL_TYPES + 1);
    return s_colors[d_value];
}

}  // close namespace sim
