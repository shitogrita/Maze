#pragma once
#include <gtest/gtest.h>

#include <cstdio>
#include <queue>
#include <string>
#include <utility>
#include <vector>

#include "../core/maze.h"

namespace {

bool AreAdjacent(const std::pair<int, int>& a, const std::pair<int, int>& b) {
  int dr = std::abs(a.first - b.first);
  int dc = std::abs(a.second - b.second);
  return dr + dc == 1;
}

bool CanMove(const s21::Maze& maze,
             const std::pair<int, int>& from,
             const std::pair<int, int>& to) {
  const auto& right = maze.GetRightWalls();
  const auto& bottom = maze.GetBottomWalls();

  int r1 = from.first;
  int c1 = from.second;
  int r2 = to.first;
  int c2 = to.second;

  if (!AreAdjacent(from, to)) {
    return false;
  }

  if (r1 == r2) {
    if (c2 == c1 + 1) {
      return right[r1][c1] == 0;
    }
    if (c2 == c1 - 1) {
      return right[r1][c2] == 0;
    }
  }

  if (c1 == c2) {
    if (r2 == r1 + 1) {
      return bottom[r1][c1] == 0;
    }
    if (r2 == r1 - 1) {
      return bottom[r2][c1] == 0;
    }
  }

  return false;
}

void ExpectValidPath(const s21::Maze& maze,
                     const std::vector<std::pair<int, int>>& path,
                     const std::pair<int, int>& start,
                     const std::pair<int, int>& finish) {
  ASSERT_FALSE(path.empty());
  EXPECT_EQ(path.front(), start);
  EXPECT_EQ(path.back(), finish);

  for (size_t i = 0; i + 1 < path.size(); ++i) {
    EXPECT_TRUE(CanMove(maze, path[i], path[i + 1]));
  }
}

}  // namespace

TEST(MazeSolver, EmptyMazeReturnsEmptyPath) {
  s21::Maze maze;
  auto path = maze.Bfs({0, 0}, {0, 0});
  EXPECT_TRUE(path.empty());
}

TEST(MazeSolver, InvalidStartReturnsEmptyPath) {
  s21::Maze maze;
  maze.GenerateMaze(4, 4);

  auto path = maze.Bfs({-1, 0}, {3, 3});
  EXPECT_TRUE(path.empty());
}

TEST(MazeSolver, InvalidFinishReturnsEmptyPath) {
  s21::Maze maze;
  maze.GenerateMaze(4, 4);

  auto path = maze.Bfs({0, 0}, {4, 4});
  EXPECT_TRUE(path.empty());
}

TEST(MazeSolver, SameStartAndFinish) {
  s21::Maze maze;
  maze.GenerateMaze(4, 4);

  auto path = maze.Bfs({2, 2}, {2, 2});

  ASSERT_EQ(path.size(), 1u);
  EXPECT_EQ(path[0], std::make_pair(2, 2));
}

TEST(MazeSolver, FindsPathInGeneratedMaze) {
  s21::Maze maze;
  maze.GenerateMaze(10, 10);

  auto path = maze.Bfs({0, 0}, {9, 9});
  ExpectValidPath(maze, path, {0, 0}, {9, 9});
}

TEST(MazeSolver, FindsPathFromCornerToCorner) {
  s21::Maze maze;
  maze.GenerateMaze(5, 5);

  auto path = maze.Bfs({0, 4}, {4, 0});
  ExpectValidPath(maze, path, {0, 4}, {4, 0});
}

TEST(MazeSolver, EveryStepInPathIsLegal) {
  s21::Maze maze;
  maze.GenerateMaze(8, 8);

  auto path = maze.Bfs({0, 0}, {7, 7});
  ASSERT_FALSE(path.empty());

  for (size_t i = 0; i + 1 < path.size(); ++i) {
    EXPECT_TRUE(AreAdjacent(path[i], path[i + 1]));
    EXPECT_TRUE(CanMove(maze, path[i], path[i + 1]));
  }
}

TEST(MazeSolver, InvalidStartRowTooLargeReturnsEmptyPath) {
  s21::Maze maze;
  maze.GenerateMaze(5, 5);

  auto path = maze.Bfs({5, 0}, {4, 4});
  EXPECT_TRUE(path.empty());
}

TEST(MazeSolver, InvalidStartColTooLargeReturnsEmptyPath) {
  s21::Maze maze;
  maze.GenerateMaze(5, 5);

  auto path = maze.Bfs({0, 5}, {4, 4});
  EXPECT_TRUE(path.empty());
}

TEST(MazeSolver, InvalidFinishNegativeReturnsEmptyPath) {
  s21::Maze maze;
  maze.GenerateMaze(5, 5);

  auto path = maze.Bfs({0, 0}, {-1, 3});
  EXPECT_TRUE(path.empty());
}

TEST(MazeSolver, InvalidFinishColTooLargeReturnsEmptyPath) {
  s21::Maze maze;
  maze.GenerateMaze(5, 5);

  auto path = maze.Bfs({0, 0}, {4, 5});
  EXPECT_TRUE(path.empty());
}

TEST(MazeSolver, OneByOneMazePathIsSingleCell) {
  s21::Maze maze;
  maze.GenerateMaze(1, 1);

  auto path = maze.Bfs({0, 0}, {0, 0});

  ASSERT_EQ(path.size(), 1u);
  EXPECT_EQ(path.front(), std::make_pair(0, 0));
  EXPECT_EQ(path.back(), std::make_pair(0, 0));
}

TEST(MazeSolver, SaveAndLoadPreservePathExistence) {
  s21::Maze maze;
  maze.GenerateMaze(6, 6);

  const std::string filename = "maze_solver_io_test.txt";

  ASSERT_TRUE(maze.SaveToFile(filename));

  s21::Maze loaded;
  ASSERT_TRUE(loaded.LoadFromFile(filename));

  auto path = loaded.Bfs({0, 0}, {5, 5});
  ASSERT_FALSE(path.empty());
  EXPECT_EQ(path.front(), std::make_pair(0, 0));
  EXPECT_EQ(path.back(), std::make_pair(5, 5));

  std::remove(filename.c_str());
}

TEST(MazeSolver, LoadFromFileReturnsFalseForMissingFile) {
  s21::Maze maze;
  EXPECT_FALSE(maze.LoadFromFile("definitely_missing_maze_file.txt"));
}