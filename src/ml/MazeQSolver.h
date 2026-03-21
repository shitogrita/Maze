#pragma once

#include <array>
#include <utility>
#include <vector>

#include "maze.h"

namespace s21 {
/**
 * @brief Класс для обучения агента прохождению лабиринта методом Q-learning.
 */
class MazeQSolver {
 public:
  /**
   * @brief Устанавливает лабиринт для обучения.
   */
  void SetMaze(const Maze& maze);
  /**
   * @brief Устанавливает целевую клетку.
   */
  void SetGoal(const std::pair<int, int>& goal);
  /**
   * @brief Обучает агента заданное число эпох.
   */
  void Train(int epochs, double alpha, double gamma, double epsilon);
  /**
   * @brief Строит маршрут от стартовой клетки до цели.
   */
  std::vector<std::pair<int, int>> BuildRoute(
      const std::pair<int, int>& start) const;
  /**
   * @brief Возвращает Q-значение для клетки и действия.
   */
  double GetQValue(int row, int col, int action) const;

 private:
  enum Action { kUp = 0, kRight = 1, kDown = 2, kLeft = 3 };

 public:
  /**
   * @brief Проверяет, готов ли агент к работе.
   */
  bool IsReady() const;
  /**
   * @brief Проверяет, находится ли клетка внутри лабиринта.
   */
  bool IsValidCell(int row, int col) const;
  /**
   * @brief Проверяет, является ли клетка целевой.
   */
  bool IsGoal(int row, int col) const;
  /**
   * @brief Возвращает список доступных действий из клетки.
   */
  std::vector<int> GetAvailableActions(int row, int col) const;
  /**
   * @brief Применяет действие к клетке.
   */
  std::pair<int, int> ApplyAction(int row, int col, int action) const;
  /**
   * @brief Возвращает награду за переход в клетку.
   */
  double GetReward(int row, int col) const;
  /**
   * @brief Выбирает действие по epsilon-greedy стратегии.
   */
  int ChooseActionEpsilonGreedy(int row, int col, double epsilon) const;
  /**
   * @brief Возвращает лучшее действие для клетки.
   */
  int GetBestAction(int row, int col) const;
  /**
   * @brief Возвращает максимальное Q-значение для клетки.
   */
  double GetMaxQ(int row, int col) const;

 private:
  Maze maze_;
  bool has_maze_ = false;

  std::pair<int, int> goal_ = {-1, -1};

  std::vector<std::vector<std::array<double, 4>>> q_values_;
};
}  // namespace s21