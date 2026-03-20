#pragma once
#include <gtest/gtest.h>

#include <queue>
#include <utility>
#include <vector>
#include <cstdio>
#include <fstream>
#include <string>

#include "../core/maze.h"

namespace {

int CountEdges(const s21::Maze& maze) {
    int rows = maze.GetRows();
    int cols = maze.GetCols();
    const auto& right = maze.GetRightWalls();
    const auto& bottom = maze.GetBottomWalls();

    int edges = 0;

    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        if (j + 1 < cols && right[i][j] == 0) {
          ++edges;
        }
        if (i + 1 < rows && bottom[i][j] == 0) {
          ++edges;
        }
      }
    }

    return edges;
}

int CountReachableCells(const s21::Maze& maze, int start_r, int start_c) {
    int rows = maze.GetRows();
    int cols = maze.GetCols();

    std::queue<std::pair<int, int>> q;
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

    q.push({start_r, start_c});
    visited[start_r][start_c] = true;

    int count = 0;

    while (!q.empty()) {
      auto current = q.front();
      q.pop();
      ++count;

      auto neighbours = maze.GetNeighbours(current.first, current.second);
      for (const auto& next : neighbours) {
        if (!visited[next.first][next.second]) {
          visited[next.first][next.second] = true;
          q.push(next);
        }
      }
    }

    return count;
  }
}

TEST(MazeGenerator, CorrectRowsAndColsAfterGeneration) {
    s21::Maze maze;
    maze.GenerateMaze(10, 15);

    EXPECT_EQ(maze.GetRows(), 10);
    EXPECT_EQ(maze.GetCols(), 15);
}

TEST(MazeGenerator, RightWallsMatrixHasCorrectSize) {
    s21::Maze maze;
    maze.GenerateMaze(6, 7);

    const auto& right = maze.GetRightWalls();
    ASSERT_EQ(right.size(), 6u);

    for (const auto& row : right) {
      EXPECT_EQ(row.size(), 7u);
    }
}

TEST(MazeGenerator, BottomWallsMatrixHasCorrectSize) {
    s21::Maze maze;
    maze.GenerateMaze(6, 7);

    const auto& bottom = maze.GetBottomWalls();
    ASSERT_EQ(bottom.size(), 6u);

    for (const auto& row : bottom) {
      EXPECT_EQ(row.size(), 7u);
    }
}

TEST(MazeGenerator, LastColumnHasRightBorder) {
    s21::Maze maze;
    maze.GenerateMaze(10, 10);

    const auto& right = maze.GetRightWalls();
    for (int i = 0; i < maze.GetRows(); ++i) {
      EXPECT_EQ(right[i][maze.GetCols() - 1], 1);
    }
}

TEST(MazeGenerator, LastRowHasBottomBorder) {
    s21::Maze maze;
    maze.GenerateMaze(10, 10);

    const auto& bottom = maze.GetBottomWalls();
    for (int j = 0; j < maze.GetCols(); ++j) {
      EXPECT_EQ(bottom[maze.GetRows() - 1][j], 1);
    }
}

TEST(MazeGenerator, MazeIsConnected) {
    s21::Maze maze;
    maze.GenerateMaze(10, 10);

    int reachable = CountReachableCells(maze, 0, 0);
    EXPECT_EQ(reachable, maze.GetRows() * maze.GetCols());
}

TEST(MazeGenerator, PerfectMazeHasVerticesMinusOneEdges) {
    s21::Maze maze;
    maze.GenerateMaze(10, 10);

    int vertices = maze.GetRows() * maze.GetCols();
    int edges = CountEdges(maze);

    EXPECT_EQ(edges, vertices - 1);
}

TEST(MazeGenerator, OneByOneMazeIsValid) {
    s21::Maze maze;
    maze.GenerateMaze(1, 1);

    EXPECT_EQ(maze.GetRows(), 1);
    EXPECT_EQ(maze.GetCols(), 1);
    EXPECT_EQ(maze.GetRightWalls().size(), 1u);
    EXPECT_EQ(maze.GetBottomWalls().size(), 1u);
    EXPECT_EQ(maze.GetRightWalls()[0].size(), 1u);
    EXPECT_EQ(maze.GetBottomWalls()[0].size(), 1u);
}

TEST(MazeGenerator, TwoByTwoMazeIsConnected) {
	  s21::Maze maze;
	  maze.GenerateMaze(2, 2);

	  int reachable = CountReachableCells(maze, 0, 0);
	  EXPECT_EQ(reachable, 4);
}

TEST(MazeGenerator, LargeMazeGenerationWorks) {
	  s21::Maze maze;
	  maze.GenerateMaze(50, 50);

	  EXPECT_EQ(maze.GetRows(), 50);
	  EXPECT_EQ(maze.GetCols(), 50);
	  EXPECT_EQ(CountReachableCells(maze, 0, 0), 2500);
}

TEST(MazeGenerator, NewMazeIsEmpty) {
    s21::Maze maze;
    EXPECT_TRUE(maze.IsEmpty());
}

TEST(MazeGenerator, GeneratedMazeIsNotEmpty) {
    s21::Maze maze;
    maze.GenerateMaze(3, 3);
    EXPECT_FALSE(maze.IsEmpty());
}

TEST(MazeGenerator, SaveToFileReturnsTrueForGeneratedMaze) {
    s21::Maze maze;
    maze.GenerateMaze(4, 4);

    const std::string filename = "maze_generator_save_test.txt";
    EXPECT_TRUE(maze.SaveToFile(filename));

    std::remove(filename.c_str());
}

TEST(MazeGenerator, LoadFromFilePreservesDimensions) {
    s21::Maze maze;
    maze.GenerateMaze(7, 9);

    const std::string filename = "maze_generator_load_dims.txt";
    ASSERT_TRUE(maze.SaveToFile(filename));

    s21::Maze loaded;
    ASSERT_TRUE(loaded.LoadFromFile(filename));

    EXPECT_EQ(loaded.GetRows(), 7);
    EXPECT_EQ(loaded.GetCols(), 9);

    std::remove(filename.c_str());
}

TEST(MazeGenerator, LoadFromFilePreservesWalls) {
    s21::Maze maze;
    maze.GenerateMaze(5, 5);

    const std::string filename = "maze_generator_load_walls.txt";
    ASSERT_TRUE(maze.SaveToFile(filename));

    s21::Maze loaded;
    ASSERT_TRUE(loaded.LoadFromFile(filename));

    EXPECT_EQ(maze.GetRightWalls(), loaded.GetRightWalls());
    EXPECT_EQ(maze.GetBottomWalls(), loaded.GetBottomWalls());

    std::remove(filename.c_str());
}

TEST(MazeGenerator, GenerateOneByManyMaze) {
    s21::Maze maze;
    maze.GenerateMaze(1, 5);

    EXPECT_EQ(maze.GetRows(), 1);
    EXPECT_EQ(maze.GetCols(), 5);
    ASSERT_EQ(maze.GetRightWalls().size(), 1u);
    ASSERT_EQ(maze.GetBottomWalls().size(), 1u);
    EXPECT_EQ(maze.GetRightWalls()[0].size(), 5u);
    EXPECT_EQ(maze.GetBottomWalls()[0].size(), 5u);
}

TEST(MazeGenerator, GenerateManyByOneMaze) {
    s21::Maze maze;
    maze.GenerateMaze(5, 1);

    EXPECT_EQ(maze.GetRows(), 5);
    EXPECT_EQ(maze.GetCols(), 1);
    ASSERT_EQ(maze.GetRightWalls().size(), 5u);
    ASSERT_EQ(maze.GetBottomWalls().size(), 5u);

    for (size_t i = 0; i < 5; ++i) {
      EXPECT_EQ(maze.GetRightWalls()[i].size(), 1u);
      EXPECT_EQ(maze.GetBottomWalls()[i].size(), 1u);
    }
}

TEST(MazeGenerator, SingleCellMazeHasNoNeighbours) {
    s21::Maze maze;
    maze.GenerateMaze(1, 1);

    auto neighbours = maze.GetNeighbours(0, 0);
    EXPECT_TRUE(neighbours.empty());
}

TEST(MazeGenerator, LoadFromFileMakesMazeNotEmpty) {
    const std::string filename = "maze_generator_manual_load.txt";

    {
      std::ofstream file(filename);
      file << "2 2\n";
      file << "0 1\n";
      file << "0 1\n";
      file << "\n";
      file << "0 0\n";
      file << "1 1\n";
    }

    s21::Maze maze;
    ASSERT_TRUE(maze.LoadFromFile(filename));
    EXPECT_FALSE(maze.IsEmpty());

    std::remove(filename.c_str());
}

TEST(MazeGenerator, LoadedMazeIsConnected) {
    const std::string filename = "maze_generator_connected_load.txt";

    {
      std::ofstream file(filename);
      file << "4 4\n";
      file << "0 0 0 1\n";
      file << "1 0 1 1\n";
      file << "0 1 0 1\n";
      file << "0 0 0 1\n";
      file << "\n";
      file << "1 0 1 0\n";
      file << "0 0 1 0\n";
      file << "1 1 0 1\n";
      file << "1 1 1 1\n";
    }

    s21::Maze maze;
    ASSERT_TRUE(maze.LoadFromFile(filename));

    int reachable = CountReachableCells(maze, 0, 0);
    EXPECT_EQ(reachable, maze.GetRows() * maze.GetCols());

    std::remove(filename.c_str());
}