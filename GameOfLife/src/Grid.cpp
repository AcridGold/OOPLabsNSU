#include "Grid.h"
#include "raylib.h"
#include <random>

Grid::Grid(int width, int height, int cellSize)
    : cellSize(cellSize)
{
    rows = height / cellSize;
    columns = width / cellSize;
    cells.resize(rows, std::vector<int>(columns, 0));
}

void Grid::Draw() const
{
    for (int row = 0; row < rows; row++)
    {
        for (int column = 0; column < columns; column++)
        {
            Color color = cells[row][column] ? GREEN : Color{55, 55, 55, 255};
            DrawRectangle(column * cellSize, row * cellSize, cellSize - 1, cellSize - 1, color);
        }
    }
}

void Grid::SetCellValue(int row, int column, int value)
{
    if (IsWithinBounds(row, column))
    {
        cells[row][column] = value;
    }
}

int Grid::GetCellValue(int row, int column) const
{
    if (IsWithinBounds(row, column))
    {
        return cells[row][column];
    }
    return 0;
}

bool Grid::IsWithinBounds(int row, int column) const
{
    return row >= 0 && row < rows && column >= 0 && column < columns;
}

void Grid::FillRandom()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 3);

    for (int row = 0; row < rows; row++)
    {
        for (int column = 0; column < columns; column++)
        {
            cells[row][column] = (dis(gen) == 0) ? 1 : 0;
        }
    }
}

void Grid::Clear()
{
    for (auto& row : cells)
    {
        std::fill(row.begin(), row.end(), 0);
    }
}

void Grid::ToggleCell(int row, int column)
{
    if (IsWithinBounds(row, column))
    {
        cells[row][column] = !cells[row][column];
    }
}
