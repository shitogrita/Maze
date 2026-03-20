#pragma once

#include <vector>
#include <utility>

#include "maze.h"

namespace s21 {
	class MazeQSolver {
	public:
		void SetMaze(const Maze& maze);
		void SetGoal(const std::pair<int, int>& goal);

		void Train(int epochs, double alpha, double gamma, double epsilon);

		std::vector<std::pair<int, int>> BuildRoute(
			const std::pair<int, int>& start) const;

		double GetQValue(int row, int col, int action) const;

	private:
		enum Action {
			kUp = 0,
			kRight = 1,
			kDown = 2,
			kLeft = 3
		  };
	public:
		bool IsReady() const;
		bool IsValidCell(int row, int col) const;
		bool IsGoal(int row, int col) const;

		std::vector<int> GetAvailableActions(int row, int col) const;
		std::pair<int, int> ApplyAction(int row, int col, int action) const;

		double GetReward(int row, int col) const;
		int ChooseActionEpsilonGreedy(int row, int col, double epsilon) const;
		int GetBestAction(int row, int col) const;
		double GetMaxQ(int row, int col) const;
	private:
		Maze maze_;
		bool has_maze_ = false;

		std::pair<int, int> goal_ = {-1, -1};

		std::vector<std::vector<std::array<double, 4>>> q_values_;
	};
}
