#pragma once

#include <QTimer>
#include <QWidget>
#include <utility>
#include <vector>

#include "../core/maze.h"

/**
 * @brief Виджет для отображения лабиринта и пути.
 */
class MazeView : public QWidget {
  Q_OBJECT

 public:
  /**
   * @brief Конструктор виджета.
   */
  explicit MazeView(QWidget* parent = nullptr);

  /**
   * @brief Устанавливает лабиринт для отображения.
   */
  void SetMaze(const s21::Maze& maze);

  /**
   * @brief Устанавливает путь для отображения.
   */
  void SetPath(const std::vector<std::pair<int, int>>& path);

 protected:
  /**
   * @brief Отрисовывает лабиринт и путь.
   */
  void paintEvent(QPaintEvent* event) override;

 private slots:
  /**
   * @brief Выполняет следующий шаг анимации пути.
   */
  void OnPathAnimationStep();

 private:
  s21::Maze maze_;

  std::vector<std::pair<int, int>> path_;
  int visible_path_size_ = 0;
  QTimer* path_timer_ = nullptr;
};