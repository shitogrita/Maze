#pragma once

#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <QTabWidget>
#include <QTimer>
#include <QWidget>

#include "MazeQSolver.h"
#include "cave.h"
#include "caveview.h"
#include "maze.h"
#include "mazeview.h"

/**
 * @brief Главное окно приложения.
 *
 * Содержит вкладки для работы с лабиринтом, пещерой
 * и обучением агента на основе Q-learning.
 */
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  /**
   * @brief Конструктор главного окна.
   */
  explicit MainWindow(QWidget* parent = nullptr);

 private slots:
  /**
   * @brief Генерирует новый лабиринт.
   */
  void OnGenerateClicked();

  /**
   * @brief Загружает лабиринт из файла.
   */
  void OnImportClicked();

  /**
   * @brief Сохраняет лабиринт в файл.
   */
  void OnExportClicked();

  /**
   * @brief Ищет путь в лабиринте.
   */
  void OnSolveClicked();

  /**
   * @brief Генерирует новую пещеру.
   */
  void OnCaveGenerateClicked();

  /**
   * @brief Загружает пещеру из файла.
   */
  void OnCaveImportClicked();

  /**
   * @brief Сохраняет пещеру в файл.
   */
  void OnCaveExportClicked();

  /**
   * @brief Выполняет один шаг эволюции пещеры.
   */
  void OnCaveStepClicked();

  /**
   * @brief Запускает автоматическое обновление пещеры.
   */
  void OnCaveAutoClicked();

  /**
   * @brief Останавливает автоматическое обновление пещеры.
   */
  void OnCaveStopClicked();

  /**
   * @brief Обрабатывает событие таймера пещеры.
   */
  void OnCaveTimerTimeout();

  /**
   * @brief Загружает лабиринт для Q-learning.
   */
  void OnQImportClicked();

  /**
   * @brief Запускает обучение агента.
   */
  void OnQTrainClicked();

  /**
   * @brief Строит маршрут обученным агентом.
   */
  void OnQRunClicked();

  /**
   * @brief Останавливает обучение агента.
   */
  void OnQStopClicked();

  /**
   * @brief Обрабатывает шаг анимации обучения агента.
   */
  void OnQTrainingTimerTimeout();

 private:
  /**
   * @brief Создаёт основной интерфейс окна.
   */
  void SetupUi();

  /**
   * @brief Создаёт вкладку лабиринта.
   */
  void SetupMazeTab();

  /**
   * @brief Создаёт вкладку пещеры.
   */
  void SetupCaveTab();

  /**
   * @brief Создаёт вкладку Q-learning.
   */
  void SetupQTab();

  /**
   * @brief Применяет стиль интерфейса.
   */
  void ApplyStyle();

 private:
  QWidget* central_widget_ = nullptr;
  QTabWidget* tab_widget_ = nullptr;

  QWidget* maze_tab_ = nullptr;
  MazeView* maze_view_ = nullptr;
  s21::Maze maze_;

  QLabel* title_label_ = nullptr;
  QLabel* info_label_ = nullptr;

  QPushButton* import_button_ = nullptr;
  QPushButton* export_button_ = nullptr;
  QPushButton* generate_button_ = nullptr;
  QPushButton* solve_button_ = nullptr;

  QSpinBox* rows_spin_ = nullptr;
  QSpinBox* cols_spin_ = nullptr;

  QSpinBox* start_row_spin_ = nullptr;
  QSpinBox* start_col_spin_ = nullptr;
  QSpinBox* finish_row_spin_ = nullptr;
  QSpinBox* finish_col_spin_ = nullptr;

  QWidget* cave_tab_ = nullptr;
  CaveView* cave_view_ = nullptr;
  s21::Cave cave_;
  QTimer* cave_timer_ = nullptr;

  QPushButton* cave_import_button_ = nullptr;
  QPushButton* cave_export_button_ = nullptr;
  QPushButton* cave_generate_button_ = nullptr;
  QPushButton* cave_step_button_ = nullptr;
  QPushButton* cave_auto_button_ = nullptr;
  QPushButton* cave_stop_button_ = nullptr;

  QSpinBox* cave_rows_spin_ = nullptr;
  QSpinBox* cave_cols_spin_ = nullptr;
  QSpinBox* cave_chance_spin_ = nullptr;
  QSpinBox* cave_birth_spin_ = nullptr;
  QSpinBox* cave_death_spin_ = nullptr;
  QSpinBox* cave_interval_spin_ = nullptr;

  QWidget* q_tab_ = nullptr;
  MazeView* q_maze_view_ = nullptr;
  s21::Maze rl_maze_;
  s21::MazeQSolver q_solver_;
  QTimer* q_training_timer_ = nullptr;

  QPushButton* q_import_button_ = nullptr;
  QPushButton* q_train_button_ = nullptr;
  QPushButton* q_run_button_ = nullptr;
  QPushButton* q_stop_button_ = nullptr;

  QLabel* q_info_label_ = nullptr;

  QSpinBox* q_goal_row_spin_ = nullptr;
  QSpinBox* q_goal_col_spin_ = nullptr;

  QSpinBox* q_start_row_spin_ = nullptr;
  QSpinBox* q_start_col_spin_ = nullptr;

  QSpinBox* q_epochs_per_tick_spin_ = nullptr;
  QSpinBox* q_timer_interval_spin_ = nullptr;
  QSpinBox* q_total_epochs_spin_ = nullptr;

  int q_epochs_done_ = 0;
  int q_total_epochs_target_ = 0;

  double q_alpha_ = 0.1;
  double q_gamma_ = 0.9;
  double q_epsilon_ = 0.2;
};