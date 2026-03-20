#pragma once

#include <random>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

namespace s21 {
	/**
	* @brief Класс для генерации, хранения, загрузки и обновления пещеры.
	*/
	class Cave {
	public:
		Cave() = default;
		/**
		* @brief Генерирует пещеру заданного размера.
		*/
		void GenerateCave(int rows, int cols, int chance);
		/**
		* @brief Выполняет один шаг клеточного автомата.
		*/
		void Step();
		/**
		* @brief Считает число живых соседей клетки.
		*/
		int CountAliveNeighbours(int row, int col) const;
		/**
		* @brief Возвращает матрицу клеток пещеры.
		*/
		const std::vector<std::vector<int>>& GetCells() const;
		/**
		* @brief Загружает пещеру из файла.
		*/
		bool LoadFromFile(const std::string& filename);
		/**
		* @brief Сохраняет пещеру в файл.
		*/
		bool SaveToFile(const std::string& filename) const;
		/**
		* @brief Возвращает число строк.
		*/
		int GetRows() const;
		/**
		* @brief Возвращает число столбцов.
		*/
		int GetCols() const;
		/**
		* @brief Проверяет, пустая ли пещера.
		*/
		bool IsEmpty() const;
		/**
		* @brief Устанавливает пороги рождения и смерти клетки.
		*/
		void SetLimits(int birth_limit, int death_limit);
		/**
		* @brief Устанавливает матрицу клеток вручную для тестов.
		*/
		void SetCellsForTests(const std::vector<std::vector<int>>& cells);

	private:
		int rows_ = 0;
		int cols_ = 0;
		int birth_limit_ = 4;
		int death_limit_ = 3;

		std::vector<std::vector<int>> cells_;
	};
}