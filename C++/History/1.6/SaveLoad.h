#include<fstream> //used for saving and loading player data
#include<string>
#include<sstream>
#include<iostream>
#include"Player.h"

#ifndef SAVELOAD_H
#define SAVELOAD_H

bool saves_exist();
void new_player(Player*&);
void save_game(Player*&);
void load_game(Player*&);
void choose_player(Player*&);
void save_or_load(Player*&);

#endif