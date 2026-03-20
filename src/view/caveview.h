#pragma once

#include <QWidget>

#include "../core/cave.h"

/**
 * @brief Виджет для отображения пещеры.
 */
class CaveView : public QWidget {
	Q_OBJECT

   public:
	/**
	 * @brief Конструктор виджета.
	 */
	explicit CaveView(QWidget* parent = nullptr);

	/**
	 * @brief Устанавливает пещеру для отображения.
	 */
	void SetCave(const s21::Cave& cave);

protected:
	/**
	 * @brief Отрисовывает пещеру.
	 */
	void paintEvent(QPaintEvent* event) override;

private:
	s21::Cave cave_;
};