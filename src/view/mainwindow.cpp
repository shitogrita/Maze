#include "mainwindow.h"

#include <QFileDialog>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	SetupUi();
	ApplyStyle();
}

void MainWindow::SetupUi() {
	setWindowTitle("Maze");
	setFixedSize(900, 620);

	central_widget_ = new QWidget(this);
	setCentralWidget(central_widget_);

	auto *root_layout = new QVBoxLayout(central_widget_);
	root_layout->setContentsMargins(12, 12, 12, 12);
	root_layout->setSpacing(8);

	tab_widget_ = new QTabWidget(this);
	root_layout->addWidget(tab_widget_);

	SetupMazeTab();
	SetupCaveTab();
	SetupQTab();
}

void MainWindow::SetupMazeTab() {
	maze_tab_ = new QWidget(this);
	tab_widget_->addTab(maze_tab_, "Maze");

	auto *main_layout = new QHBoxLayout(maze_tab_);
	main_layout->setContentsMargins(16, 16, 16, 16);
	main_layout->setSpacing(16);

	maze_view_ = new MazeView(this);
	main_layout->addWidget(maze_view_, 0, Qt::AlignLeft | Qt::AlignTop);

	auto *side_panel = new QFrame(this);
	side_panel->setFixedWidth(280);

	auto *side_layout = new QVBoxLayout(side_panel);
	side_layout->setContentsMargins(12, 12, 12, 12);
	side_layout->setSpacing(8);

	title_label_ = new QLabel("MAZE CONTROL", this);
	title_label_->setAlignment(Qt::AlignCenter);
	title_label_->setFixedHeight(22);
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

	auto *settings_title = new QLabel("MAZE GENERATION SETTINGS", this);
	settings_title->setAlignment(Qt::AlignCenter);
	settings_title->setFixedHeight(18);
	side_layout->addWidget(settings_title);

	auto *rows_label = new QLabel("MAZE ROWS", this);
	rows_label->setFixedHeight(16);
	rows_spin_ = new QSpinBox(this);
	rows_spin_->setRange(1, 50);
	rows_spin_->setValue(10);
	rows_spin_->setFixedHeight(26);

	auto *cols_label = new QLabel("MAZE COLS", this);
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

	auto *solve_title = new QLabel("PATHFINDING SETTINGS", this);
	solve_title->setAlignment(Qt::AlignCenter);
	solve_title->setFixedHeight(18);
	side_layout->addWidget(solve_title);

	auto *path_frame = new QFrame(this);
	auto *path_layout = new QGridLayout(path_frame);
	path_layout->setContentsMargins(8, 8, 8, 8);
	path_layout->setHorizontalSpacing(8);
	path_layout->setVerticalSpacing(6);

	path_layout->setColumnMinimumWidth(0, 65);
	path_layout->setColumnMinimumWidth(1, 70);
	path_layout->setColumnMinimumWidth(2, 70);

	auto *start_label = new QLabel("START", path_frame);
	start_label->setFixedHeight(18);

	start_row_spin_ = new QSpinBox(path_frame);
	start_row_spin_->setRange(0, 49);
	start_row_spin_->setValue(0);
	start_row_spin_->setFixedHeight(26);

	start_col_spin_ = new QSpinBox(path_frame);
	start_col_spin_->setRange(0, 49);
	start_col_spin_->setValue(0);
	start_col_spin_->setFixedHeight(26);

	auto *finish_label = new QLabel("FINISH", path_frame);
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

void MainWindow::SetupCaveTab() {
	cave_tab_ = new QWidget(this);
	tab_widget_->addTab(cave_tab_, "Cave");

	auto *main_layout = new QHBoxLayout(cave_tab_);
	main_layout->setContentsMargins(16, 16, 16, 16);
	main_layout->setSpacing(16);

	cave_timer_ = new QTimer(this);
	connect(cave_timer_, &QTimer::timeout,
			this, &MainWindow::OnCaveTimerTimeout);

	cave_view_ = new CaveView(this);
	main_layout->addWidget(cave_view_, 0, Qt::AlignLeft | Qt::AlignTop);

	auto *side_panel = new QFrame(this);
	side_panel->setFixedWidth(280);

	auto *side_layout = new QVBoxLayout(side_panel);
	side_layout->setContentsMargins(10, 10, 10, 10);
	side_layout->setSpacing(6);

	auto *title = new QLabel("CAVE CONTROL", this);
	title->setAlignment(Qt::AlignCenter);
	title->setFixedHeight(18);
	side_layout->addWidget(title);

	cave_import_button_ = new QPushButton("IMPORT", this);
	cave_export_button_ = new QPushButton("EXPORT", this);
	cave_generate_button_ = new QPushButton("GENERATE", this);
	cave_step_button_ = new QPushButton("NEXT STEP", this);
	cave_auto_button_ = new QPushButton("AUTO RUN", this);
	cave_stop_button_ = new QPushButton("STOP", this);

	cave_import_button_->setFixedHeight(28);
	cave_export_button_->setFixedHeight(28);
	cave_generate_button_->setFixedHeight(28);
	cave_step_button_->setFixedHeight(28);
	cave_auto_button_->setFixedHeight(28);
	cave_stop_button_->setFixedHeight(28);

	auto *import_export_layout = new QHBoxLayout();
	import_export_layout->setSpacing(6);
	import_export_layout->addWidget(cave_import_button_);
	import_export_layout->addWidget(cave_export_button_);
	side_layout->addLayout(import_export_layout);

	auto *generation_title = new QLabel("CAVE GENERATION SETTINGS", this);
	generation_title->setAlignment(Qt::AlignCenter);
	generation_title->setFixedHeight(16);
	side_layout->addWidget(generation_title);

	auto *size_frame = new QFrame(this);
	auto *size_layout = new QGridLayout(size_frame);
	size_layout->setContentsMargins(6, 6, 6, 6);
	size_layout->setHorizontalSpacing(6);
	size_layout->setVerticalSpacing(4);

	auto *rows_label = new QLabel("ROWS", size_frame);
	rows_label->setFixedHeight(16);
	cave_rows_spin_ = new QSpinBox(size_frame);
	cave_rows_spin_->setRange(1, 50);
	cave_rows_spin_->setValue(20);
	cave_rows_spin_->setFixedHeight(24);

	auto *cols_label = new QLabel("COLS", size_frame);
	cols_label->setFixedHeight(16);
	cave_cols_spin_ = new QSpinBox(size_frame);
	cave_cols_spin_->setRange(1, 50);
	cave_cols_spin_->setValue(20);
	cave_cols_spin_->setFixedHeight(24);

	size_layout->addWidget(rows_label, 0, 0);
	size_layout->addWidget(cols_label, 0, 1);
	size_layout->addWidget(cave_rows_spin_, 1, 0);
	size_layout->addWidget(cave_cols_spin_, 1, 1);

	side_layout->addWidget(size_frame);

	auto *chance_label = new QLabel("INITIAL CHANCE %", this);
	chance_label->setFixedHeight(16);
	cave_chance_spin_ = new QSpinBox(this);
	cave_chance_spin_->setRange(0, 100);
	cave_chance_spin_->setValue(45);
	cave_chance_spin_->setFixedHeight(24);

	side_layout->addWidget(chance_label);
	side_layout->addWidget(cave_chance_spin_);
	side_layout->addWidget(cave_generate_button_);

	auto *automata_title = new QLabel("CELLULAR AUTOMATON SETTINGS", this);
	automata_title->setAlignment(Qt::AlignCenter);
	automata_title->setFixedHeight(16);
	side_layout->addWidget(automata_title);

	auto *limits_frame = new QFrame(this);
	auto *limits_layout = new QGridLayout(limits_frame);
	limits_layout->setContentsMargins(6, 6, 6, 6);
	limits_layout->setHorizontalSpacing(6);
	limits_layout->setVerticalSpacing(4);

	auto *birth_label = new QLabel("BIRTH", limits_frame);
	birth_label->setFixedHeight(16);
	cave_birth_spin_ = new QSpinBox(limits_frame);
	cave_birth_spin_->setRange(0, 7);
	cave_birth_spin_->setValue(4);
	cave_birth_spin_->setFixedHeight(24);

	auto *death_label = new QLabel("DEATH", limits_frame);
	death_label->setFixedHeight(16);
	cave_death_spin_ = new QSpinBox(limits_frame);
	cave_death_spin_->setRange(0, 7);
	cave_death_spin_->setValue(3);
	cave_death_spin_->setFixedHeight(24);

	limits_layout->addWidget(birth_label, 0, 0);
	limits_layout->addWidget(death_label, 0, 1);
	limits_layout->addWidget(cave_birth_spin_, 1, 0);
	limits_layout->addWidget(cave_death_spin_, 1, 1);

	side_layout->addWidget(limits_frame);

	auto *interval_label = new QLabel("AUTO STEP MS", this);
	interval_label->setFixedHeight(16);
	cave_interval_spin_ = new QSpinBox(this);
	cave_interval_spin_->setRange(1, 5000);
	cave_interval_spin_->setValue(210);
	cave_interval_spin_->setFixedHeight(24);

	side_layout->addWidget(interval_label);
	side_layout->addWidget(cave_interval_spin_);

	side_layout->addWidget(cave_step_button_);
	side_layout->addWidget(cave_auto_button_);
	side_layout->addWidget(cave_stop_button_);

	side_layout->addStretch();

	main_layout->addWidget(side_panel, 0, Qt::AlignTop);

	connect(cave_generate_button_, &QPushButton::clicked,
			this, &MainWindow::OnCaveGenerateClicked);
	connect(cave_import_button_, &QPushButton::clicked,
			this, &MainWindow::OnCaveImportClicked);
	connect(cave_export_button_, &QPushButton::clicked,
			this, &MainWindow::OnCaveExportClicked);
	connect(cave_step_button_, &QPushButton::clicked,
			this, &MainWindow::OnCaveStepClicked);
	connect(cave_auto_button_, &QPushButton::clicked,
			this, &MainWindow::OnCaveAutoClicked);
	connect(cave_stop_button_, &QPushButton::clicked,
			this, &MainWindow::OnCaveStopClicked);
}

void MainWindow::SetupQTab() {
	q_tab_ = new QWidget(this);
	tab_widget_->addTab(q_tab_, "Q-Learning");

	auto *main_layout = new QHBoxLayout(q_tab_);
	main_layout->setContentsMargins(16, 16, 16, 16);
	main_layout->setSpacing(16);

	q_maze_view_ = new MazeView(this);
	main_layout->addWidget(q_maze_view_, 0, Qt::AlignLeft | Qt::AlignTop);

	q_training_timer_ = new QTimer(this);
	connect(q_training_timer_, &QTimer::timeout,
			this, &MainWindow::OnQTrainingTimerTimeout);

	auto *side_panel = new QFrame(this);
	side_panel->setFixedWidth(300);

	auto *side_layout = new QVBoxLayout(side_panel);
	side_layout->setContentsMargins(12, 12, 12, 12);
	side_layout->setSpacing(8);

	auto *title = new QLabel("Q-LEARNING CONTROL", this);
	title->setAlignment(Qt::AlignCenter);
	title->setFixedHeight(22);
	side_layout->addWidget(title);

	q_import_button_ = new QPushButton("IMPORT MAZE", this);
	q_train_button_ = new QPushButton("TRAIN AGENT", this);
	q_run_button_ = new QPushButton("RUN AGENT", this);
	q_stop_button_ = new QPushButton("STOP TRAINING", this);

	q_import_button_->setFixedHeight(30);
	q_train_button_->setFixedHeight(30);
	q_run_button_->setFixedHeight(30);
	q_stop_button_->setFixedHeight(30);

	side_layout->addWidget(q_import_button_);

	auto *goal_title = new QLabel("FIXED GOAL", this);
	goal_title->setAlignment(Qt::AlignCenter);
	side_layout->addWidget(goal_title);

	auto *goal_frame = new QFrame(this);
	auto *goal_layout = new QGridLayout(goal_frame);
	goal_layout->setContentsMargins(8, 8, 8, 8);
	goal_layout->setHorizontalSpacing(8);
	goal_layout->setVerticalSpacing(6);

	auto *goal_row_label = new QLabel("GOAL", goal_frame);
	q_goal_row_spin_ = new QSpinBox(goal_frame);
	q_goal_col_spin_ = new QSpinBox(goal_frame);

	q_goal_row_spin_->setRange(0, 49);
	q_goal_col_spin_->setRange(0, 49);
	q_goal_row_spin_->setValue(9);
	q_goal_col_spin_->setValue(9);

	q_goal_row_spin_->setFixedHeight(26);
	q_goal_col_spin_->setFixedHeight(26);

	goal_layout->addWidget(goal_row_label, 0, 0);
	goal_layout->addWidget(q_goal_row_spin_, 0, 1);
	goal_layout->addWidget(q_goal_col_spin_, 0, 2);

	side_layout->addWidget(goal_frame);

	auto *train_title = new QLabel("TRAINING SETTINGS", this);
	train_title->setAlignment(Qt::AlignCenter);
	side_layout->addWidget(train_title);

	auto *train_frame = new QFrame(this);
	auto *train_layout = new QGridLayout(train_frame);
	train_layout->setContentsMargins(8, 8, 8, 8);
	train_layout->setHorizontalSpacing(8);
	train_layout->setVerticalSpacing(6);

	auto *total_epochs_label = new QLabel("TOTAL EPOCHS", train_frame);
	q_total_epochs_spin_ = new QSpinBox(train_frame);
	q_total_epochs_spin_->setRange(1, 1000000);
	q_total_epochs_spin_->setValue(5000);
	q_total_epochs_spin_->setFixedHeight(26);

	auto *chunk_label = new QLabel("EPOCHS/TICK", train_frame);
	q_epochs_per_tick_spin_ = new QSpinBox(train_frame);
	q_epochs_per_tick_spin_->setRange(1, 10000);
	q_epochs_per_tick_spin_->setValue(100);
	q_epochs_per_tick_spin_->setFixedHeight(26);

	auto *interval_label = new QLabel("TIMER MS", train_frame);
	q_timer_interval_spin_ = new QSpinBox(train_frame);
	q_timer_interval_spin_->setRange(1, 5000);
	q_timer_interval_spin_->setValue(60);
	q_timer_interval_spin_->setFixedHeight(26);

	train_layout->addWidget(total_epochs_label, 0, 0);
	train_layout->addWidget(q_total_epochs_spin_, 0, 1);
	train_layout->addWidget(chunk_label, 1, 0);
	train_layout->addWidget(q_epochs_per_tick_spin_, 1, 1);
	train_layout->addWidget(interval_label, 2, 0);
	train_layout->addWidget(q_timer_interval_spin_, 2, 1);

	side_layout->addWidget(train_frame);
	side_layout->addWidget(q_train_button_);
	side_layout->addWidget(q_stop_button_);

	auto *run_title = new QLabel("RUN TRAINED AGENT", this);
	run_title->setAlignment(Qt::AlignCenter);
	side_layout->addWidget(run_title);

	auto *run_frame = new QFrame(this);
	auto *run_layout = new QGridLayout(run_frame);
	run_layout->setContentsMargins(8, 8, 8, 8);
	run_layout->setHorizontalSpacing(8);
	run_layout->setVerticalSpacing(6);

	auto *start_label = new QLabel("START", run_frame);
	q_start_row_spin_ = new QSpinBox(run_frame);
	q_start_col_spin_ = new QSpinBox(run_frame);

	q_start_row_spin_->setRange(0, 49);
	q_start_col_spin_->setRange(0, 49);
	q_start_row_spin_->setValue(0);
	q_start_col_spin_->setValue(0);

	q_start_row_spin_->setFixedHeight(26);
	q_start_col_spin_->setFixedHeight(26);

	run_layout->addWidget(start_label, 0, 0);
	run_layout->addWidget(q_start_row_spin_, 0, 1);
	run_layout->addWidget(q_start_col_spin_, 0, 2);

	side_layout->addWidget(run_frame);
	side_layout->addWidget(q_run_button_);

	q_info_label_ = new QLabel("Load maze, set goal, train agent.", this);
	q_info_label_->setWordWrap(true);
	side_layout->addWidget(q_info_label_);

	side_layout->addStretch();

	main_layout->addWidget(side_panel, 0, Qt::AlignTop);

	connect(q_import_button_, &QPushButton::clicked,
			this, &MainWindow::OnQImportClicked);
	connect(q_train_button_, &QPushButton::clicked,
			this, &MainWindow::OnQTrainClicked);
	connect(q_run_button_, &QPushButton::clicked,
			this, &MainWindow::OnQRunClicked);
	connect(q_stop_button_, &QPushButton::clicked,
			this, &MainWindow::OnQStopClicked);
}

void MainWindow::OnQImportClicked() {
	QString path = QFileDialog::getOpenFileName(
		this, "Open maze file", "", "Text files (*.txt);;All files (*)");

	if (path.isEmpty()) {
		return;
	}

	if (!rl_maze_.LoadFromFile(path.toStdString())) {
		QMessageBox::warning(this, "Ошибка",
							"Не удалось загрузить лабиринт из файла.");
		return;
	}

	q_maze_view_->SetMaze(rl_maze_);
	q_maze_view_->SetPath({});

	q_goal_row_spin_->setRange(0, rl_maze_.GetRows() - 1);
	q_goal_col_spin_->setRange(0, rl_maze_.GetCols() - 1);
	q_start_row_spin_->setRange(0, rl_maze_.GetRows() - 1);
	q_start_col_spin_->setRange(0, rl_maze_.GetCols() - 1);

	q_goal_row_spin_->setValue(rl_maze_.GetRows() - 1);
	q_goal_col_spin_->setValue(rl_maze_.GetCols() - 1);
	q_start_row_spin_->setValue(0);
	q_start_col_spin_->setValue(0);

	q_solver_.SetMaze(rl_maze_);
	q_solver_.SetGoal({q_goal_row_spin_->value(), q_goal_col_spin_->value()});

	q_epochs_done_ = 0;
	q_total_epochs_target_ = 0;

	q_info_label_->setText("Maze loaded. Configure goal and start training.");
}

void MainWindow::OnQTrainClicked() {
	if (rl_maze_.IsEmpty()) {
		QMessageBox::information(this, "Информация",
								"Сначала загрузите лабиринт.");
		return;
	}

	std::pair<int, int> goal = {
		q_goal_row_spin_->value(),
		q_goal_col_spin_->value()
	};

	q_solver_.SetMaze(rl_maze_);
	q_solver_.SetGoal(goal);

	q_epochs_done_ = 0;
	q_total_epochs_target_ = q_total_epochs_spin_->value();

	q_maze_view_->SetMaze(rl_maze_);
	q_maze_view_->SetPath({});

	q_training_timer_->stop();
	q_training_timer_->setInterval(q_timer_interval_spin_->value());
	q_training_timer_->start();

	q_info_label_->setText("Training started...");
}

void MainWindow::OnQStopClicked() {
	q_training_timer_->stop();
	q_info_label_->setText("Training stopped.");
}

void MainWindow::OnQTrainingTimerTimeout() {
	if (q_epochs_done_ >= q_total_epochs_target_) {
		q_training_timer_->stop();
		q_info_label_->setText(
			QString("Training completed: %1 epochs.")
			.arg(q_epochs_done_));
		return;
	}

	int chunk = q_epochs_per_tick_spin_->value();
	int remaining = q_total_epochs_target_ - q_epochs_done_;
	int current_chunk = std::min(chunk, remaining);

	q_solver_.Train(current_chunk, q_alpha_, q_gamma_, q_epsilon_);
	q_epochs_done_ += current_chunk;

	std::pair<int, int> demo_start = {
		q_start_row_spin_->value(),
		q_start_col_spin_->value()
	};

	auto demo_route = q_solver_.BuildRoute(demo_start);
	q_maze_view_->SetPath(demo_route);

	q_info_label_->setText(
		QString("Training: %1 / %2 epochs")
		.arg(q_epochs_done_)
		.arg(q_total_epochs_target_));
}


void MainWindow::OnQRunClicked() {
	if (rl_maze_.IsEmpty()) {
		QMessageBox::information(this, "Информация",
								"Сначала загрузите лабиринт.");
		return;
	}

	std::pair<int, int> goal = {
		q_goal_row_spin_->value(),
		q_goal_col_spin_->value()
	};
	q_solver_.SetGoal(goal);

	std::pair<int, int> start = {
		q_start_row_spin_->value(),
		q_start_col_spin_->value()
	};

	auto route = q_solver_.BuildRoute(start);
	if (route.empty()) {
		QMessageBox::information(this, "Информация",
								"Агент не смог построить маршрут. Продолжите обучение.");
		q_maze_view_->SetPath({});
		return;
	}

	q_maze_view_->SetPath(route);
	q_info_label_->setText("Agent route построен.");
}

void MainWindow::ApplyStyle() {
	setStyleSheet(R"(
    QMainWindow, QWidget {
      background-color: #2a2c33;
      color: #d8e1f0;
      font-family: Consolas, 'Courier New', monospace;
      font-size: 12px;
    }

    QTabWidget::pane {
      border: 1px solid #4b5568;
      background-color: #2a2c33;
    }

    QTabBar::tab {
      background-color: #1f2530;
      color: #78a6ff;
      border: 1px solid #4f78c7;
      padding: 5px 12px;
      min-width: 80px;
    }

    QTabBar::tab:selected {
      background-color: #263041;
      color: #9cc0ff;
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

void MainWindow::OnCaveGenerateClicked() {
	int rows = cave_rows_spin_->value();
	int cols = cave_cols_spin_->value();
	int chance = cave_chance_spin_->value();

	cave_.GenerateCave(rows, cols, chance);
	cave_.SetLimits(cave_birth_spin_->value(), cave_death_spin_->value());
	cave_view_->SetCave(cave_);
}

void MainWindow::OnCaveImportClicked() {
	QString path = QFileDialog::getOpenFileName(
		this, "Open cave file", "", "Text files (*.txt);;All files (*)");

	if (path.isEmpty()) {
		return;
	}

	if (!cave_.LoadFromFile(path.toStdString())) {
		QMessageBox::warning(this, "Ошибка",
							"Не удалось загрузить пещеру из файла.");
		return;
	}

	cave_rows_spin_->setValue(cave_.GetRows());
	cave_cols_spin_->setValue(cave_.GetCols());
	cave_view_->SetCave(cave_);
}

void MainWindow::OnCaveExportClicked() {
	if (cave_.IsEmpty()) {
		QMessageBox::information(this, "Информация",
								"Сначала сгенерируйте или загрузите пещеру.");
		return;
	}

	QString path = QFileDialog::getSaveFileName(
		this, "Save cave file", "", "Text files (*.txt);;All files (*)");

	if (path.isEmpty()) {
		return;
	}

	if (!cave_.SaveToFile(path.toStdString())) {
		QMessageBox::warning(this, "Ошибка",
							"Не удалось сохранить пещеру в файл.");
	}
}

void MainWindow::OnCaveStepClicked() {
	if (cave_.IsEmpty()) {
		QMessageBox::information(this, "Информация",
								"Сначала сгенерируйте или загрузите пещеру.");
		return;
	}

	cave_.SetLimits(cave_birth_spin_->value(), cave_death_spin_->value());
	cave_.Step();
	cave_view_->SetCave(cave_);
}

void MainWindow::OnCaveAutoClicked() {
	if (cave_.IsEmpty()) {
		QMessageBox::information(this, "Информация",
								"Сначала сгенерируйте или загрузите пещеру.");
		return;
	}

	cave_.SetLimits(cave_birth_spin_->value(), cave_death_spin_->value());

	int interval_ms = cave_interval_spin_->value();
	cave_timer_->stop();
	cave_timer_->setInterval(interval_ms);
	cave_timer_->start();
}

void MainWindow::OnCaveStopClicked() {
	cave_timer_->stop();
}

void MainWindow::OnCaveTimerTimeout() {
	if (cave_.IsEmpty()) {
		cave_timer_->stop();
		return;
	}

	cave_.SetLimits(cave_birth_spin_->value(), cave_death_spin_->value());
	cave_.Step();
	cave_view_->SetCave(cave_);
}
