#include <sim/grid.hpp>

#include <algorithm>

namespace sim {

Grid::Grid(int width, int height) : d_cells(width * height), d_width(width), d_height(height)
{
    d_display = std::make_unique<GridDisplay>(*this);
}

int Grid::width() const
{
    return d_width;
}
int Grid::height() const
{
    return d_height;
}

Cell Grid::getCell(int x, int y) const
{
    return d_cells[index(x, y)];
}
void Grid::setCell(int x, int y, Cell cell)
{
    d_cells[index(x, y)] = cell;
    d_display->update(x, y, cell.color());
}

void Grid::draw(sf::RenderTarget &target) const
{
    d_display->draw(target);
}

void Grid::updateRandomCell(Random &random)
{
    int idx = getRandomCellIndex(random);
    Cell cell = d_cells[idx];
    auto [x, y] = getXY(idx);

    int dir = random.randInt(0, 3);
    int changeX[] = {-1, 0, 1, 0};
    int changeY[] = {0, -1, 0, 1};
    int neighborX = x + changeX[dir];
    int neighborY = y + changeY[dir];
    if (neighborX == -1)
        neighborX = d_width - 1;
    else if (neighborX == d_width)
        neighborX = 0;
    if (neighborY == -1)
        neighborY = d_height - 1;
    else if (neighborY == d_height)
        neighborY = 0;

    Cell neighborCell = getCell(neighborX, neighborY);

    constexpr float select = 1.0f;
    constexpr float reproduce = 1.0f;
    constexpr float exchange = 0.05f;
    constexpr float total = select + reproduce + exchange;
    constexpr float selectionRate = select / total;
    constexpr float reproduceRate = reproduce / total;

    float actionValue = random.randFloat(0.0f, total);
    if (actionValue < selectionRate)
    {
        // selection
        // 1 kills 2, 2 kills 3, 3 kills 1
        if (!neighborCell.empty() && (cell.value() % 3) + 1 == neighborCell.value())
            setCell(neighborX, neighborY, Cell());
    }
    else if (actionValue < selectionRate + reproduceRate)
    {
        // reproduce
        if (neighborCell.empty())
            setCell(neighborX, neighborY, cell);
    }
    else
    {
        // exchange
        setCell(x, y, neighborCell);
        setCell(x, y, cell);
    }
}

void Grid::fillRandom(Random &random, int count)
{
    std::vector<std::pair<int, int>> coords;
    for (int x = 0; x < d_width; x++)
        for (int y = 0; y < d_height; y++)
            coords.push_back({x, y});

    std::vector<std::pair<int, int>> fillCoords;
    std::sample(
        coords.begin(), coords.end(), std::back_inserter(fillCoords), count, random.generator());

    for (auto [x, y] : fillCoords)
        setCell(x, y, Cell(random.randInt(1, 3)));
}

int Grid::index(int x, int y) const
{
    return y * d_width + x;
}
std::pair<int, int> Grid::getXY(int idx) const
{
    return {idx % d_width, idx / d_width};
}

int Grid::getRandomCellIndex(Random &random) const
{
    // FIXME inefficient when grid is sparse
    int idx;
    do
    {
        idx = random.randInt<int>(0, d_cells.size() - 1);
    } while (d_cells[idx].empty());
    return idx;
}

}  // close namespace sim
