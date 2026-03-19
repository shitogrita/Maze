#include "mainwindow.h"

#include <QFileDialog>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  SetupUi();
  ApplyStyle();
}

void MainWindow::SetupUi() {
  setWindowTitle("Maze");
  setFixedSize(860, 560);

  central_widget_ = new QWidget(this);
  setCentralWidget(central_widget_);

  auto* main_layout = new QHBoxLayout(central_widget_);
  main_layout->setContentsMargins(20, 20, 20, 20);
  main_layout->setSpacing(20);

  maze_view_ = new MazeView(this);
  main_layout->addWidget(maze_view_, 0, Qt::AlignLeft | Qt::AlignTop);

  auto* side_panel = new QFrame(this);
  side_panel->setFixedWidth(280);

  auto* side_layout = new QVBoxLayout(side_panel);
  side_layout->setContentsMargins(12, 12, 12, 12);
  side_layout->setSpacing(8);

  title_label_ = new QLabel("MAZE CONTROL", this);
  title_label_->setAlignment(Qt::AlignCenter);
  title_label_->setFixedHeight(20);
  side_layout->addWidget(title_label_);

  import_button_ = new QPushButton("IMPORT", this);
  export_button_ = new QPushButton("EXPORT", this);
  generate_button_ = new QPushButton("GENERATE", this);
  solve_button_ = new QPushButton("SOLVE", this);

  import_button_->setFixedHeight(30);
  export_button_->setFixedHeight(30);
  generate_button_->setFixedHeight(30);
  solve_button_->setFixedHeight(30);

  side_layout->addWidget(import_button_);
  side_layout->addWidget(export_button_);

  auto* settings_title = new QLabel("MAZE GENERATION SETTINGS", this);
  settings_title->setAlignment(Qt::AlignCenter);
  settings_title->setFixedHeight(18);
  side_layout->addWidget(settings_title);

  auto* rows_label = new QLabel("MAZE ROWS", this);
  rows_label->setFixedHeight(16);
  rows_spin_ = new QSpinBox(this);
  rows_spin_->setRange(1, 50);
  rows_spin_->setValue(10);
  rows_spin_->setFixedHeight(26);

  auto* cols_label = new QLabel("MAZE COLS", this);
  cols_label->setFixedHeight(16);
  cols_spin_ = new QSpinBox(this);
  cols_spin_->setRange(1, 50);
  cols_spin_->setValue(10);
  cols_spin_->setFixedHeight(26);

  side_layout->addWidget(rows_label);
  side_layout->addWidget(rows_spin_);
  side_layout->addWidget(cols_label);
  side_layout->addWidget(cols_spin_);

  side_layout->addWidget(generate_button_);

  auto* solve_title = new QLabel("PATHFINDING SETTINGS", this);
  solve_title->setAlignment(Qt::AlignCenter);
  solve_title->setFixedHeight(18);
  side_layout->addWidget(solve_title);

  auto* path_frame = new QFrame(this);
  auto* path_layout = new QGridLayout(path_frame);
  path_layout->setContentsMargins(8, 8, 8, 8);
  path_layout->setHorizontalSpacing(8);
  path_layout->setVerticalSpacing(6);

  path_layout->setColumnMinimumWidth(0, 65);
  path_layout->setColumnMinimumWidth(1, 70);
  path_layout->setColumnMinimumWidth(2, 70);

  auto* start_label = new QLabel("START", path_frame);
  start_label->setFixedHeight(18);

  start_row_spin_ = new QSpinBox(path_frame);
  start_row_spin_->setRange(0, 49);
  start_row_spin_->setValue(0);
  start_row_spin_->setFixedHeight(26);

  start_col_spin_ = new QSpinBox(path_frame);
  start_col_spin_->setRange(0, 49);
  start_col_spin_->setValue(0);
  start_col_spin_->setFixedHeight(26);

  auto* finish_label = new QLabel("FINISH", path_frame);
  finish_label->setFixedHeight(18);

  finish_row_spin_ = new QSpinBox(path_frame);
  finish_row_spin_->setRange(0, 49);
  finish_row_spin_->setValue(9);
  finish_row_spin_->setFixedHeight(26);

  finish_col_spin_ = new QSpinBox(path_frame);
  finish_col_spin_->setRange(0, 49);
  finish_col_spin_->setValue(9);
  finish_col_spin_->setFixedHeight(26);

  path_layout->addWidget(start_label, 0, 0);
  path_layout->addWidget(start_row_spin_, 0, 1);
  path_layout->addWidget(start_col_spin_, 0, 2);

  path_layout->addWidget(finish_label, 1, 0);
  path_layout->addWidget(finish_row_spin_, 1, 1);
  path_layout->addWidget(finish_col_spin_, 1, 2);

  side_layout->addWidget(path_frame);
  side_layout->addWidget(solve_button_);

  info_label_ = new QLabel(" ", this);
  info_label_->setWordWrap(true);
  side_layout->addWidget(info_label_);

  side_layout->addStretch();

  main_layout->addWidget(side_panel, 0, Qt::AlignTop);

  connect(generate_button_, &QPushButton::clicked,
          this, &MainWindow::OnGenerateClicked);
  connect(import_button_, &QPushButton::clicked,
          this, &MainWindow::OnImportClicked);
  connect(export_button_, &QPushButton::clicked,
          this, &MainWindow::OnExportClicked);
  connect(solve_button_, &QPushButton::clicked,
          this, &MainWindow::OnSolveClicked);
}

void MainWindow::ApplyStyle() {
  setStyleSheet(R"(
    QMainWindow, QWidget {
      background-color: #2a2c33;
      color: #d8e1f0;
      font-family: Consolas, 'Courier New', monospace;
      font-size: 12px;
    }

    QFrame {
      background-color: #30333b;
      border: 1px solid #4b5568;
    }

    QLabel {
      color: #d8e1f0;
      background: transparent;
      border: none;
    }

    QSpinBox {
      background-color: #23252b;
      color: #e4ecf7;
      border: 1px solid #4e607c;
      padding: 2px 4px;
      min-height: 24px;
    }

    QPushButton {
      background-color: #1f2530;
      color: #78a6ff;
      border: 1px solid #4f78c7;
      padding: 4px;
      font-weight: bold;
      min-height: 26px;
    }

    QPushButton:hover {
      background-color: #263041;
      color: #9cc0ff;
      border: 1px solid #7ea7ff;
    }

    QPushButton:pressed {
      background-color: #18202c;
    }
  )");
}

void MainWindow::OnGenerateClicked() {
  int rows = rows_spin_->value();
  int cols = cols_spin_->value();

  maze_.GenerateMaze(rows, cols);
  maze_view_->SetMaze(maze_);
  maze_view_->SetPath({});

  start_row_spin_->setRange(0, rows - 1);
  start_col_spin_->setRange(0, cols - 1);
  finish_row_spin_->setRange(0, rows - 1);
  finish_col_spin_->setRange(0, cols - 1);

  start_row_spin_->setValue(0);
  start_col_spin_->setValue(0);
  finish_row_spin_->setValue(rows - 1);
  finish_col_spin_->setValue(cols - 1);
}

void MainWindow::OnImportClicked() {
  QString path = QFileDialog::getOpenFileName(
      this, "Open maze file", "", "Text files (*.txt);;All files (*)");

  if (path.isEmpty()) {
    return;
  }

  if (!maze_.LoadFromFile(path.toStdString())) {
    QMessageBox::warning(this, "Ошибка",
                         "Не удалось загрузить лабиринт из файла.");
    return;
  }

  rows_spin_->setValue(maze_.GetRows());
  cols_spin_->setValue(maze_.GetCols());

  start_row_spin_->setRange(0, maze_.GetRows() - 1);
  start_col_spin_->setRange(0, maze_.GetCols() - 1);
  finish_row_spin_->setRange(0, maze_.GetRows() - 1);
  finish_col_spin_->setRange(0, maze_.GetCols() - 1);

  start_row_spin_->setValue(0);
  start_col_spin_->setValue(0);
  finish_row_spin_->setValue(maze_.GetRows() - 1);
  finish_col_spin_->setValue(maze_.GetCols() - 1);

  maze_view_->SetMaze(maze_);
  maze_view_->SetPath({});
}

void MainWindow::OnExportClicked() {
  if (maze_.IsEmpty()) {
    QMessageBox::information(this, "Информация",
                             "Сначала сгенерируйте или загрузите лабиринт.");
    return;
  }

  QString path = QFileDialog::getSaveFileName(
      this, "Save maze file", "", "Text files (*.txt);;All files (*)");

  if (path.isEmpty()) {
    return;
  }

  if (!maze_.SaveToFile(path.toStdString())) {
    QMessageBox::warning(this, "Ошибка",
                         "Не удалось сохранить лабиринт в файл.");
  }
}

void MainWindow::OnSolveClicked() {
  if (maze_.IsEmpty()) {
    QMessageBox::information(this, "Информация",
                             "Сначала сгенерируйте или загрузите лабиринт.");
    return;
  }

  int start_row = start_row_spin_->value();
  int start_col = start_col_spin_->value();
  int finish_row = finish_row_spin_->value();
  int finish_col = finish_col_spin_->value();

  if (start_row < 0 || start_row >= maze_.GetRows() ||
      start_col < 0 || start_col >= maze_.GetCols() ||
      finish_row < 0 || finish_row >= maze_.GetRows() ||
      finish_col < 0 || finish_col >= maze_.GetCols()) {
    QMessageBox::warning(
        this, "Ошибка",
        "Координаты старта или финиша выходят за границы лабиринта.");
    maze_view_->SetPath({});
    return;
      }

  auto path = maze_.Bfs({start_row, start_col}, {finish_row, finish_col});

  if (path.empty()) {
    QMessageBox::information(this, "Информация", "Путь не найден.");
    maze_view_->SetPath({});
    return;
  }

  maze_view_->SetPath(path);
}