#pragma once

#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace s21 {
/**
 * @brief Класс для генерации, хранения, загрузки и решения лабиринта.
 */
class Maze {
 public:
  Maze() = default;
  /**
   * @brief Проверяет нижние стены строки и исправляет их при необходимости.
   */
  void ValidateBottomWalls(std::vector<int>& maze, int row);
  /**
   * @brief Подготавливает следующую строку после текущей.
   */
  void PrepareNextRow(std::vector<int>& maze, int row) const;
  /**
   * @brief Строит последнюю строку лабиринта.
   */
  void BuildLastRow(std::vector<int>& maze, int row);
  /**
   * @brief Возвращает соседние доступные клетки.
   */
  std::vector<std::pair<int, int>> GetNeighbours(int row, int column) const;
  /**
   * @brief Генерирует лабиринт заданного размера.
   */
  void GenerateMaze(int rows, int cols);
  /**
   * @brief Генерирует лабиринт заданного размера.
   */
  bool LoadFromFile(const std::string& path);
  /**
   * @brief Сохраняет лабиринт в файл.
   */
  bool SaveToFile(const std::string& path) const;
  /**
   * @brief Возвращает число строк.
   */
  int GetRows() const;
  /**
   * @brief Возвращает число столбцов.
   */
  int GetCols() const;
  /**
   * @brief Возвращает матрицу правых стен.
   */
  const std::vector<std::vector<int>>& GetRightWalls() const;
  /**
   * @brief Возвращает матрицу нижних стен.
   */
  const std::vector<std::vector<int>>& GetBottomWalls() const;
  /**
   * @brief Проверяет, пустой ли лабиринт.
   */
  bool IsEmpty() const;
  /**
   * @brief Ищет путь от старта до финиша.
   */
  std::vector<std::pair<int, int>> Bfs(const std::pair<int, int> start,
                                       const std::pair<int, int> finish);

  // void PrintLine(const std::vector<int>& maze) const;
  // void PrintMatrixRow(const std::vector<std::vector<int>>& matrix, int row)
  // const;

 private:
  int rows_ = 0;
  int cols_ = 0;

  std::vector<std::vector<int>> right_walls_;
  std::vector<std::vector<int>> bottom_walls_;

  /**
   * @brief Инициализирует матрицы стен.
   */
  void InitMatrices(int rows, int cols);
  /**
   * @brief Инициализирует множества строки.
   */
  void MazeInit(std::vector<int>& maze);
  /**
   * @brief Возвращает случайное булево значение.
   */
  bool Random();
  /**
   * @brief Объединяет множества.
   */
  void Merge(std::vector<int>& maze, int index);
  /**
   * @brief Строит правые стены строки.
   */
  void BuildRightWalls(std::vector<int>& maze, int row);
  /**
   * @brief Проверяет изоляцию множества снизу.
   * @param maze Строка множеств.
   * @param row Номер строки.
   * @param set Номер множества.
   * @return true, если множество изолируется.
   */
  bool check_bottom_isolation(std::vector<int>& maze, int row, int set) const;
  /**
   * @brief Считает количество клеток в множестве.
   */
  int CalculateSetMembers(std::vector<int>& maze, int element) const;
  /**
   * @brief Проверяет, одна ли клетка в множестве.
   */
  bool CheckUniqueSet(std::vector<int>& maze, int set, int index) const;
  /**
   * @brief Строит нижние стены строки.
   */
  void BuildBottomWalls(std::vector<int>& maze, int row);
  /**
   * @brief Считает число нижних стен в множестве.
   */
  int CalculateBottomWalls(std::vector<int>& maze, int row, int set) const;
};
}  // namespace s21
