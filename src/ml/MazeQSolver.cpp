#include "MazeQSolver.h"

#include <algorithm>
#include <cstdlib>
#include <limits>

namespace s21 {
	void MazeQSolver::SetMaze(const Maze& maze) {
		maze_ = maze;
		has_maze_ = !maze_.IsEmpty();

		if (!has_maze_) {
			q_values_.clear();
			return;
		}

		q_values_.assign(maze_.GetRows(), std::vector<std::array<double, 4>>(maze_.GetCols(), {0.0, 0.0, 0.0, 0.0}));
	}

	void MazeQSolver::SetGoal(const std::pair<int, int>& goal) {
		goal_ = goal;
	}

	std::vector<int> MazeQSolver::GetAvailableActions(int row, int col) const {
		std::vector<int> actions;
		auto neighbours = maze_.GetNeighbours(row, col);

		for (const auto& next : neighbours) {
			if (next.first == row - 1 && next.second == col) {
				actions.push_back(kUp);
			} else if (next.first == row && next.second == col + 1) {
				actions.push_back(kRight);
			} else if (next.first == row + 1 && next.second == col) {
				actions.push_back(kDown);
			} else if (next.first == row && next.second == col - 1) {
				actions.push_back(kLeft);
			}
		}

		return actions;
	}

	std::pair<int, int> MazeQSolver::ApplyAction(int row, int col, int action) const {
		switch (action) {
			case kUp:
				return {row - 1, col};
			case kRight:
				return {row, col + 1};
			case kDown:
				return {row + 1, col};
			case kLeft:
				return {row, col - 1};
			default:
				return {row, col};
		}
	}

	bool MazeQSolver::IsReady() const {
		return has_maze_ && IsValidCell(goal_.first, goal_.second);
	}

	bool MazeQSolver::IsValidCell(int row, int col) const {
		return row >= 0 && row < maze_.GetRows() && col >= 0 && col < maze_.GetCols();
	}

	bool MazeQSolver::IsGoal(int row, int col) const {
		return row == goal_.first && col == goal_.second;
	}

	double MazeQSolver::GetReward(int row, int col) const {
		return IsGoal(row, col) ? 1000.0 : -1.0;
	}

	int MazeQSolver::ChooseActionEpsilonGreedy(
	int row, int col, double epsilon) const {
		auto actions = GetAvailableActions(row, col);
		if (actions.empty()) {
			return -1;
		}

		double random_value = static_cast<double>(std::rand()) / RAND_MAX;
		if (random_value < epsilon) {
			int index = std::rand() % actions.size();
			return actions[index];
		}

		return GetBestAction(row, col);
	}

	int MazeQSolver::GetBestAction(int row, int col) const {
		auto actions = GetAvailableActions(row, col);
		if (actions.empty()) {
			return -1;
		}

		int best_action = actions[0];
		double best_value = q_values_[row][col][best_action];

		for (int action : actions) {
			if (q_values_[row][col][action] > best_value) {
				best_value = q_values_[row][col][action];
				best_action = action;
			}
		}

		return best_action;
	}

	double MazeQSolver::GetMaxQ(int row, int col) const {
		auto actions = GetAvailableActions(row, col);
		if (actions.empty()) {
			return 0.0;
		}

		double best = q_values_[row][col][actions[0]];
		for (int action : actions) {
			best = std::max(best, q_values_[row][col][action]);
		}
		return best;
	}

	void MazeQSolver::Train(
	int epochs, double alpha, double gamma, double epsilon) {
		if (!IsReady()) {
			return;
		}
		int rows = maze_.GetRows();
		int cols = maze_.GetCols();

		for (int epoch = 0; epoch < epochs; ++epoch) {
			int row = std::rand() % rows;
			int col = std::rand() % cols;

			while (IsGoal(row, col)) {
				row = std::rand() % rows;
				col = std::rand() % cols;
			}

			int max_steps = rows * cols * 4;

			for (int step = 0; step < max_steps; ++step) {
				if (IsGoal(row, col)) break;
				int action = ChooseActionEpsilonGreedy(row, col, epsilon);
				if (action == -1) {
					break;
				}

				auto next = ApplyAction(row, col, action);
				int next_row = next.first;
				int next_col = next.second;


				double reward = GetReward(next_row, next_col);
				double old_q = q_values_[row][col][action];
				double next_max_q = GetMaxQ(next_row, next_col);

				q_values_[row][col][action] =
					old_q + alpha * (reward + gamma * next_max_q - old_q);

				row = next_row;
				col = next_col;
				if (IsGoal(row, col)) {
					break;
				}
			}


		}
	}

	std::vector<std::pair<int, int>> MazeQSolver::BuildRoute(
	const std::pair<int, int>& start) const {
		std::vector<std::pair<int, int>> route;

		if (!IsReady()) {
			return route;
		}

		if (!IsValidCell(start.first, start.second)) {
			return route;
		}

		int row = start.first;
		int col = start.second;

		route.push_back(start);

		int max_steps = maze_.GetRows() * maze_.GetCols() * 4;

		for (int step = 0; step < max_steps; ++step) {
			if (IsGoal(row, col)) {
				return route;
			}

			int action = GetBestAction(row, col);
			if (action == -1) {
				route.clear();
				return route;
			}

			auto next = ApplyAction(row, col, action);

			if (next == std::make_pair(row, col)) {
				route.clear();
				return route;
			}

			row = next.first;
			col = next.second;
			route.push_back({row, col});

			if (IsGoal(row, col)) {
				return route;
			}
		}

		route.clear();
		return route;
	}

	double MazeQSolver::GetQValue(int row, int col, int action) const {
		return q_values_[row][col][action];
	}
}