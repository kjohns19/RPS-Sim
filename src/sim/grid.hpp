#ifndef INCLUDED_SIM__GRID
#define INCLUDED_SIM__GRID

#include <sim/cell.hpp>
#include <sim/grid_display.hpp>
#include <sim/random.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <array>
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
    static constexpr int UPDATE_SIZE = 50;

    void commitUpdates();
    void applyUpdates();

    void updateRandomCell(Random &random);

    int index(int x, int y) const;
    std::pair<int, int> getXY(int idx) const;
    int getRandomCellIndex(Random &random) const;

    struct UpdateData
    {
        int x;
        int y;
        sf::Color color;
    };

    std::mutex d_updateMutex;
    std::array<UpdateData, UPDATE_SIZE> d_updates;
    int d_updatePos;

    std::vector<UpdateData> d_allUpdates;

    std::unique_ptr<GridDisplay> d_display;
    std::vector<Cell> d_cells;
    int d_width;
    int d_height;
};

}  // close namespace sim

#endif  // INCLUDED_SIM__GRID
