#pragma once

#include <QTimer>
#include <QWidget>
#include <utility>
#include <vector>

#include "../core/maze.h"

class MazeView : public QWidget {
	Q_OBJECT

   public:
	explicit MazeView(QWidget* parent = nullptr);

	void SetMaze(const s21::Maze& maze);
	void SetPath(const std::vector<std::pair<int, int>>& path);

protected:
	void paintEvent(QPaintEvent* event) override;

private slots:
 void OnPathAnimationStep();

private:
	s21::Maze maze_;

	std::vector<std::pair<int, int>> path_;
	int visible_path_size_ = 0;
	QTimer* path_timer_ = nullptr;
};