#pragma once

#include <gtest/gtest.h>

#include <cmath>
#include <utility>
#include <vector>

#include "../core/maze.h"
#include "../ml/MazeQSolver.h"

inline bool AreAdjacentQ(const std::pair<int, int>& a,
                         const std::pair<int, int>& b) {
  int dr = std::abs(a.first - b.first);
  int dc = std::abs(a.second - b.second);
  return dr + dc == 1;
}

inline bool CanMoveQ(const s21::Maze& maze, const std::pair<int, int>& from,
                     const std::pair<int, int>& to) {
  const auto& right = maze.GetRightWalls();
  const auto& bottom = maze.GetBottomWalls();

  int r1 = from.first;
  int c1 = from.second;
  int r2 = to.first;
  int c2 = to.second;

  if (!AreAdjacentQ(from, to)) {
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

inline void ExpectValidRouteQ(const s21::Maze& maze,
                              const std::vector<std::pair<int, int>>& route,
                              const std::pair<int, int>& start,
                              const std::pair<int, int>& goal) {
  ASSERT_FALSE(route.empty());
  EXPECT_EQ(route.front(), start);
  EXPECT_EQ(route.back(), goal);

  for (size_t i = 0; i + 1 < route.size(); ++i) {
    EXPECT_TRUE(CanMoveQ(maze, route[i], route[i + 1]));
  }
}

TEST(MazeQSolverTest, NotReadyWithoutMaze) {
  s21::MazeQSolver solver;
  EXPECT_FALSE(solver.IsReady());
}

TEST(MazeQSolverTest, NotReadyWithoutGoal) {
  s21::Maze maze;
  maze.GenerateMaze(5, 5);

  s21::MazeQSolver solver;
  solver.SetMaze(maze);

  EXPECT_FALSE(solver.IsReady());
}

TEST(MazeQSolverTest, ReadyAfterMazeAndGoal) {
  s21::Maze maze;
  maze.GenerateMaze(5, 5);

  s21::MazeQSolver solver;
  solver.SetMaze(maze);
  solver.SetGoal({4, 4});

  EXPECT_TRUE(solver.IsReady());
}

TEST(MazeQSolverTest, ValidCellCheck) {
  s21::Maze maze;
  maze.GenerateMaze(4, 4);

  s21::MazeQSolver solver;
  solver.SetMaze(maze);
  solver.SetGoal({3, 3});

  EXPECT_TRUE(solver.IsValidCell(0, 0));
  EXPECT_TRUE(solver.IsValidCell(3, 3));
  EXPECT_FALSE(solver.IsValidCell(-1, 0));
  EXPECT_FALSE(solver.IsValidCell(0, -1));
  EXPECT_FALSE(solver.IsValidCell(4, 0));
  EXPECT_FALSE(solver.IsValidCell(0, 4));
}

TEST(MazeQSolverTest, GoalCheckWorks) {
  s21::Maze maze;
  maze.GenerateMaze(4, 4);

  s21::MazeQSolver solver;
  solver.SetMaze(maze);
  solver.SetGoal({2, 1});

  EXPECT_TRUE(solver.IsGoal(2, 1));
  EXPECT_FALSE(solver.IsGoal(1, 2));
}

TEST(MazeQSolverTest, RewardForGoalAndNormalCell) {
  s21::Maze maze;
  maze.GenerateMaze(4, 4);

  s21::MazeQSolver solver;
  solver.SetMaze(maze);
  solver.SetGoal({3, 3});

  EXPECT_DOUBLE_EQ(solver.GetReward(0, 0), -1.0);
  EXPECT_DOUBLE_EQ(solver.GetReward(3, 3), 1000.0);
}

TEST(MazeQSolverTest, ApplyActionWorks) {
  s21::MazeQSolver solver;

  EXPECT_EQ(solver.ApplyAction(3, 3, 0), std::make_pair(2, 3));
  EXPECT_EQ(solver.ApplyAction(3, 3, 1), std::make_pair(3, 4));
  EXPECT_EQ(solver.ApplyAction(3, 3, 2), std::make_pair(4, 3));
  EXPECT_EQ(solver.ApplyAction(3, 3, 3), std::make_pair(3, 2));
}

TEST(MazeQSolverTest, InitialQValuesAreZero) {
  s21::Maze maze;
  maze.GenerateMaze(3, 3);

  s21::MazeQSolver solver;
  solver.SetMaze(maze);
  solver.SetGoal({2, 2});

  EXPECT_DOUBLE_EQ(solver.GetQValue(0, 0, 0), 0.0);
  EXPECT_DOUBLE_EQ(solver.GetQValue(0, 0, 1), 0.0);
  EXPECT_DOUBLE_EQ(solver.GetQValue(0, 0, 2), 0.0);
  EXPECT_DOUBLE_EQ(solver.GetQValue(0, 0, 3), 0.0);
}

TEST(MazeQSolverTest, BuildRouteReturnsEmptyIfNotReady) {
  s21::MazeQSolver solver;
  auto route = solver.BuildRoute({0, 0});
  EXPECT_TRUE(route.empty());
}

TEST(MazeQSolverTest, BuildRouteReturnsEmptyForInvalidStart) {
  s21::Maze maze;
  maze.GenerateMaze(5, 5);

  s21::MazeQSolver solver;
  solver.SetMaze(maze);
  solver.SetGoal({4, 4});
  solver.Train(5000, 0.1, 0.9, 0.2);

  auto route = solver.BuildRoute({10, 10});
  EXPECT_TRUE(route.empty());
}

TEST(MazeQSolverTest, AvailableActionsAreConsistentWithMaze) {
  s21::Maze maze;
  maze.GenerateMaze(5, 5);

  s21::MazeQSolver solver;
  solver.SetMaze(maze);
  solver.SetGoal({4, 4});

  auto actions = solver.GetAvailableActions(0, 0);

  for (int action : actions) {
    auto next = solver.ApplyAction(0, 0, action);
    EXPECT_TRUE(next.first >= 0 && next.first < maze.GetRows());
    EXPECT_TRUE(next.second >= 0 && next.second < maze.GetCols());
    EXPECT_TRUE(CanMoveQ(maze, {0, 0}, next));
  }
}

TEST(MazeQSolverTest, TrainChangesAtLeastOneQValue) {
  s21::Maze maze;
  maze.GenerateMaze(6, 6);

  s21::MazeQSolver solver;
  solver.SetMaze(maze);
  solver.SetGoal({5, 5});

  bool changed = false;

  solver.Train(20000, 0.1, 0.9, 0.2);

  for (int i = 0; i < maze.GetRows(); ++i) {
    for (int j = 0; j < maze.GetCols(); ++j) {
      for (int action = 0; action < 4; ++action) {
        if (solver.GetQValue(i, j, action) != 0.0) {
          changed = true;
        }
      }
    }
  }

  EXPECT_TRUE(changed);
}

TEST(MazeQSolverTest, TrainedSolverBuildsRouteToGoal) {
  s21::Maze maze;
  maze.GenerateMaze(6, 6);

  s21::MazeQSolver solver;
  solver.SetMaze(maze);
  solver.SetGoal({5, 5});
  solver.Train(30000, 0.1, 0.9, 0.2);

  auto route = solver.BuildRoute({0, 0});
  ExpectValidRouteQ(maze, route, {0, 0}, {5, 5});
}

TEST(MazeQSolverTest, TrainedSolverBuildsRouteFromDifferentStart) {
  s21::Maze maze;
  maze.GenerateMaze(6, 6);

  s21::MazeQSolver solver;
  solver.SetMaze(maze);
  solver.SetGoal({5, 5});
  solver.Train(30000, 0.1, 0.9, 0.2);

  auto route = solver.BuildRoute({2, 1});
  ExpectValidRouteQ(maze, route, {2, 1}, {5, 5});
}