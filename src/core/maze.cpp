#include "maze.h"
#include <fstream>

namespace s21 {
	int counter_ = 1;

	void Maze::MazeInit(std::vector<int> &maze) {
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

	// void Maze::PrintLine(const std::vector<int> &maze) const {
	// 	// old
	// 	for (const auto &elem: maze) {
	// 		std::cout << elem << " ";
	// 	}
	// }
	//
	// void Maze::PrintMatrixRow(const std::vector<std::vector<int> > &matrix, //old
	// 						int row) const {
	// 	for (int j = 0; j < static_cast<int>(matrix[row].size()); ++j) {
	// 		std::cout << matrix[row][j] << " ";
	// 	}
	// 	std::cout << std::endl;
	// }

	bool Maze::Random() {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::bernoulli_distribution dist(0.5);
		return dist(gen);
	}

	void Maze::Merge(std::vector<int> &maze, int index) {
		int set_elem = maze[index];
		int replace_elem = maze[index + 1];
		for (int i = 0; i < static_cast<int>(maze.size()); i++) {
			if (maze[i] == replace_elem) {
				maze[i] = set_elem;
			}
		}
	}

	void Maze::BuildRightWalls(std::vector<int> &maze, int row) {
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


	int Maze::CalculateSetMembers(std::vector<int> &maze, int element) const {
		int count_set = 0;
		for (int i = 0; i < cols_; i++) {
			if (maze[i] == element) {
				count_set++;
			}
		}
		return count_set;
	}


	bool Maze::CheckUniqueSet(std::vector<int> &maze, int set, int index) const {
		for (int i = 0; i < cols_; ++i) {
			if (maze[i] == set && i != index) {
				return false;
			}
		}
		return true;
	}

	void Maze::BuildBottomWalls(std::vector<int> &maze, int row) {
		for (int i = 0; i < cols_; ++i) {
			auto should_build = Random();
			if (!CheckUniqueSet(maze, maze[i], i) && should_build) {
				bottom_walls_[row][i] = 1;
			}
		}
	}

	int Maze::CalculateBottomWalls(std::vector<int> &maze, int row, int set) const {
		auto count = 0;
		for (int i = 0; i < cols_; ++i) {
			if (bottom_walls_[row][i] == 1 && maze[i] == set) {
				count++;
			}
		}
		return count;
	}

	void Maze::ValidateBottomWalls(std::vector<int> &maze, int row) {
		for (int i = 0; i < cols_; ++i) {
			if (CalculateBottomWalls(maze, row, maze[i]) == CalculateSetMembers(maze, maze[i])) {
				bottom_walls_[row][i] = 0;
			}
		}
	}

	// 1 1 0 3 4
	void Maze::PrepareNextRow(std::vector<int> &maze, int row) const {
		for (int i = 0; i < cols_; ++i) {
			if (bottom_walls_[row][i] == 1) {
				maze[i] = 0;
			}
		}
	}

	void Maze::BuildLastRow(std::vector<int> &maze, int row) {
		MazeInit(maze);

		for (int i = 0; i < cols_ - 1; ++i) {
			if (maze[i] != maze[i + 1]) {
				right_walls_[row][i] = 0;
				Merge(maze, i);
			} else {
				right_walls_[row][i] = 1;
			}
		}

		right_walls_[row][cols_ - 1] = 1;

		for (int i = 0; i < cols_; ++i) {
			bottom_walls_[row][i] = 1;
		}
	}

	std::vector<std::pair<int, int> > Maze::GetNeighbours(int row, int column) const {
		std::vector<std::pair<int, int> > neighbours;
		if (column + 1 < cols_ && right_walls_[row][column] == 0) {
			neighbours.emplace_back(row, column + 1);
		}

		if (column - 1 >= 0 && right_walls_[row][column - 1] == 0) {
			neighbours.emplace_back(row, column - 1);
		}

		if (row + 1 < rows_ && bottom_walls_[row][column] == 0) {
			neighbours.emplace_back(row + 1, column);
		}

		if (row - 1 >= 0 && bottom_walls_[row - 1][column] == 0) {
			neighbours.emplace_back(row - 1, column);
		}
		return neighbours;
	}

	void Maze::GenerateMaze(int rows, int cols) {
		if (rows <= 0 || cols <= 0 || rows > 50 || cols > 50) {
			return;
		}

		InitMatrices(rows, cols);
		std::vector<int> maze(cols, 0);

		for (int row = 0; row < rows_ - 1; ++row) {
			MazeInit(maze);
			BuildRightWalls(maze, row);
			BuildBottomWalls(maze, row);
			ValidateBottomWalls(maze, row);
			PrepareNextRow(maze, row);
		}

		BuildLastRow(maze, rows_ - 1);
	}

	bool Maze::LoadFromFile(const std::string& filename) {
		std::ifstream file(filename);
		if (!file.is_open()) {
			return false;
		}

		int rows = 0;
		int cols = 0;
		file >> rows >> cols;

		rows_ = rows;
		cols_ = cols;
		right_walls_.assign(rows_, std::vector<int>(cols_, 0));
		bottom_walls_.assign(rows_, std::vector<int>(cols_, 0));

		for (int i = 0; i < rows_; ++i) {
			for (int j = 0; j < cols_; ++j) {
				file >> right_walls_[i][j];
			}
		}
		for (int i = 0; i < rows_; ++i) {
			for (int j = 0; j < cols_; ++j) {
				file >> bottom_walls_[i][j];
			}
		}

		return true;
	}

	bool Maze::SaveToFile(const std::string& filename) const {
		std::ofstream file(filename);
		if (!file.is_open()) {
			return false;
		}

		file << rows_ << " " << cols_ << "\n";

		for (int i = 0; i < rows_; ++i) {
			for (int j = 0; j < cols_; ++j) {
				file << right_walls_[i][j] << " ";
			}
			file << "\n";
		}
		file << "\n";
		for (int i = 0; i < rows_; ++i) {
			for (int j = 0; j < cols_; ++j) {
				file << bottom_walls_[i][j] << " ";
			}
			file << "\n";
		}

		return true;
	}

	int Maze::GetRows() const { return rows_; }
	int Maze::GetCols() const { return cols_; }

	const std::vector<std::vector<int> > &Maze::GetRightWalls() const {
		return right_walls_;
	}

	const std::vector<std::vector<int> > &Maze::GetBottomWalls() const {
		return bottom_walls_;
	}

	bool Maze::IsEmpty() const {
		return rows_ == 0 || cols_ == 0;
	}

	std::vector<std::pair<int, int> > Maze::Bfs(const std::pair<int, int> start, const std::pair<int, int> finish) {
		std::vector<std::pair<int, int> > path; // возращенный путь

		if (rows_ <= 0 || cols_ <= 0) {
			return path;
		}

		if (start.first < 0 || start.first >= rows_ ||
			start.second < 0 || start.second >= cols_) {
			return path;
		}

		if (finish.first < 0 || finish.first >= rows_ ||
			finish.second < 0 || finish.second >= cols_) {
			return path;
		}

		if (start == finish) {
			path.push_back(start);
			return path;
		}
		std::queue<std::pair<int, int> > index; // путь который надо обработать
		std::vector<std::vector<bool> > visited(rows_, std::vector<bool>(cols_, false));
		std::vector<std::vector<std::pair<int, int> > > parent(
			rows_, std::vector<std::pair<int, int> >(cols_, {-1, -1}));

		index.push(start);
		visited[start.first][start.second] = true;

		while (!index.empty()) {
			std::pair<int, int> current = index.front();
			index.pop();

			if (current == finish) {
				break;
			}

			auto neighbours = GetNeighbours(current.first, current.second);

			for (const auto &next: neighbours) {
				if (next.first < 0 || next.first >= rows_ ||
					next.second < 0 || next.second >= cols_) {
					continue;
					}
				if (!visited[next.first][next.second]) {
					visited[next.first][next.second] = true;
					parent[next.first][next.second] = current;
					index.push(next);
				}
			}
		}
			if (!visited[finish.first][finish.second]) {
				return path;
			}

			std::pair<int, int> restore = finish;
			while (restore != start) {
				path.push_back(restore);

				std::pair<int, int> prev = parent[restore.first][restore.second];
				if (prev.first == -1 && prev.second == -1) {
					path.clear();
					return path;
				}

				restore = prev;
			}

			path.push_back(start);
			std::reverse(path.begin(), path.end());

			return path;

	}
}
