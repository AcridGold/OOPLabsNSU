#include "Simulation.h"
#include <utility>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <unordered_set>

Simulation::Simulation(int width, int height, int cellSize)
    : grid(width, height, cellSize),
      tempGrid(width, height, cellSize),
      running(false)
{
    birth.fill(false);
    survival.fill(false);
    birth[3] = true;
    survival[2] = true;
    survival[3] = true;
}

void Simulation::Draw() const {
    grid.Draw();
}

void Simulation::Update() {
    if (!running) return;
    Step();
}

void Simulation::Step() {
    for (int row = 0; row < grid.GetRows(); row++) {
        for (int column = 0; column < grid.GetColumns(); column++) {
            int liveNeighbors = CountLiveNeighbors(row, column);
            int cellValue = grid.GetCellValue(row, column);

            if (cellValue == 1) {
                tempGrid.SetCellValue(row, column, survival[liveNeighbors] ? 1 : 0);
            } else {
                tempGrid.SetCellValue(row, column, birth[liveNeighbors] ? 1 : 0);
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

int Simulation::GetCellValue(int row, int column) const {
    return grid.GetCellValue(row, column);
}

// Trim both ends (safe for empty/all-space strings)
static std::string trim_copy(const std::string& s) {
    if (s.empty()) return std::string();
    size_t first = 0;
    while (first < s.size() && std::isspace(static_cast<unsigned char>(s[first]))) ++first;
    if (first == s.size()) return std::string();
    size_t last = s.size() - 1;
    while (last > first && std::isspace(static_cast<unsigned char>(s[last]))) --last;
    return s.substr(first, last - first + 1);
}

bool Simulation::LoadFromLife106(const std::string& filePath, std::vector<std::string>& warnings) {
    std::ifstream in(filePath);
    if (!in.is_open()) {
        warnings.push_back("Cannot open file: " + filePath);
        return false;
    }

    universeName.clear();
    bool hasName = false;
    bool hasRule = false;

    birth.fill(false);
    survival.fill(false);
    birth[3] = true;
    survival[2] = true;
    survival[3] = true;

    grid.Clear();
    tempGrid = grid;

    std::string line;
    int lineNo = 0;
    bool headerChecked = false;
    std::unordered_set<long long> placed;
    int centerRow = grid.GetRows() / 2;
    int centerCol = grid.GetColumns() / 2;

    while (std::getline(in, line)) {
        ++lineNo;
        std::string t = trim_copy(line);
        if (t.empty()) continue;

        if (!headerChecked) {
            headerChecked = true;
            if (t != "Life 1.06") {
                warnings.push_back("Missing or invalid header 'Life 1.06' at line " + std::to_string(lineNo) + "; found: '" + t + "'");
            }
        }

        if (!t.empty() && t[0] == '#') {
            //  #N or #R
            if (t.size() >= 2 && (t[1] == 'N' || t[1] == 'n')) {
                // #N <name>
                std::string name = t.size() > 2 ? trim_copy(t.substr(2)) : std::string();
                universeName = name;
                hasName = true;
            } else if (t.size() >= 2 && (t[1] == 'R' || t[1] == 'r')) {
                // #R Bx/Sy
                std::string rest = t.size() > 2 ? trim_copy(t.substr(2)) : std::string();
                rest.erase(std::remove_if(rest.begin(), rest.end(), ::isspace), rest.end());
                // Expect format with B
                size_t posB = rest.find_first_of("Bb");
                size_t posS = rest.find_first_of("Ss");
                if (posB == std::string::npos || posS == std::string::npos) {
                    warnings.push_back("Invalid #R rule at line " + std::to_string(lineNo) + ": '" + t + "'");
                } else {
                    std::string bpart = rest.substr(posB + 1, posS - (posB + 1));
                    std::string spart = rest.substr(posS + 1);
                    // reset rules
                    birth.fill(false);
                    survival.fill(false);
                    bool ok = true;
                    for (char c : bpart) {
                        if (c >= '0' && c <= '8') {
                            birth[c - '0'] = true;
                        } else {
                            ok = false;
                        }
                    }
                    for (char c : spart) {
                        if (c >= '0' && c <= '8') {
                            survival[c - '0'] = true;
                        } else {
                            ok = false;
                        }
                    }
                    if (!ok) {
                        warnings.push_back("Invalid digits in #R rule at line " + std::to_string(lineNo) + ": '" + t + "'");
                    } else {
                        hasRule = true;
                    }
                }
            }
            continue;
        }

        std::istringstream iss(t);
        int x, y;
        if (!(iss >> x >> y)) {
            warnings.push_back("Cannot parse coordinates at line " + std::to_string(lineNo) + ": '" + t + "'");
            continue;
        }

        int row = centerRow + y;
        int col = centerCol + x;

        if (!grid.IsWithinBounds(row, col)) {
            warnings.push_back("Coordinate out of bounds at line " + std::to_string(lineNo) + ": (" + std::to_string(x) + "," + std::to_string(y) + ") mapped to (" + std::to_string(row) + "," + std::to_string(col) + ")");
            continue;
        }

        long long key = (static_cast<long long>(row) << 32) | static_cast<unsigned int>(col);
        if (placed.find(key) != placed.end()) {
            warnings.push_back("Duplicate coordinate (same cell) at line " + std::to_string(lineNo) + ": (" + std::to_string(x) + "," + std::to_string(y) + ")");
            continue;
        }

        grid.SetCellValue(row, col, 1);
        placed.insert(key);
    }

    if (!hasName) {
        warnings.push_back("No universe name (#N) found in file");
    }
    if (!hasRule) {
        warnings.push_back("No rule (#R Bx/Sy) found in file — defaulting to B3/S23");
    }

    tempGrid = grid;

    return true;
}

bool Simulation::SaveToLife106(const std::string& outPath, std::string* err) const {
    std::ofstream out(outPath);
    if (!out.is_open()) {
        if (err) *err = "Cannot open output file: " + outPath;
        return false;
    }

    out << "Life 1.06\n";
    if (!universeName.empty()) {
        out << "#N " << universeName << "\n";
    }
    // write rule
    out << "#R B";
    for (int i = 0; i <= 8; ++i) if (birth[i]) out << i;
    out << "/S";
    for (int i = 0; i <= 8; ++i) if (survival[i]) out << i;
    out << "\n";

    int centerRow = grid.GetRows() / 2;
    int centerCol = grid.GetColumns() / 2;
    for (int r = 0; r < grid.GetRows(); ++r) {
        for (int c = 0; c < grid.GetColumns(); ++c) {
            if (grid.GetCellValue(r, c) != 0) {
                int x = c - centerCol;
                int y = r - centerRow;
                out << x << " " << y << "\n";
            }
        }
    }

    return true;
}
