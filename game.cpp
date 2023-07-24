#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "headers.h"

int game(int maxnum, std::string& usrname, bool flg){
    if(flg) {
        // Блок знакомства с пользователем
        std::cout << "Welcome to 'Guess the number' game!" << std::endl;
        std::cout << "Enter your username:" << std::endl;

        std::getline(std::cin, usrname);
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        std::cout << "Ok, let's go! :)" << std::endl;
    }

	std::cout << "Guess the number from 1 up to " << maxnum << ":" << std::endl;
	int targnum = std::rand() % maxnum + 1;
	int attnum = 0;
    int usrnum;

	do {
		std::cin >> usrnum;
			++attnum;

			if (usrnum > targnum) {
				std::cout << "It's less than " << usrnum << std::endl;
			}
			else if (usrnum < targnum) {
				std::cout << "It's greater than " << usrnum << std::endl;
			}
			else{
				std::cout << "Congratulations, " << usrname << "! You win! It's " << targnum << "!" << std::endl;
				break;
			}
        if(attnum > maxnum) return -1;
    } while(true);

    return attnum;
}

int restart_offer(){
    std::string responce;

    // Блок предложения повторной игры
	for (int i=1; ; ++i){
		std::cout << "Would you like to play again? [y/n]:" << std::endl;
		std::cin >> responce;

		if (responce == "y"){
			std::cout << "Good :)" << std::endl << "Let's start!" << std::endl;
			return 1;
		}
		if (responce == "n"){
			std::cout << "Ok :(" << std::endl << "C u!" << std::endl;
		    return 0;
		}
		if (i > 2){
			std::cout << "We can't understand each other. Bye!" << std::endl;
			return -1;
		} else{
			std::cout << "I can't understand your answer. Please, enter correct one..." << std::endl;
		}
	}
}