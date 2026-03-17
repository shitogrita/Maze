#include "mazeview.h"

#include <QPainter>
#include <QPen>

MazeView::MazeView(QWidget* parent) : QWidget(parent) {
	setFixedSize(500, 500);
}

void MazeView::SetMaze(const s21::Maze& maze) {
	maze_ = maze;
	update();
}

void MazeView::paintEvent(QPaintEvent* event) {
	QWidget::paintEvent(event);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, false);

	painter.fillRect(rect(), QColor(28, 30, 36));

	if (maze_.IsEmpty()) {
		QPen border_pen(QColor(120, 140, 170));
		border_pen.setWidth(2);
		painter.setPen(border_pen);
		painter.drawRect(0, 0, 499, 499);
		return;
	}

	int rows = maze_.GetRows();
	int cols = maze_.GetCols();

	double cell_w = 500.0 / cols;
	double cell_h = 500.0 / rows;

	const auto& right = maze_.GetRightWalls();
	const auto& bottom = maze_.GetBottomWalls();

	// Подложка клеток
	QPen cell_pen(QColor(55, 58, 66));
	cell_pen.setWidth(1);
	painter.setPen(cell_pen);

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			int x = static_cast<int>(j * cell_w);
			int y = static_cast<int>(i * cell_h);
			int w = static_cast<int>((j + 1) * cell_w) - x;
			int h = static_cast<int>((i + 1) * cell_h) - y;

			painter.fillRect(x, y, w, h, QColor(40, 42, 48));
			painter.drawRect(x, y, w, h);
		}
	}

	// Внешняя рамка и стены лабиринта
	QPen wall_pen(QColor(215, 225, 240));
	wall_pen.setWidth(2);
	painter.setPen(wall_pen);

	painter.drawRect(0, 0, 499, 499);

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			int x1 = static_cast<int>(j * cell_w);
			int y1 = static_cast<int>(i * cell_h);
			int x2 = static_cast<int>((j + 1) * cell_w);
			int y2 = static_cast<int>((i + 1) * cell_h);

			if (right[i][j]) {
				painter.drawLine(x2, y1, x2, y2);
			}

			if (bottom[i][j]) {
				painter.drawLine(x1, y2, x2, y2);
			}
		}
	}
}