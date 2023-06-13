#pragma once

// starkeys.cpp
int get_max_num(int const& paramnum, char** params, int const& max_upplim, std::string const& r_fname);

// game.cpp
int game(int const& maxnum, std::string& usrname, bool flg);
int restart_offer();