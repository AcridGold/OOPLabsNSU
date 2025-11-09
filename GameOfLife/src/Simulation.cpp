#include "Simulation.h"
#include <utility>

Simulation::Simulation(int width, int height, int cellSize)
    : grid(width, height, cellSize),
      tempGrid(width, height, cellSize),
      running(false)
{
}

void Simulation::Draw() const {
    grid.Draw();
}

void Simulation::Update() {
    if (!running) return;
    
    for (int row = 0; row < grid.GetRows(); row++) {
        for (int column = 0; column < grid.GetColumns(); column++) {
            int liveNeighbors = CountLiveNeighbors(row, column);
            int cellValue = grid.GetCellValue(row, column);
            
            // rules
            if (cellValue == 1) {
                // Cell is alive
                if (liveNeighbors < 2 || liveNeighbors > 3) {
                    tempGrid.SetCellValue(row, column, 0);
                } else {
                    tempGrid.SetCellValue(row, column, 1);
                }
            } else {
                // Cell is dead
                if (liveNeighbors == 3) {
                    tempGrid.SetCellValue(row, column, 1);
                } else {
                    tempGrid.SetCellValue(row, column, 0);
                }
            }
        }
    }
    
    grid = tempGrid;
}

int Simulation::CountLiveNeighbors(int row, int column) const {
    int liveNeighbors = 0;
    
    // Check all 8 neighbors
    std::pair<int, int> neighborOffsets[] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };
    
    for (const auto& offset : neighborOffsets) {
        int neighborRow = (row + offset.first + grid.GetRows()) % grid.GetRows();
        int neighborColumn = (column + offset.second + grid.GetColumns()) % grid.GetColumns();
        liveNeighbors += grid.GetCellValue(neighborRow, neighborColumn);
    }
    
    return liveNeighbors;
}

void Simulation::ClearGrid() {
    grid.Clear();
}

void Simulation::CreateRandomState() {
    grid.FillRandom();
}

void Simulation::ToggleCell(int row, int column) {
    grid.ToggleCell(row, column);
}
