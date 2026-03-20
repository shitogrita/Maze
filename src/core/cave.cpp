#include "cave.h"

namespace s21 {
	void Cave::GenerateCave(int rows, int cols, int chance) {
		if (rows <= 0 || cols <= 0 || rows > 50 || cols > 50) {
			return;
		}
		if (chance < 0) {
			chance = 0;
		}
		if (chance > 100) {
			chance = 100;
		}
		rows_ = rows;
		cols_ = cols;
		cells_.assign(rows_, std::vector<int>(cols_, 0));

		std::random_device rd;
		std::mt19937 gen(rd());
		std::bernoulli_distribution dist(chance / 100.0);

		for (int i = 0; i < rows_; ++i) {
			for (int j = 0; j < cols_; ++j) {
				cells_[i][j] = dist(gen) ? 1 : 0;
			}
		}
	}

	int Cave::CountAliveNeighbours(int row, int col) const {
		int count = 0;
		for (int row_offset = -1; row_offset <= 1; ++row_offset) {
			for (int col_offset = -1; col_offset <= 1; ++col_offset) {
				if (row_offset == 0 && col_offset == 0) {
					continue; // место где мы находимся
				}

				int neighbour_row = row + row_offset;
				int neighbour_col = col + col_offset;

				if (neighbour_row < 0 || neighbour_row >= rows_ ||
					neighbour_col < 0 || neighbour_col >= cols_) {
					count++; // по тз внешние клетки живые
				} else if (cells_[neighbour_row][neighbour_col] == 1) {
					count++;
				}
			}
		}
		return count;
	}

	void Cave::Step() {
		if (IsEmpty()) {
			return;
		}
		std::vector<std::vector<int>> next(rows_, std::vector<int>(cols_,0));
		for (int i = 0; i < rows_; ++i) {
			for (int j = 0; j < cols_; ++j) {
				int neighbours = CountAliveNeighbours(i, j);

				if (cells_[i][j] == 1) {
					if (neighbours < death_limit_) {
						next[i][j] = 0;
					} else {
						next[i][j] = 1;
					}
				} else {
					if (neighbours > birth_limit_) {
						next[i][j] = 1;
					} else {
						next[i][j] = 0;
					}
				}
			}
		}

		cells_ = next;
	}

	void Cave::SetLimits(int birth_limit, int death_limit) {
		if (birth_limit >= 0 && birth_limit <= 7) {
			birth_limit_ = birth_limit;
		}

		if (death_limit >= 0 && death_limit <= 7) {
			death_limit_ = death_limit;
		}
	}

	const std::vector<std::vector<int>>& Cave::GetCells() const {
		return cells_;
	}

	bool Cave::IsEmpty() const {
		return rows_ == 0 || cols_ == 0;
	}

	int Cave::GetRows() const { return rows_; }

	int Cave::GetCols() const { return cols_; }

	bool Cave::LoadFromFile(const std::string& filename) {
		std::ifstream file(filename);
		if (!file.is_open()) {
			return false;
		}

		int rows = 0;
		int cols = 0;
		file >> rows >> cols;

		if (rows <= 0 || cols <= 0 || rows > 50 || cols > 50) {
			return false;
		}

		std::vector<std::vector<int>> temp(rows, std::vector<int>(cols, 0));

		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				if (!(file >> temp[i][j])) {
					return false;
				}
				if (temp[i][j] != 0 && temp[i][j] != 1) {
					return false;
				}
			}
		}

		rows_ = rows;
		cols_ = cols;
		cells_ = temp;
		return true;
	}

	bool Cave::SaveToFile(const std::string& filename) const {
		std::ofstream file(filename);
		if (!file.is_open()) {
			return false;
		}

		file << rows_ << " " << cols_ << "\n";
		for (int i = 0; i < rows_; ++i) {
			for (int j = 0; j < cols_; ++j) {
				file << cells_[i][j];
				if (j + 1 < cols_) {
					file << " ";
				}
			}
			file << "\n";
		}

		return true;
	}
}