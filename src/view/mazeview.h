#pragma once

#include <QWidget>

#include "maze.h"

class MazeView : public QWidget {
	Q_OBJECT

   public:
	explicit MazeView(QWidget* parent = nullptr);

	void SetMaze(const s21::Maze& maze);

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	s21::Maze maze_;
};