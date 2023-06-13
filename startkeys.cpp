#include <iostream>
#include <fstream>
#include <string>
#include "headers.h"

// Блок получения ключей при запуске
// Ключ "-max <макс.число>"
// Устанавливает передаваемое <макс.число> в качестве максимального задаваемого
// Ключ "-level <число = 1, 2 или 3>"
// Устанавливает в качестве максимального задаваемого числа относительное значение в зависимости от аргумента
// Ключ "-table"
// При запуске с данным ключом просто выводится таблица рекордов без игры
int get_max_num(int const& paramnum, char** params, int const& max_upplim, std::string const& r_fname) {
	int default_number = 100;

	// Ключевых слов может быть не больше двух
	if (paramnum > 1 && paramnum < 4) {

		// Получаем название ключа в новую переменную
		std::string parameter {params[1]};

		// Ключ -max <макс.число>
		if (parameter == "-max") {
			// Длина диапазона должна содержать корректное значение и быть не слишком большим
			if ((max_upplim > std::stoi(params[2])) && (std::stoi(params[2]) > 0)) {
				return std::stoi(params[2]);
			} else {
				// Если было введено некорректное значение ключа -max
				std::cout << "Warning: wrong -max number value: max_num value set by default now." << std::endl;
				return default_number;
			}
		}

		// Ключ -level <число = 1, 2 или 3>
		if (parameter == "-level") {
			// Длина диапазона должна содержать корректное значение и быть не слишком большим
			int level_num = std::stoi(params[2]);
			if ((4 > level_num) && (level_num > 0)) {
				switch (level_num){
					case 1:
						return (default_number/10);
						break;
					case 2:
						return (default_number/2);
						break;
					case 3:
						return default_number;
						break;
				}
			} else {
				// Если было введено некорректное значение ключа -level
				std::cout << "Warning: wrong -level number value: level_num value set by default now." << std::endl;
				return default_number;
			}
		}

		// Ключ -table
		if (parameter == "-table"){
			// Выводим таблицу рекордов в консоль
			return 0;
		}
	}
	return default_number;
}