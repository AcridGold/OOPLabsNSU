#pragma once
#include "Grid.h"
#include <string>
#include <vector>
#include <array>

class Simulation
{
public:
    Simulation(int width, int height, int cellSize);

    void Draw() const;
    void Update();
    void Step();
    void ClearGrid();
    void CreateRandomState();
    void ToggleCell(int row, int column);
    void Start() { running = true; }
    void Stop() { running = false; }
    bool IsRunning() const { return running; }

    bool LoadFromLife106(const std::string& filePath, std::vector<std::string>& warnings);
    bool SaveToLife106(const std::string& outPath, std::string* err = nullptr) const;

    const std::string& GetUniverseName() const { return universeName; }

    // helpers for tests
    int GetCellValue(int row, int column) const;
    int GetRows() const { return grid.GetRows(); }
    int GetColumns() const { return grid.GetColumns(); }

private:
    int CountLiveNeighbors(int row, int column) const;

    Grid grid;
    Grid tempGrid;
    bool running;

    // birth[n] == true => dead cell with n neighbors becomes alive
    // survival[n] == true => live cell with n neighbors survives
    std::array<bool, 9> birth{};
    std::array<bool, 9> survival{};

    // Optional name parsed from #N comment
    std::string universeName;
};
