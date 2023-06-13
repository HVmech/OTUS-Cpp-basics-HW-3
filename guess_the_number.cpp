#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <filesystem>

#include "headers.h"

bool update_file(std::string const& filename, std::string const& usrname, const int& attnum){
	std::fstream file{filename};
    // Подготавливаем файл к чтению/записи
    // Проверяем, открыт ли файл
    // Если файл не открылся, то аварийно завершаем функцию
    if (!file.is_open()) {
        file.open(filename, std::ios_base::beg);
        if (!file.is_open()){
		    std::cout << "Failed attempt to open file: " << filename << "!" << std::endl;
		    return 0;
        }
    }

	// Инициализируем значения внутренних переменных
	int strnum = 1;
	int rewr = 0;
    size_t char_count = 0;
    int atttemp;
    std::string tempname;
    std:: string str1, str2;
    unsigned long long filesize;

    // Переносим указатель в начало файла
	file.seekp(0, std::ios_base::beg);

	// В цикле выполняется подсчет количества строк файла с таблицей рекордов
	// 1. Переменная strnum получает на выходе количество записей рекордов 
	// strnum = кол-во строк файла - 1
	// 2. Переменная rewr запоминает номер строки 
	// rewr = 0, если игрок новичок
	// rewr = -1*strnum, если игрок не улучшил результат
	// rewr = strnum, если игрок улучшил результат
    while(1) {
		file >> tempname;
		file >> atttemp;
		if(file.fail()) break;
		file.ignore();
		if(tempname == usrname){
			if(atttemp > attnum){
				rewr = strnum;
			}
			else{
				rewr = strnum*(-1);
			}
		}
		strnum++;
	}
	// Закрываем файл, т.к. он завершился аварийно и более не нужен
	file.close();

	// Снова открываем тот же файл
	file.open(filename);

    // Переносим указатель в начало файла
	file.seekp(0, std::ios_base::beg);

	// Проверка, открыт ли файл
	if (!file.is_open()) {
		std::cout << "Failed attempt to open file:  " << filename << "!" << std::endl;
		return 0;
	}

    // Если результат пользователя улучшился и файл необходимо изменить rewr > 0:
	if(rewr > 0){
		// Доходим до искомой строки в файле
		for(int i = 1; i < rewr; i++){
			std::getline(file, str1);
		}

		size_t pos = size_t(file.tellp());

		// Перемещаем строку в конец файла
		for(int i = rewr; i < strnum-1; i++){
            // Запоминаем позицию указателя ввода/вывода в файле
			pos = size_t(file.tellp());

            // Считываем две строки в буферные переменные
			getline(file, str1);
			getline(file, str2);

            // Перемещаемся на позицию в начале первой строки
			file.seekp (pos, file.beg);

            // Меняем строки местами, записывая с перезаписью сначала строку #2, а потом строку #1
			file << str2 << std::endl;
			file << str1 << std::endl;

            // Обновляем файл
			file.flush();

            // Переходим в начало последней записанной строки
			pos = size_t(file.tellp()) - str1.length() - 2;
			file.seekp(pos, file.beg);
		}

		// Устанавливаем указатель на нужное место
		file.seekp(pos, file.beg);

        // Перезаписываем последнюю строку файла
		file << usrname << " " << attnum;
		file.flush();

        // Измеряем размер файла
        filesize = std::filesystem::file_size(filename);

        // Считываем мусорные символы в конце файла, если таковые есть
		// Вносим символ конца строки в начало кучи мусора
		pos = size_t(file.tellp());
		std::getline(file, str1);
		char_count = str1.length();
		file.seekp(pos, file.beg);
		file << std::endl;

        // Закрываем файл
        file.close();

        // Выкидываем мусор из конца файла
        filesize = filesize - char_count;
        std::filesystem::resize_file(filename, filesize);

        return 1;
	}
	else{
        // Если игрок ранее не играл в игру
		if(!rewr){
            // Добавляем строку с именем пользователя и результатами в конец файла
			file.seekp (0, file.end);
			file << usrname << " " << attnum << std::endl;
		}
        // Если игрок не улучшил счет или строка с результатами была добавлена, закрываем файл
        file.close();
        return 1;
	}
}

bool read_file(std::string const& filename){
	std::ifstream file{filename};
    // Проверка, открыт ли файл
	if (!file.is_open()) {
		file.open(filename);
		file.seekg(0, std::ios_base::beg);
		if(file.is_open()){
			std::cout << "Failed attempt to open file:  " << filename << "!" << std::endl;
			return 0;
		}
	}

    // Инициализируем строку-буфер
    std::string buff;

    std::cout << "High scores table:" << std::endl;
    // Выводим содержимое файла в консоль
    while(1){
        std::getline(file, buff);
        std::cout << buff << std::endl;;
        if(file.fail()) break;
    }

	file.close();
    return 1;
}

int main(int argc, char** argv) {
	// Константы:
	// 1. Путь к файлу с рекордами
	// 2. Имеющий смысл диапазон угадывания числа
	const std::string high_scores_filename{"../high_scores.txt"};
	const int reasonable_max = 1000;

	// Игровые переменные:
	// 1. Имя пользователя
	std::string username;
	// 2. Индикатор первого запуска
	bool first_time_only = true;
	// 3. Количество затраченных на угадывание попыток
	int attempts;

	// Обрабатываем ключи
	// Если задействован ключ -max, то инициализируем максимальное значение для генерируемого числа
	const int max_num = get_max_num(argc, argv, reasonable_max, high_scores_filename);
	// Если задействован ключ "-table", то необходимо завершить выполнение программы
	if(!max_num) {
		// Выводим таблицу рекордов в консоль
		if(!read_file(high_scores_filename)){
			std::cout << "Failed attempt to read file!" << std::endl;
			return -1;
		}
		return 0;
	}

	// Заходим в блок игры
	do{
		// Играем раунд
		// Если играем впервые, то необходимо запомнить имя игрока
		// После первого раунда игра более не запрашивает ввод имени
		if (first_time_only){
			attempts = game(max_num, username, first_time_only);
			first_time_only = false;
		}
		else{
			attempts = game(max_num, username, first_time_only);
		}

		// Предохранитель на случай неверного ввода числа
		if(attempts < 0){
			std::cout << "Warning! Too many attempts!" << std::endl;
			return attempts;
		}

		// Обновляем файл с таблицей рекордов
		// Если файл не обновился, то аварийно завершаем программу
		if(!update_file(high_scores_filename, username, attempts)){
			std::cout << "Failed attempt to update file!" << std::endl;
			return -1;
		}

		// Выводим таблицу рекордов в консоль
		if(!read_file(high_scores_filename)){
			std::cout << "Failed attempt to read file!" << std::endl;
			return -1;
		}

		// Предлагаем сыграть ещё раз
		switch (restart_offer()){
			case 1:
				std::cout << std::endl;
				break;
			case 0:
				return 0;
				break;
			case -1:
				return -1;
				break;
		}
	}while(1);

	return 0;
}