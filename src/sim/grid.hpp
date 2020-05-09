#ifndef INCLUDED_SIM__GRID
#define INCLUDED_SIM__GRID

#include <sim/cell.hpp>
#include <sim/grid_display.hpp>
#include <sim/random.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

#include <memory>
#include <mutex>
#include <utility>
#include <vector>

namespace sim {

class Grid
{
  public:
    Grid(int width, int height);

    int width() const;
    int height() const;

    Cell getCell(int x, int y) const;
    void setCell(int x, int y, Cell cell);

    void draw(sf::RenderTarget &target);

    void iterate(Random &random);

    void fillRandom(Random &random, int count);

  private:
    void updateRandomCell(Random &random);

    int index(int x, int y) const;
    std::pair<int, int> getXY(int idx) const;
    int getRandomCellIndex(Random &random) const;

    std::mutex d_updateMutex;
    std::unique_ptr<GridDisplay> d_display;
    std::vector<Cell> d_cells;
    int d_width;
    int d_height;
};

}  // close namespace sim

#endif  // INCLUDED_SIM__GRID
