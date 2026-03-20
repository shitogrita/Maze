#pragma once

#include <QWidget>

#include "cave.h"

class CaveView : public QWidget {
	Q_OBJECT

   public:
	explicit CaveView(QWidget* parent = nullptr);

	void SetCave(const s21::Cave& cave);

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	s21::Cave cave_;
};