#pragma once

#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include "../core/cave.h"

namespace {

std::string MakeTempFileName(const std::string& name) {
  return "test_" + name + ".txt";
}

void RemoveFile(const std::string& path) { std::remove(path.c_str()); }

bool IsBinaryMatrix(const std::vector<std::vector<int>>& cells) {
  for (const auto& row : cells) {
    for (int cell : row) {
      if (cell != 0 && cell != 1) {
        return false;
      }
    }
  }
  return true;
}

}  // namespace

TEST(CaveModel, DefaultStateIsEmpty) {
  s21::Cave cave;

  EXPECT_TRUE(cave.IsEmpty());
  EXPECT_EQ(cave.GetRows(), 0);
  EXPECT_EQ(cave.GetCols(), 0);
  EXPECT_TRUE(cave.GetCells().empty());
}

TEST(CaveModel, GenerateCaveCorrectSize) {
  s21::Cave cave;
  cave.GenerateCave(10, 15, 45);

  EXPECT_FALSE(cave.IsEmpty());
  EXPECT_EQ(cave.GetRows(), 10);
  EXPECT_EQ(cave.GetCols(), 15);
  EXPECT_EQ(cave.GetCells().size(), 10u);

  for (const auto& row : cave.GetCells()) {
    EXPECT_EQ(row.size(), 15u);
  }

  EXPECT_TRUE(IsBinaryMatrix(cave.GetCells()));
}

TEST(CaveModel, GenerateCaveMinSize) {
  s21::Cave cave;
  cave.GenerateCave(1, 1, 50);

  EXPECT_FALSE(cave.IsEmpty());
  EXPECT_EQ(cave.GetRows(), 1);
  EXPECT_EQ(cave.GetCols(), 1);
  ASSERT_EQ(cave.GetCells().size(), 1u);
  ASSERT_EQ(cave.GetCells()[0].size(), 1u);
  EXPECT_TRUE(cave.GetCells()[0][0] == 0 || cave.GetCells()[0][0] == 1);
}

TEST(CaveModel, GenerateCaveMaxSize) {
  s21::Cave cave;
  cave.GenerateCave(50, 50, 50);

  EXPECT_FALSE(cave.IsEmpty());
  EXPECT_EQ(cave.GetRows(), 50);
  EXPECT_EQ(cave.GetCols(), 50);
  ASSERT_EQ(cave.GetCells().size(), 50u);
  ASSERT_EQ(cave.GetCells()[0].size(), 50u);
  EXPECT_TRUE(IsBinaryMatrix(cave.GetCells()));
}

TEST(CaveModel, InvalidRowsDoNotCreateCave) {
  s21::Cave cave;
  cave.GenerateCave(0, 10, 50);

  EXPECT_TRUE(cave.IsEmpty());
  EXPECT_EQ(cave.GetRows(), 0);
  EXPECT_EQ(cave.GetCols(), 0);
}

TEST(CaveModel, InvalidColsDoNotCreateCave) {
  s21::Cave cave;
  cave.GenerateCave(10, 0, 50);

  EXPECT_TRUE(cave.IsEmpty());
  EXPECT_EQ(cave.GetRows(), 0);
  EXPECT_EQ(cave.GetCols(), 0);
}

TEST(CaveModel, TooLargeRowsDoNotCreateCave) {
  s21::Cave cave;
  cave.GenerateCave(51, 10, 50);

  EXPECT_TRUE(cave.IsEmpty());
}

TEST(CaveModel, TooLargeColsDoNotCreateCave) {
  s21::Cave cave;
  cave.GenerateCave(10, 51, 50);

  EXPECT_TRUE(cave.IsEmpty());
}

TEST(CaveModel, LoadFromFileSuccess) {
  std::string path = MakeTempFileName("cave_load_success");

  {
    std::ofstream out(path);
    out << "3 4\n";
    out << "0 1 0 1\n";
    out << "1 1 0 0\n";
    out << "0 0 1 0\n";
  }

  s21::Cave cave;
  EXPECT_TRUE(cave.LoadFromFile(path));

  EXPECT_EQ(cave.GetRows(), 3);
  EXPECT_EQ(cave.GetCols(), 4);

  const auto& cells = cave.GetCells();
  ASSERT_EQ(cells.size(), 3u);
  ASSERT_EQ(cells[0].size(), 4u);

  EXPECT_EQ(cells[0][0], 0);
  EXPECT_EQ(cells[0][1], 1);
  EXPECT_EQ(cells[1][0], 1);
  EXPECT_EQ(cells[1][1], 1);
  EXPECT_EQ(cells[2][2], 1);

  RemoveFile(path);
}

TEST(CaveModel, LoadFromFileFailsForMissingFile) {
  s21::Cave cave;
  EXPECT_FALSE(cave.LoadFromFile("missing_cave_file_12345.txt"));
}

TEST(CaveModel, LoadFromFileFailsForInvalidSize) {
  std::string path = MakeTempFileName("cave_invalid_size");

  {
    std::ofstream out(path);
    out << "0 3\n";
    out << "1 0 1\n";
  }

  s21::Cave cave;
  EXPECT_FALSE(cave.LoadFromFile(path));

  RemoveFile(path);
}

TEST(CaveModel, LoadFromFileFailsForInvalidCellValue) {
  std::string path = MakeTempFileName("cave_invalid_value");

  {
    std::ofstream out(path);
    out << "2 2\n";
    out << "0 1\n";
    out << "2 0\n";
  }

  s21::Cave cave;
  EXPECT_FALSE(cave.LoadFromFile(path));

  RemoveFile(path);
}

TEST(CaveModel, LoadFromFileFailsForIncompleteData) {
  std::string path = MakeTempFileName("cave_incomplete_data");

  {
    std::ofstream out(path);
    out << "2 3\n";
    out << "0 1 0\n";
    out << "1 0\n";
  }

  s21::Cave cave;
  EXPECT_FALSE(cave.LoadFromFile(path));

  RemoveFile(path);
}

TEST(CaveModel, SaveToFileSuccess) {
  std::string path = MakeTempFileName("cave_save_success");

  s21::Cave cave;
  cave.GenerateCave(5, 5, 50);

  EXPECT_TRUE(cave.SaveToFile(path));

  s21::Cave loaded;
  EXPECT_TRUE(loaded.LoadFromFile(path));

  EXPECT_EQ(loaded.GetRows(), cave.GetRows());
  EXPECT_EQ(loaded.GetCols(), cave.GetCols());
  EXPECT_EQ(loaded.GetCells(), cave.GetCells());

  RemoveFile(path);
}

TEST(CaveModel, SaveAndLoadPreserveData) {
  std::string path = MakeTempFileName("cave_save_load");

  s21::Cave cave;
  cave.GenerateCave(7, 9, 35);

  ASSERT_TRUE(cave.SaveToFile(path));

  s21::Cave loaded;
  ASSERT_TRUE(loaded.LoadFromFile(path));

  EXPECT_EQ(loaded.GetRows(), 7);
  EXPECT_EQ(loaded.GetCols(), 9);
  EXPECT_EQ(loaded.GetCells(), cave.GetCells());

  RemoveFile(path);
}

TEST(CaveModel, StepDoesNotBreakDimensions) {
  s21::Cave cave;
  cave.GenerateCave(10, 10, 45);

  int rows_before = cave.GetRows();
  int cols_before = cave.GetCols();

  cave.SetLimits(4, 3);
  cave.Step();

  EXPECT_EQ(cave.GetRows(), rows_before);
  EXPECT_EQ(cave.GetCols(), cols_before);
  EXPECT_EQ(cave.GetCells().size(), static_cast<size_t>(rows_before));

  for (const auto& row : cave.GetCells()) {
    EXPECT_EQ(row.size(), static_cast<size_t>(cols_before));
  }

  EXPECT_TRUE(IsBinaryMatrix(cave.GetCells()));
}

TEST(CaveModel, MultipleStepsKeepBinaryMatrix) {
  s21::Cave cave;
  cave.GenerateCave(20, 20, 45);
  cave.SetLimits(4, 3);

  for (int i = 0; i < 10; ++i) {
    cave.Step();
  }

  EXPECT_EQ(cave.GetRows(), 20);
  EXPECT_EQ(cave.GetCols(), 20);
  EXPECT_TRUE(IsBinaryMatrix(cave.GetCells()));
}

TEST(CaveModel, StepForEmptyCaveDoesNothing) {
  s21::Cave cave;
  cave.SetLimits(4, 3);
  cave.Step();

  EXPECT_TRUE(cave.IsEmpty());
  EXPECT_EQ(cave.GetRows(), 0);
  EXPECT_EQ(cave.GetCols(), 0);
}