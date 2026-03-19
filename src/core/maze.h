#pragma once

#include <random>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace s21 {
	class Maze {
	  public:
		Maze() = default;

		void ValidateBottomWalls(std::vector<int> &maze, int row);

		void PrepareNextRow(std::vector<int> &maze, int row) const;

		void BuildLastRow(std::vector<int> &maze, int row);

		std::vector<std::pair<int, int>> GetNeighbours(int row, int column) const;

		void GenerateMaze(int rows, int cols);

		bool LoadFromFile(const std::string& path);
		bool SaveToFile(const std::string& path) const;

		int GetRows() const;
		int GetCols() const;

		const std::vector<std::vector<int>>& GetRightWalls() const;
		const std::vector<std::vector<int>>& GetBottomWalls() const;

		bool IsEmpty() const;

		std::vector<std::pair<int, int>> Bfs(const std::pair<int, int> start, const std::pair<int, int> finish);

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
		void BuildRightWalls(std::vector<int>& maze, int row);

		bool check_bottom_isolation(std::vector<int> &maze, int row, int set) const;

		int CalculateSetMembers(std::vector<int> &maze, int element) const;

		bool CheckUniqueSet(std::vector<int> &maze, int set, int index) const;

		void BuildBottomWalls(std::vector<int>& maze, int row);

		int CalculateBottomWalls(std::vector<int> &maze, int row, int set) const;
	};
}
