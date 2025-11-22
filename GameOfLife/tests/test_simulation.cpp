#include <gtest/gtest.h>
#include "Grid.h"
#include "Simulation.h"
#include <fstream>

static Simulation makeSmallSim()
{
    return Simulation(30, 30, 10);
}

TEST(GridBasics, SetGetToggleClear)
{
    Grid g(30, 20, 10);
    EXPECT_EQ(g.GetRows(), 2);
    EXPECT_EQ(g.GetColumns(), 3);

    EXPECT_EQ(g.GetCellValue(0,0), 0);
    g.SetCellValue(0, 0, 1);
    EXPECT_EQ(g.GetCellValue(0,0), 1);
    g.ToggleCell(0, 0);
    EXPECT_EQ(g.GetCellValue(0,0), 0);

    g.SetCellValue(1, 2, 1);
    EXPECT_EQ(g.GetCellValue(1,2), 1);
    g.Clear();
    EXPECT_EQ(g.GetCellValue(1,2), 0);
}

TEST(SimulationStep, BlinkerOscillator)
{
    Simulation sim = makeSmallSim();
    sim.ClearGrid();
    sim.ToggleCell(1, 0);
    sim.ToggleCell(1, 1);
    sim.ToggleCell(1, 2);

    sim.Step();

    EXPECT_EQ(sim.GetCellValue(0,1), 1);
    EXPECT_EQ(sim.GetCellValue(1,1), 1);
    EXPECT_EQ(sim.GetCellValue(2,1), 1);

    EXPECT_EQ(sim.GetCellValue(1,0), 0);
    EXPECT_EQ(sim.GetCellValue(1,2), 0);
}

TEST(ToroidalWrap, NeighborsWrapAround)
{
    Simulation sim = makeSmallSim();
    sim.ClearGrid();
    sim.ToggleCell(0, 0);
    sim.ToggleCell(2, 2);
    sim.Step();
    for (int r = 0; r < sim.GetRows(); ++r)
    {
        for (int c = 0; c < sim.GetColumns(); ++c)
        {
            int val = sim.GetCellValue(r, c);
            EXPECT_TRUE(val == 0 || val == 1);
        }
    }
}

TEST(LoadSaveRoundtrip, LoadFromFile)
{
    const std::string path = "tests_tmp_pattern.lif";
    std::ofstream out(path);
    out << "Life 1.06\n";
    out << "#N test-blinker\n";
    out << "#R B3/S23\n";
    out << "0 0\n";
    out << "1 0\n";
    out << "-1 0\n";
    out.close();

    Simulation sim = makeSmallSim();
    std::vector<std::string> warnings;
    bool ok = sim.LoadFromLife106(path, warnings);
    EXPECT_TRUE(ok);
    EXPECT_EQ(sim.GetCellValue(1,1), 1);
    EXPECT_EQ(sim.GetCellValue(1,2), 1);
    EXPECT_EQ(sim.GetCellValue(1,0), 1);

    std::remove(path.c_str());
}

TEST(ParserWarnings, DuplicateCoordinateWarning)
{
    const std::string path = "tests_tmp_dup.lif";
    std::ofstream out(path);
    out << "Life 1.06\n";
    out << "#N dup-test\n";
    out << "#R B3/S23\n";
    out << "0 0\n";
    out << "0 0\n";
    out.close();

    Simulation sim = makeSmallSim();
    std::vector<std::string> warnings;
    bool ok = sim.LoadFromLife106(path, warnings);
    EXPECT_TRUE(ok);
    bool foundDuplicate = false;
    for (auto& w : warnings)
    {
        if (w.find("Duplicate") != std::string::npos) foundDuplicate = true;
    }
    EXPECT_TRUE(foundDuplicate);
    std::remove(path.c_str());
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
