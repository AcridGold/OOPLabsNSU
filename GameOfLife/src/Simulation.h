#pragma once
#include "Grid.h"

class Simulation {
public:
    Simulation(int width, int height, int cellSize);
    
    void Draw() const;
    void Update();
    void ClearGrid();
    void CreateRandomState();
    void ToggleCell(int row, int column);
    void Start() { running = true; }
    void Stop() { running = false; }
    bool IsRunning() const { return running; }
    
private:
    int CountLiveNeighbors(int row, int column) const;
    
    Grid grid;
    Grid tempGrid;
    bool running;
};
