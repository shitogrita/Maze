#include "mainwindow.h"

#include <QFileDialog>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

#include "mazeview.h"

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
  side_layout->setContentsMargins(16, 16, 16, 16);
  side_layout->setSpacing(14);

  title_label_ = new QLabel("MAZE CONTROL", this);
  side_layout->addWidget(title_label_);

  import_button_ = new QPushButton("IMPORT", this);
  export_button_ = new QPushButton("EXPORT", this);
  generate_button_ = new QPushButton("GENERATE", this);

  side_layout->addWidget(import_button_);
  side_layout->addWidget(export_button_);

  auto* settings_title = new QLabel("MAZE GENERATION SETTINGS", this);
  side_layout->addWidget(settings_title);

  auto* rows_label = new QLabel("MAZE ROWS", this);
  rows_spin_ = new QSpinBox(this);
  rows_spin_->setRange(1, 50);
  rows_spin_->setValue(10);

  auto* cols_label = new QLabel("MAZE COLS", this);
  cols_spin_ = new QSpinBox(this);
  cols_spin_->setRange(1, 50);
  cols_spin_->setValue(10);

  side_layout->addWidget(rows_label);
  side_layout->addWidget(rows_spin_);
  side_layout->addWidget(cols_label);
  side_layout->addWidget(cols_spin_);

  side_layout->addWidget(generate_button_);

  info_label_ = new QLabel(
      " ",
      this);
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
    }

    QSpinBox {
      background-color: #23252b;
      color: #e4ecf7;
      border: 1px solid #4e607c;
      padding: 6px;
      min-height: 28px;
    }

    QPushButton {
      background-color: #1f2530;
      color: #78a6ff;
      border: 1px solid #4f78c7;
      padding: 10px;
      font-weight: bold;
      min-height: 34px;
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
  maze_view_->SetMaze(maze_);
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