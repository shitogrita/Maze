#include "caveview.h"

#include <QPainter>
#include <QPen>

CaveView::CaveView(QWidget* parent) : QWidget(parent) {
  setFixedSize(500, 500);
}

void CaveView::SetCave(const s21::Cave& cave) {
  cave_ = cave;
  update();
}

void CaveView::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, false);

  painter.fillRect(rect(), QColor(28, 30, 36));

  if (cave_.IsEmpty()) {
    QPen border_pen(QColor(120, 140, 170));
    border_pen.setWidth(2);
    painter.setPen(border_pen);
    painter.drawRect(0, 0, 499, 499);
    return;
  }

  int rows = cave_.GetRows();
  int cols = cave_.GetCols();

  double cell_w = 500.0 / cols;
  double cell_h = 500.0 / rows;

  const auto& cells = cave_.GetCells();

  QPen grid_pen(QColor(55, 58, 66));
  grid_pen.setWidth(1);
  painter.setPen(grid_pen);

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      int x = static_cast<int>(j * cell_w);
      int y = static_cast<int>(i * cell_h);
      int w = static_cast<int>((j + 1) * cell_w) - x;
      int h = static_cast<int>((i + 1) * cell_h) - y;

      if (cells[i][j] == 1) {
        painter.fillRect(x, y, w, h, QColor(215, 225, 240));
      } else {
        painter.fillRect(x, y, w, h, QColor(40, 42, 48));
      }

      painter.drawRect(x, y, w, h);
    }
  }

  QPen border_pen(QColor(215, 225, 240));
  border_pen.setWidth(2);
  painter.setPen(border_pen);
  painter.setBrush(Qt::NoBrush);
  painter.drawRect(0, 0, 499, 499);
}