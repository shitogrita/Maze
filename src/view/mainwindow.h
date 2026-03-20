#pragma once

#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <QTabWidget>
#include <QTimer>
#include <QWidget>

#include "cave.h"
#include "caveview.h"
#include "maze.h"
#include "mazeview.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = nullptr);

 private slots:
  void OnGenerateClicked();
  void OnImportClicked();
  void OnExportClicked();
  void OnSolveClicked();

  void OnCaveGenerateClicked();
  void OnCaveImportClicked();
  void OnCaveExportClicked();
  void OnCaveStepClicked();
  void OnCaveAutoClicked();
  void OnCaveStopClicked();
  void OnCaveTimerTimeout();

 private:
  void SetupUi();
  void SetupMazeTab();
  void SetupCaveTab();
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
};