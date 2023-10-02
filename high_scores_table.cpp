#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include "headers.h"

bool update_file(std::fstream& file, std::string const& filename, std::string const& usrname, int attnum)
{
    // Подготавливаем файл к чтению/записи
    // Проверяем, открыт ли файл
    // Если файл не открылся, то аварийно завершаем функцию
    if (!file.is_open())
    {
        file.open(filename);
	    
        if (!file.is_open())
	{
	     std::cout << "Failed attempt to open file: " << filename << "!" << std::endl;
		
	     return 0;
        }
    }

    // Инициализируем значения внутренних переменных
    int strnum = 1;
    int rewr = 0;
    int char_count = 0;
    int atttemp;
    std::string tempname;
    std:: string str1, str2;
    size_t pos;
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
    std::cout << usrname << std::endl;
	
    while(1)
    {
        file >> tempname;
	    
        std::cout << tempname << std::endl;
	    
	file >> atttemp;
	    
	if(file.fail()) break;
	    
	file.ignore();
	
	if(tempname == usrname)
	{
	    if(atttemp > attnum)
	    {
	        rewr = strnum;
	    }
	    else
	    {
		rewr = strnum*(-1);
	    }
	}
	strnum++;
    }

    std::cout << "Test #1" << std::endl;
    std::cout << "rewr = " << rewr << std::endl;
    std::cout << "strnum = " << strnum << std::endl;
    
    // Закрываем файл, т.к. он завершился аварийно и более не нужен
    file.close();

    // Снова открываем тот же файл
    file.open(filename);

    // Переносим указатель в начало файла
    file.seekp(0, std::ios_base::beg);

    // Проверка, открыт ли файл
    if (!file.is_open())
    {
	std::cout << "Failed attempt to open file:  " << filename << "!" << std::endl;
	    
	return 0;
    }

    std::cout << "Test #2" << std::endl;
    std::cout << "Ready to analyse file" << std::endl;
    std::cout << "pos = " << file.tellp() << std::endl;

    // Если результат пользователя улучшился и файл необходимо изменить rewr > 0:
    if(rewr > 0)
    {
	// Доходим до искомой строки в файле
	for(int i = 1; i < rewr; i++)
	{
	    std::getline(file, str1);
	}

	pos = size_t(file.tellp());
			
	// Перемещаем строку в конец файла
	for(int i = rewr; i < strnum-1; i++)
	{
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

        // Перезаписываем последнюю строку файла
	file << usrname << " " << attnum << std::endl;

	file.flush();

        // Измеряем размер файла
        filesize = std::filesystem::file_size(filename);

        // Считаем мусорные символы в конце файла, если таковые есть
        while(file.get())
	{
            char_count++;
	    if(file.fail()) break;			
	}

        // Закрываем файл
        file.close();

        // Выкидываем мусор из конца файла
        filesize = filesize - char_count+1;
        std::filesystem::resize_file(filename, filesize);
        return 1;
    }
    else
    {
    // Если игрок ранее не играл в игру
	if(!rewr)
	{
            // Добавляем строку с именем пользователя и результатами в конец файла
	    file.seekp (0, file.end);
	    file << usrname << " " << attnum << std::endl;
	}
	    
        // Если игрок не улучшил счет или строка с результатами была добавлена, закрываем файл
        file.close();
        return 1;
    }
}

bool read_file(std::fstream& file, const std::string filename)
{
    // Открываем файл
    file.close();
    file.open(filename);

    // Проверка, открыт ли файл
    if (!file.is_open())
    {
	std::cout << "Failed attempt to open file:  " << filename << "!" << std::endl;
	return 0;
    }

    // Инициализируем строку-буфер
    std::string buff;

    std::cout << "High scores table:" << std::endl;
    // Выводим содержимое файла в консоль
    while(1)
    {
        std::getline(file, buff);
	    
        std::cout << buff << std::endl;;
	    
        file.ignore();
	    
        if(file.fail()) break;
    }

    return 1;
}
