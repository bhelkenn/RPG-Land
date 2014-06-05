#include"Shop.h"
using namespace std;

//CONSTRUCTORS
Blacksmith::Blacksmith(int shop_level) {
	level = shop_level;
	init_inventory();
	display_inventory_flavor = "Blacksmith: 'Here's what I have to offer:'";
	buy_flavor = "As you wander over to the equipment the blacksmith is selling, you notice ";
	buy_flavor += "a wide variety of weapons and armor, from pathetic to top-notch. The blacksmith ";
	buy_flavor += "notices you staring at a diamond-encrusted hilt and slides the weapon under the ";
	buy_flavor += "table. 'You'll need to prove you can use it before you can buy it, bub,' he says.";
	sell_flavor = "Blacksmith: 'What would you like to sell to me?' ";
	entrance_flavor = "You enter the blacksmith. The proprietor notices you enter.";
	menu_flavor = "Blacksmith: 'Welcome to my shop! What would you like?'";
	leave_flavor = "You decide to leave the blacksmith's shop. ";
	leave_flavor += "The proprietor scowls at your retreating back.";
}
Inn::Inn(int shop_level) {
	level = shop_level;
	init_inventory();
	init_room();
	display_inventory_flavor = "Innkeeper: 'Here's what I have to offer:'";
	sell_flavor = "Innkeeper: 'What would you like to do to - I mean, sell me?' ";
	entrance_flavor = "You enter the inn. A buxom maiden notices your entrance and sashays over.";
	menu_flavor = "Innkeeper: 'Welcome to my inn!'";
	leave_flavor = "You decide to leave, possibly put off by the innkeeper's come-hither stares. ";
	leave_flavor += "She pouts after you as you leave.";
}

//INITIALIZATION
void Blacksmith::init_inventory() {
	if (level < MAX_LEVEL && level > 0) {
		size_of_inventory = 16;
		inventory = new Item[size_of_inventory];
		inventory[0].set_item("Weapon", "Sword", level);
		inventory[1].set_item("Weapon", "Sword", level + 1);
		inventory[2].set_item("Weapon", "Dagger", level);
		inventory[3].set_item("Weapon", "Dagger", level + 1);
		inventory[4].set_item("Weapon", "Staff", level);
		inventory[5].set_item("Weapon", "Staff", level + 1);
		inventory[6].set_item("Armor", "Cloth", level);
		inventory[7].set_item("Armor", "Cloth", level + 1);
		inventory[8].set_item("Armor", "Leather", level);
		inventory[9].set_item("Armor", "Leather", level + 1);
		inventory[10].set_item("Armor", "Mail", level);
		inventory[11].set_item("Armor", "Mail", level + 1);
		inventory[12].set_item("Shield", "Shield", level);
		inventory[13].set_item("Shield", "Shield", level + 1);
		inventory[14].set_item("Offhand", "Orb", level);
		inventory[15].set_item("Offhand", "Orb", level + 1);
	}
	else if (level == MAX_LEVEL) {
		size_of_inventory = 8;
		inventory = new Item[size_of_inventory];
		inventory[0].set_item("Weapon", "Sword", level);
		inventory[1].set_item("Weapon", "Dagger", level);
		inventory[2].set_item("Weapon", "Staff", level);
		inventory[3].set_item("Armor", "Cloth", level);
		inventory[4].set_item("Armor", "Leather", level);
		inventory[5].set_item("Armor", "Mail", level);
		inventory[6].set_item("Shield", "Shield", level);
		inventory[7].set_item("Offhand", "Orb", level);
	}
	else {cout << "Invalid level (blacksmith)" << endl;}
}
void Inn::init_inventory() {
	if (level < MAX_LEVEL) {
		size_of_inventory = 4;
		inventory = new Item[size_of_inventory];
		inventory[0].set_item("Potion", "Health", level);
		inventory[1].set_item("Potion", "Health", level + 1);
		inventory[2].set_item("Potion", "Mana", level);
		inventory[3].set_item("Potion", "Mana", level + 1);
	}
	else {
		size_of_inventory = 2;
		inventory = new Item[size_of_inventory];
		inventory[0].set_item("Potion", "Health", level);
		inventory[1].set_item("Potion", "Mana", level);
	}
}

//top level menu
void Blacksmith::menu(Player* &player) {
	cout << entrance_flavor << endl;
	Pause();
	menu_start:
	Clear();
	cout << menu_flavor << endl << endl;
	
	int count = 1;
	cout << count << ". Buy / Browse goods" << endl;
	count++;
	cout << count << ". Sell" << endl;
	count++;
	cout << count << ". Repair" << endl;
	count++;
	cout << count << ". Leave blacksmith" << endl;
	
	int choice;
	while (true) {
		choice = GetInt();
		switch (choice) {
			case 1:
				Clear();
				cout << buy_flavor << endl;
				Pause();
				buy(player);
				goto menu_start;
				break;
			case 2:
				sell(player);
				goto menu_start;
				break;
			case 3:
				repair(player);
				goto menu_start;
				break;
			case 4:
				cout << leave_flavor << endl;
				Pause();
				return;
				break;
			default:
				cout << "Please enter a number between 1 and " << count << ": ";
				break;
		}
	}
}
void Inn::menu(Player* &player) {
	Clear();
	cout << entrance_flavor << endl;
	menu_start:
	cout << menu_flavor << endl;
	cout << "What would you like to do?\n" << endl;
	
	int count = 1;
	cout << count << ". Buy / Browse goods" << endl;
	count++;
	cout << count << ". Sell" << endl;
	count++;
	cout << count << ". Stay the night" << endl;
	count++;
	cout << count << ". Leave inn" << endl;
	
	int choice;
	while (true) {
		choice = GetInt();
		switch (choice) {
			case 1:
				buy(player);
				Clear();
				goto menu_start;
				break;
			case 2:
				sell(player);
				Clear();
				goto menu_start;
				break;
			case 3:
				stay(player);
				Clear();
				goto menu_start;
				break;
			case 4:
				cout << leave_flavor << endl;
				Pause();
				return;
				break;
			default:
				cout << "Please enter a number between 1 and " << count << ": ";
				break;
		}
	}
}

//shop inventory
void Shop::display_inventory() {
	Clear();
	cout << display_inventory_flavor << endl << endl;
	for (int i = 0; i < size_of_inventory; i++) {
		cout << i + 1 << ". " << inventory[i].get_name() << " (Requires Level "
		<< inventory[i].get_level() << ") - " << inventory[i].get_buy_value() << " gold" << endl;
	}
}

//BUYING AND SELLING
void Shop::buy(Player* &player) {
	buy_start:
	display_inventory();
	cout << size_of_inventory + 1 << ". Exit menu\n" << endl;		
	player->view_gold();
	
	int choice;
	while (true) {
		choice = GetInt();
		choice--;
		if (choice < size_of_inventory && choice >= 0) {
			if (player->get_gold() >= inventory[choice].get_buy_value()) {
				int selection_choice;
				while (true) {
					Clear();
					cout << "You've selected " << inventory[choice].get_name() << endl;
					cout << "What would you like to do?" << endl;
					int count = 1;
					cout << count << ". Purchase" << endl;
					count++;
					cout << count << ". View detailed information" << endl;
					count++;
					cout << count << ". Cancel request" << endl;
					
					buy_choice:
					selection_choice = GetInt();
					switch (selection_choice) {
						case 1:
							player->inc_inventory(inventory[choice]);
							player->remove_gold(inventory[choice].get_buy_value());
							cout << "You've purchased " << inventory[choice].get_name()
							<< " for " << inventory[choice].get_buy_value() << " gold." << endl;
							Pause();
							goto buy_start;
							break;
						case 2:
							inventory[choice].view_stats(player->get_player_class());
							continue;
							break;
						case 3:
							goto buy_start;
							break;
						default:
							cout << "Please enter a number between 1 and " << count << ": ";
							goto buy_choice;
							break;
					}
				}
			}
			else {cout << "You don't have enough gold." << endl;}
			Pause();
			goto buy_start;
		}
		else if (choice == size_of_inventory) {return;}
		else {cout << "Please enter a number between 1 and " << size_of_inventory + 1 << ": ";}
	}
}
void Shop::sell(Player* &player) {
	sell_menu_start:
	Clear();
	if (player->get_items_in_inventory() == 0) {
		cout << "You don't have any items to sell." << endl;
		Pause();
		return;
	}
	else {
		Item *inv = player->get_inventory();
		cout << sell_flavor << endl << endl;
		for (int i = 0; i < player->get_items_in_inventory(); i++) {
			cout << i + 1 << ". " << inv[i].get_name() << " x" << inv[i].get_quantity()
			<< " - " << inv[i].get_sell_value() << " gold" << endl;
		}
		cout << player->get_items_in_inventory() + 1 << ". Exit menu\n" << endl;
		
		int choice;
		while (true) {
			choice = GetInt();
			choice--;
			if (choice < player->get_items_in_inventory() && choice >= 0) {
				cout << "You would like to sell " << inv[choice].get_name()
				<< " for " << inv[choice].get_sell_value() << " gold. Is that "
				<< "acceptable (Y/N)?" << endl;
				char decision;
				while (true) {
					decision = GetChar();
					switch (decision) {
						case 'Y':
						case 'y':
							player->dec_inventory(inv[choice]);
							player->add_gold(inv[choice].get_sell_value());
							cout << "You have sold " << inv[choice].get_name() << " for "
							<< inv[choice].get_sell_value() << " gold.\n" << endl;
							Pause();
							goto sell_menu_start;
							break;
						case 'N':
						case 'n':
							goto sell_menu_start;
							break;
						default:
							cout << "Please enter Y or N: " << endl;
					}
				}
			}
			else if (choice == player->get_items_in_inventory()) {return;}
			else {cout << "Please enter a number between 1 and " << player->get_items_in_inventory() + 1 << ": ";}
		}
		//purge pointer to prevent memory leak
		delete[] inv;
		inv = 0;
	}
}

//SPECIALTY
void Blacksmith::repair(Player* &player) {
	Item *equipment = player->get_equipment();
	const int EQUIPMENT_SLOTS = player->get_equipment_slots();
	int cost;
	int choice;
	char decision;
	while (true) {
		Clear();
		bool no_dmg = true;
		for (int i = 0; i < EQUIPMENT_SLOTS; i++) {
			if (equipment[i].get_level() > 0 && equipment[i].get_damage() > 0) {no_dmg = false;}
		}
		if (no_dmg) {
			cout << "You don't have any equipment that needs repairing..." << endl;
			Pause();
			return;
		}
		cout << "Which item would you like to repair?\n" << endl;
		player->view_equipment();
		cout << EQUIPMENT_SLOTS + 1 << ". Cancel" << endl;
		
		repair_choice:
		choice = GetInt();
		choice--;
		if (choice < EQUIPMENT_SLOTS && choice >= 0) {
			if (equipment[choice].get_level() == 0 || equipment[choice].get_damage() == 0) {
				cout << "This item doesn't require repairing." << endl;
				Pause();
				continue;
			}
			cost = (int)((equipment[choice].get_damage() / 100.0) * equipment[choice].get_buy_value());
			cout << "It will cost " << cost << " to repair " << equipment[choice].get_name() << ". ";
			cout << "Is this acceptable (Y/N)? ";
			decision = GetChar();
			while (true) {
				if (decision == 'Y' || decision == 'y') {
					player->repair(choice);
					player->remove_gold(cost);
					cout << equipment[choice].get_name() << " has been repaired." << endl;
					Pause();
					break;
				}
				else if (decision == 'N' || decision == 'n') {break;}
				else {cout << "Please enter Y or N: ";}
			}
		}
		else if (choice == EQUIPMENT_SLOTS) {return;}
		else {
			cout << "Please enter a number between 1 and " << EQUIPMENT_SLOTS + 1 << ": ";
			goto repair_choice;
		}
	}
}
void Inn::stay(Player* &player) {
	Clear();
	cout << "Innkeeper: 'Our " << room.get_name() << " is available to rent for " << room.get_buy_value()
	<< " gold. We also offer 'specials' for some of our more...distinguished guests.'\n" << endl;
	cout << "You look in the " << room.get_name() << ". " << room.get_description() << endl << endl;
	player->view_gold();
	cout << "\nWould you like to stay the night (Y/N)? ";
	
	char choice;
	while (true) {
		choice = GetChar();
		if (choice == 'Y' || choice == 'y') {
			if (player->get_gold() >= room.get_buy_value()) {
				player->remove_gold(room.get_buy_value());
				player->restore_health_to_max();
				if (player->get_player_class() == "Mage") {player->restore_mana_to_max();}
				Clear();
				cout << "You've stayed in the " << room.get_name() << ", and your health and mana have "
				<< "recovered fully. Anything else, such as certain people sneaking into your room in the middle "
				<< "of the night, you'd rather not think about too much." << endl;
			}
			else {cout << "You don't have enough gold." << endl;}
			Pause();
			return;
		}
		else if (choice == 'N' || choice == 'n') {
			cout << "You decided not to stay the night. Perhaps it had something to do with how the innkeeper has been "
			<< "staring at you this entire time...." << endl;
			Pause();
			return;
		}
		else {cout << "Please enter Y or N: ";}
	}
}