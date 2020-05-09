#ifndef INCLUDED_SIM__GRID_DISPLAY
#define INCLUDED_SIM__GRID_DISPLAY

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <string>

namespace sim {

class Grid;

class GridDisplay
{
  public:
    GridDisplay(const Grid &grid);

    void draw(sf::RenderTarget &target);

    void update(int x, int y, sf::Color color);

    void save(const std::string &filename) const;

  private:
    sf::Image d_image;
    const Grid &d_grid;
};

}  // close namespace sim

#endif  // INCLUDED_SIM__GRID_DISPLAY
