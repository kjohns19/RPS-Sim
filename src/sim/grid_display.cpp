#include <sim/grid_display.hpp>

#include <sim/grid.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace sim {

GridDisplay::GridDisplay(const Grid &grid) : d_grid(grid)
{
    d_image.create(grid.width(), grid.height());
    for (int x = 0; x < grid.width(); x++)
        for (int y = 0; y < grid.height(); y++)
            update(x, y, grid.getCell(x, y).color());
}

void GridDisplay::draw(sf::RenderTarget &target)
{
    sf::Texture texture;
    texture.loadFromImage(d_image);

    sf::Sprite sprite;
    sprite.setTexture(texture);

    target.draw(sprite);
}

void GridDisplay::update(int x, int y, sf::Color color)
{
    d_image.setPixel(x, y, color);
}

}  // close namespace sim
