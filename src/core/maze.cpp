#include "maze.h"
#include <fstream>

namespace s21 {
	static int counter_ = 1;
	void Maze::MazeInit(std::vector<int>& maze) {
		for (int i = 0; i < maze.size(); ++i) {
			if (maze[i] == 0) {
				maze[i] = counter_;
				counter_++;
			}
		}
	}

	void Maze::InitMatrices(int rows, int cols) {
		rows_ = rows;
		cols_ = cols;
		counter_ = 1;

		right_walls_.assign(rows_, std::vector<int>(cols_, 0));
		bottom_walls_.assign(rows_, std::vector<int>(cols_, 0));
	}

	void Maze::PrintLine(const std::vector<int>& maze) const { // old
		for (const auto& elem : maze) {
			std::cout << elem << " ";
		}
	}

	void Maze::PrintMatrixRow(const std::vector<std::vector<int>>& matrix, //old
							  int row) const {
		for (int j = 0; j < static_cast<int>(matrix[row].size()); ++j) {
			std::cout << matrix[row][j] << " ";
		}
		std::cout << std::endl;
	}

	bool Maze::Random() {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::bernoulli_distribution dist(0.5);
		return dist(gen);
	}

	void Maze::Merge(std::vector<int>& maze, int index) {
		int set_elem = maze[index];
		int replace_elem = maze[index + 1];
		for (int i = 0; i < static_cast<int>(maze.size()); i++) {
			if (maze[i] == replace_elem) {
				maze[i] = set_elem;
			}
		}
	}

	void Maze::BuildWalls(std::vector<int>& maze, int row) {
		for (int i = 0; i < static_cast<int>(maze.size()) - 1; i++) {
			bool should_build = Random();
			if (should_build || maze[i] == maze[i + 1]) {
				right_walls_[row][i] = 1;
			} else {
				Merge(maze, i);
			}
		}
		right_walls_[row][maze.size() - 1] = 1;
	}

	void Maze::GenerateMaze(int rows, int cols) {
		if (rows <= 0 || cols <= 0 || rows > 50 || cols > 50) {
			return;
		}

		InitMatrices(rows, cols);

		std::vector<int> maze(cols, 0);

		// ВАЖНО:
		// Здесь оставлена ваша текущая логика без изменения идеи.
		// Это пока заглушечная/неполная генерация — только старт вашего алгоритма.
		// GUI уже сможет это отрисовать.
		MazeInit(maze);
		BuildWalls(maze, 0);

		// Заглушка, чтобы на экране что-то было и по нижним стенам тоже:
		for (int r = 0; r < rows_; ++r) {
			for (int c = 0; c < cols_; ++c) {
				if (r == rows_ - 1) {
					bottom_walls_[r][c] = 1;
				} else {
					bottom_walls_[r][c] = Random() ? 1 : 0;
				}
			}
			right_walls_[r][cols_ - 1] = 1;
		}
	}

	bool Maze::LoadFromFile(const std::string& path) {
		(void)path;
		return false;
	}

	bool Maze::SaveToFile(const std::string& path) const {
		(void)path;
		return false;
	}

	int Maze::GetRows() const { return rows_; }
	int Maze::GetCols() const { return cols_; }

	const std::vector<std::vector<int>>& Maze::GetRightWalls() const {
		return right_walls_;
	}
	const std::vector<std::vector<int>>& Maze::GetBottomWalls() const {
		return bottom_walls_;
	}

	bool Maze::IsEmpty() const {
		return rows_ == 0 || cols_ == 0;
	}

}