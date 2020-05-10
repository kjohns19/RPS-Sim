#ifndef INCLUDED_SIM__CELL
#define INCLUDED_SIM__CELL

#include <SFML/Graphics/Color.hpp>

namespace sim {

class Cell
{
  public:
    static constexpr int CELL_TYPES = 3;

    Cell() : d_value(0) {}
    explicit Cell(unsigned char value) : d_value(value) {}

    sf::Color color() const;

    bool empty() const;

    int value() const;

  private:
    unsigned char d_value;
};

inline bool Cell::empty() const
{
    return d_value == 0;
}

inline int Cell::value() const
{
    return d_value;
}

}  // close namespace sim

#endif  // INCLUDED_SIM__CELL
