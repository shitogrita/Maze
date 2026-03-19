#include "mazeview.h"

#include <QPainter>
#include <QPen>

MazeView::MazeView(QWidget* parent) : QWidget(parent) {
  setFixedSize(500, 500);

  path_timer_ = new QTimer(this);
  connect(path_timer_, &QTimer::timeout, this, &MazeView::OnPathAnimationStep);
}

void MazeView::SetMaze(const s21::Maze& maze) {
  maze_ = maze;
  path_.clear();
  visible_path_size_ = 0;

  if (path_timer_->isActive()) {
    path_timer_->stop();
  }

  update();
}

void MazeView::SetPath(const std::vector<std::pair<int, int>>& path) {
  path_ = path;
  visible_path_size_ = 0;

  if (path_timer_->isActive()) {
    path_timer_->stop();
  }

  if (path_.empty()) {
    update();
    return;
  }

  visible_path_size_ = 1;
  path_timer_->start(35);
  update();
}

void MazeView::OnPathAnimationStep() {
  if (visible_path_size_ < static_cast<int>(path_.size())) {
    ++visible_path_size_;
    update();
  } else {
    path_timer_->stop();
  }
}

void MazeView::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);

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

  if (!path_.empty() && visible_path_size_ > 0) {
    QPen path_pen(QColor(120, 166, 255));
    path_pen.setWidth(4);
    painter.setPen(path_pen);

    int draw_count = std::min(visible_path_size_, static_cast<int>(path_.size()));

    for (int i = 0; i + 1 < draw_count; ++i) {
      int row1 = path_[i].first;
      int col1 = path_[i].second;
      int row2 = path_[i + 1].first;
      int col2 = path_[i + 1].second;

      int x1 = static_cast<int>(col1 * cell_w + cell_w / 2.0);
      int y1 = static_cast<int>(row1 * cell_h + cell_h / 2.0);
      int x2 = static_cast<int>(col2 * cell_w + cell_w / 2.0);
      int y2 = static_cast<int>(row2 * cell_h + cell_h / 2.0);

      painter.drawLine(x1, y1, x2, y2);
    }

    if (draw_count >= 1) {
      int last_row = path_[draw_count - 1].first;
      int last_col = path_[draw_count - 1].second;

      int cx = static_cast<int>(last_col * cell_w + cell_w / 2.0);
      int cy = static_cast<int>(last_row * cell_h + cell_h / 2.0);

      painter.setBrush(QColor(120, 166, 255));
      painter.setPen(Qt::NoPen);
      painter.drawEllipse(QPoint(cx, cy), 4, 4);
    }
  }

  QPen wall_pen(QColor(215, 225, 240));
  wall_pen.setWidth(2);
  painter.setPen(wall_pen);
  painter.setBrush(Qt::NoBrush);

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