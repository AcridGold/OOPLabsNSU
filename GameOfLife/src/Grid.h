#pragma once
#include <vector>

class Grid
{
public:
    Grid(int width, int height, int cellSize);

    void Draw() const;
    void SetCellValue(int row, int column, int value);
    int GetCellValue(int row, int column) const;
    bool IsWithinBounds(int row, int column) const;
    void FillRandom();
    void Clear();
    void ToggleCell(int row, int column);

    int GetRows() const { return rows; }
    int GetColumns() const { return columns; }

private:
    int rows;
    int columns;
    int cellSize;
    std::vector<std::vector<int>> cells;
};
