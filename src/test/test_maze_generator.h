#pragma once
#include <gtest/gtest.h>

#include <queue>
#include <utility>
#include <vector>

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