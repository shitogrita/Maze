#pragma once

#include <QMainWindow>

#include "maze.h"

class QSpinBox;
class QPushButton;
class QLabel;
class MazeView;

class MainWindow : public QMainWindow {
	Q_OBJECT

   public:
	explicit MainWindow(QWidget* parent = nullptr);

private slots:
 void OnGenerateClicked();
	void OnImportClicked();
	void OnExportClicked();

private:
	void SetupUi();
	void ApplyStyle();

	s21::Maze maze_;

	QWidget* central_widget_ = nullptr;
	MazeView* maze_view_ = nullptr;

	QSpinBox* rows_spin_ = nullptr;
	QSpinBox* cols_spin_ = nullptr;

	QPushButton* generate_button_ = nullptr;
	QPushButton* import_button_ = nullptr;
	QPushButton* export_button_ = nullptr;

	QLabel* title_label_ = nullptr;
	QLabel* info_label_ = nullptr;
};