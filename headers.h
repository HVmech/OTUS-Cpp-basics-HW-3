#pragma once

// starkeys.cpp
int get_max_num(int paramnum, char** params, int max_upplim, std::string const& r_fname);

// game.cpp
int game(int maxnum, std::string& usrname, bool flg);
int restart_offer();