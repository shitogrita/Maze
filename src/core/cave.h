#pragma once

#include <random>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

namespace s21 {
	class Cave {
	  public:
		Cave() = default;

		void GenerateCave(int rows, int cols, int chance);
		void Step();
		int CountAliveNeighbours(int row, int col) const;
		const std::vector<std::vector<int>>& GetCells() const;
		bool LoadFromFile(const std::string& filename);
		bool SaveToFile(const std::string& filename) const;

		int GetRows() const;
		int GetCols() const;
		bool IsEmpty() const;

		void SetLimits(int birth_limit, int death_limit);

		void SetCellsForTests(const std::vector<std::vector<int>>& cells);

	  private:
		int rows_ = 0;
		int cols_ = 0;
		int birth_limit_ = 4;
		int death_limit_ = 3;

		std::vector<std::vector<int>> cells_;

	};
}