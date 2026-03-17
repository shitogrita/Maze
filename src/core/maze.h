#pragma once

#include <random>
#include <vector>
#include <string>
#include <iostream>

namespace s21 {
	class Maze {
	  public:
		Maze() = default;
		void GenerateMaze(int rows, int cols);

		bool LoadFromFile(const std::string& path);
		bool SaveToFile(const std::string& path) const;

		int GetRows() const;
		int GetCols() const;

		const std::vector<std::vector<int>>& GetRightWalls() const;
		const std::vector<std::vector<int>>& GetBottomWalls() const;

		bool IsEmpty() const;

		void PrintLine(const std::vector<int>& maze) const;
		void PrintMatrixRow(const std::vector<std::vector<int>>& matrix, int row) const;

	  private:
		int rows_ = 0;
		int cols_ = 0;

		std::vector<std::vector<int>> right_walls_;
		std::vector<std::vector<int>> bottom_walls_;

		void InitMatrices(int rows, int cols);
		void MazeInit(std::vector<int>& maze);
		bool Random();
		void Merge(std::vector<int>& maze, int index);
		void BuildWalls(std::vector<int>& maze, int row);
	};
}
