#include<iostream>
#include<string>
#include"Item.h"
#include"Player.h"
using namespace std;

#ifndef SHOP_H
#define SHOP_H

//shop definition and subtypes
class Shop {
protected:
	//VARIABLES
	Item *inventory;
	string display_inventory_flavor, buy_flavor, sell_flavor, entrance_flavor, leave_flavor, menu_flavor;
	int size_of_inventory, level;
	
	//shop inventory
	void display_inventory();

	//BUYING AND SELLING
	void buy(Player*&);
	void sell(Player*&);
};
class Blacksmith: public Shop {
public:
	Blacksmith(int);

	//top level menu
	void menu(Player*&);
	
private:
	//initializes inventory
	void init_inventory();
	
	void repair(Player*&);
};
class Inn: public Shop {
public:
	Inn(int);
	
	//top level menu
	void menu(Player*&);
	
private:
	//VARIABLES
	Item room;
	
	//INITIALIZATION
	void init_inventory();
	void init_room() {room.set_item("Room", "Room", level);}
	
	//room menu
	void stay(Player*&);
};

#endif