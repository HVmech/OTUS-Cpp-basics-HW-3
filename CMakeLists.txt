cmake_minimum_required(VERSION 3.5)

project(guess_the_number)

add_executable(guess_the_number
	startkeys.cpp
	game.cpp
	guess_the_number.cpp
)

set_target_properties(
	guess_the_number
PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)
