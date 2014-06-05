#include"SaveLoad.h"
using namespace std;

//LOAD-SAVE FUNCTIONS
bool saves_exist() {
	ifstream file;
	file.open("save_games.txt");
	if (file.is_open()) {return true;}
	else {return false;}
}
void new_player(Player* &player) {
	player->set_player();
	if (player->get_player_class() == "Warrior") {
		player = new Warrior(player->get_name());
		player->set_player();
	}
	else if (player->get_player_class() == "Rogue") {
		player = new Rogue(player->get_name());
		player->set_player();
	}
	else if (player->get_player_class() == "Mage") {
		player = new Mage(player->get_name());
		player->set_player();
	}
	else {cout << "Oops! (new_player)" << endl;}
}
void save_game(Player* &player) {
	string filename, filename_short;
	fstream file;
	save_game_start:
	Clear();
	cout << "To create a save game, enter a name: ";
	getline(cin, filename_short);
	
	//Takes input filename and adjusts it to include the player's level
	//Stores in filename (separate from filename_short)
	int player_level = player->get_level();
	stringstream ss;
	ss << filename_short << ", Level " << player_level;
	filename = ss.str();
	
	//loads in existing saves for use in string manipulation later in
	const int NUM_OF_SAVES = 3;
	string saves[NUM_OF_SAVES], saves_short[NUM_OF_SAVES];
	string line;
	file.open("save_games.txt");
	int saves_count = 0;
	for (int i = 0; file.good(); i++) {
		getline(file, line);
		if (line != "") {
			saves_count++;
			saves[i] = line;
		}
	}
	file.close();
	string junk;
	file.open("save_games.txt");
	for (int i = 0; file.good(); i++) {
		getline(file, line, ',');
		getline(file, junk);
		if (line != "") {saves_short[i] = line;}
	}
	file.close();
	
	//checks if a save game exists with the inputted name
	bool save_exists = false;
	int save_index = 99;
	for (int i = 0; i < NUM_OF_SAVES; i++) {
		if (saves_short[i] == filename_short) {
			save_exists = true;
			save_index = i;
		}
	}
	
	if (save_exists) {
		cout << "A game already exists with this name. Overwrite (Y/N)? ";
		char decision;
		while (true) {
			decision = GetChar();
			if (decision == 'Y' || decision == 'y') {
				file.open("save_games.txt", fstream::out);
				for (int i = 0; i < NUM_OF_SAVES; i++) {
					if (i != save_index) {file << saves[i] << endl;}
					else {file << filename << endl;}
				}
				file.close();
				goto save_game_saving;
			}
			else if (decision == 'N' || decision == 'n') {goto save_game_start;}
			else {cout << "Please enter Y or N: ";}
		}
	}
	else {
		file.open("save_games.txt", fstream::out |  fstream::app);
		file << filename_short << ", Level " << player_level << endl;
		file.close();
	}
	
	save_game_saving:
	cout << "\nSaving game...." << endl;
	
	//attributes and stats
	int *base_attribs = player->get_base_attribs();
	int *exp_gold_inv_count = player->get_exp_gold_inv_count();
	const int BASE_ATTRIB_COUNT = player->get_base_attrib_count();
	const int MISC_COUNT = player->get_misc_count();
	AchievementDatabase game_achievements = player->get_game_achievements();
	file.open(filename_short + "_int.txt", fstream::out);
	for (int i = 0; i < BASE_ATTRIB_COUNT; i++) {file << base_attribs[i] << endl;}
	for (int i = 0; i < MISC_COUNT; i++) {file << exp_gold_inv_count[i] << endl;}
	file << game_achievements.get_earned_count() << endl;
	file.close();
	file.open(filename_short + "_char.txt", fstream::out);
	file << player->get_name() << endl;
	file << player->get_player_class() << endl;
	file.close();
	
	//quests - always MAX_LEVEL number
	bool *boss_quest_completion = player->get_boss_quests_complete();
	file.open(filename_short + "_int.txt", fstream::out | fstream::app);
	for (int i = 0; i < MAX_LEVEL; i++) {
		if (boss_quest_completion[i]) {file << 1 << endl;}
		else {file << 0 << endl;}
	}
	file.close();
	
	//achievements - only if there are any
	if (game_achievements.get_earned_count() > 0) {
		Achievement *earned = game_achievements.get_earned_achievements();
		for (int i = 0; i < game_achievements.get_earned_count(); i++) {
			file.open(filename_short + "_char.txt", fstream::out | fstream::app);
			file << earned[i].get_type() << endl;
			file.close();
			file.open(filename_short + "_int.txt", fstream::out | fstream::app);
			file << earned[i].get_rank() << endl;
			file.close();
		}
	}
	
	//inventory - only if there is any
	if (exp_gold_inv_count[MISC_COUNT - 1] > 0) {
		Item *item = player->get_inventory();
		for (int i = 0; i < exp_gold_inv_count[MISC_COUNT - 1]; i++) {
			file.open(filename_short + "_char.txt", fstream::out | fstream::app);
			file << item[i].get_type() << endl;
			file << item[i].get_subtype() << endl;
			file.close();
			file.open(filename_short + "_int.txt", fstream::out | fstream::app);
			file << item[i].get_level() << endl;
			file << item[i].get_quantity() << endl;
			file.close();
		}
	}
	
	//equipment
	const int EQUIPMENT_SLOTS = player->get_equipment_slots();
	Item *equipment = new Item[EQUIPMENT_SLOTS];
	equipment = player->get_equipment();
	for (int i = 0; i < EQUIPMENT_SLOTS; i++) {
		file.open(filename_short + "_char.txt", fstream::out | fstream::app);
		file << equipment[i].get_type() << endl;
		file << equipment[i].get_subtype() << endl;
		file.close();
		file.open(filename_short + "_int.txt", fstream::out | fstream::app);
		file << equipment[i].get_level() << endl;
		file.close();
	}
	
	cout << filename << " has been saved." << endl;
	Pause();
}
void load_game(Player* &player) {
	Clear();
	cout << "Please choose a game to load:" << endl;
	
	//display list of saved games
	ifstream file;
	string line;
	const int NUM_OF_SAVES = 3;
	string saves[NUM_OF_SAVES], saves_short[NUM_OF_SAVES];
	file.open("save_games.txt");
	int saves_count = 0;
	for (int i = 0; file.good(); i++) {
		getline(file, line);
		if (line != "") {
			cout << i + 1 << ". " << line << endl;
			saves_count++;
			saves[i] = line;
		}
	}
	file.close();
	file.open("save_games.txt");
	for (int i = 0; file.good(); i++) {
		string junk;
		getline(file, line, ',');
		getline(file, junk);
		if (line != "") {saves_short[i] = line;}
	}
	file.close();
	
	string filename;
	int choice;
	while (true) {
		choice = GetInt();
		choice--;
		if (choice < saves_count && choice >= 0) {
			filename = saves_short[choice];
			cout << "\nLoading " << filename << "...." << endl;
			break;
		}
		else {cout << "Please enter a number between 1 and " << saves_count << ": ";}
	}
	
	//attributes and stats
	file.open(filename + "_char.txt");
	char c[30];
	string name, loaded_class;
	for (int i = 0; file.good(); i++) {
		file.getline(c, 30, '\n');
		if (i == 0) {name = c;}
		if (i == 1) {
			loaded_class = c;
			if (loaded_class == "Warrior") {player = new Warrior(name);}
			else if (loaded_class == "Rogue") {player = new Rogue(name);}
			else if (loaded_class == "Mage") {player = new Mage(name);}
			else {cout << "Oops! (load_game -> setting player class)" << endl;}
		}
	}
	file.close();
	int char_count = 2; //after establishing initial char strings, use this for offset
	
	int x, int_count;
	const int BASE_ATTRIB_COUNT = player->get_base_attrib_count();
	const int MISC_COUNT = player->get_misc_count();
	int *base_attribs = new int[BASE_ATTRIB_COUNT];
	int *exp_gold_inv_count = new int[MISC_COUNT];
	
	AchievementDatabase game_achievements = player->get_game_achievements();
	
	bool *boss_quest_completion = new bool[MAX_LEVEL];
	file.open(filename + "_int.txt");
	int_count = 0;
	for (int i = 0; i < BASE_ATTRIB_COUNT; i++) {
		file >> x;
		base_attribs[i - int_count] = x;
	}
	int_count += BASE_ATTRIB_COUNT; //adjusts int counter
	for (int i = int_count; i < int_count + MISC_COUNT; i++) {
		file >> x;
		exp_gold_inv_count[i - int_count] = x;
	}
	int_count += MISC_COUNT; //adjusts int counter
	
	//achievement count
	file >> x;
	int earned_count = x;
	int_count += 1;
	
	//quests - always MAX_LEVEL number
	for (int i = int_count; i < int_count + MAX_LEVEL; i++) {
		file >> x;
		if (x == 1) {boss_quest_completion[i - int_count] = true;}
		else {boss_quest_completion[i - int_count] = false;}
	}
	file.close();
	int_count += MAX_LEVEL; //adjusts int counter
	
	int j = 0; //used for 2nd-level loops
	
	//achievements
	Achievement *earned;
	if (earned_count > 0) {
		earned = new Achievement[earned_count];
		for (j = 0; j < earned_count; j++) {
			file.open(filename + "_char.txt");
			for (int i = 0; file.good(); i++) {
				file.getline(c, 30, '\n');
				if (i >= char_count && i < char_count + earned_count) {
					if ((i - char_count) % earned_count == j) {earned[j].load_type(c);}
				}
			}
			file.close();
		}
		char_count += earned_count; //adjusts char offset for following iterations
		
		for (j = 0; j < earned_count; j++) {
			file.open(filename + "_int.txt");
			for (int i = 0; file.good(); i++) {
				file >> x;
				if (i >= int_count && i < int_count + earned_count) {
					if ((i - int_count) % earned_count == j) {earned[j].load_rank(x);}
				}
			}
			file.close();
		}
		int_count += earned_count; //adjusts int offset for following iterations
		
		for (int i = 0; i < earned_count; i++) {
			earned[i].init_achievement(earned[i].get_type(), earned[i].get_rank());
		}
	}
	
	//inventory
	int int_length, char_length; //number of inventory lines
	Item *inv;
	if (exp_gold_inv_count[MISC_COUNT - 1] > 0) {
		//last index of exp_gold_inv_count[] is the number of items in inventory
		//char and int lengths each number 2, multiplied by the number of items in inventory
		char_length = exp_gold_inv_count[MISC_COUNT - 1] * 2;
		int_length = exp_gold_inv_count[MISC_COUNT - 1] * 2;
		inv = new Item[exp_gold_inv_count[MISC_COUNT - 1]];
		
		for (j = 0; j < exp_gold_inv_count[MISC_COUNT - 1]; j++) {
			file.open(filename + "_char.txt");
			for (int i = 0; file.good(); i++) {
				file.getline(c, 30, '\n');
				if ((i >= char_count) && (i < char_count + char_length)) { //ensures it stays within inventory bounds
					if ((i - char_count) % char_length == 2 * j) {inv[j].load_type(c);}
					if ((i - char_count) % char_length == (2 * j) + 1) {inv[j].load_subtype(c);}
				}
			}
			file.close();
		}
		
		for (j = 0; j < exp_gold_inv_count[MISC_COUNT - 1]; j++) {
			file.open(filename + "_int.txt");
			for (int i = 0; file.good(); i++) {
				file >> x;
				if ((i >= int_count) && (i < int_count + int_length)) { //ensures it stays within inventory bounds
					if ((i - int_count) % int_length == 2 * j) {inv[j].load_level(x);}
					if ((i - int_count) % int_length == (2 * j) + 1) {inv[j].load_quantity(x);}
				}
			}
			file.close();
		}

		for (int i = 0; i < exp_gold_inv_count[MISC_COUNT - 1]; i++) {
			int num = inv[i].get_quantity(); //backup quantity
			inv[i].set_item(inv[i].get_type(), inv[i].get_subtype(), inv[i].get_level()); //init_item
			inv[i].load_quantity(num); //restore original quantity
		}
		
	}
	//if no inventory
	else {char_length = int_length = 0;}
	
	char_count += char_length; //adjusts char offset for further iterations
	int_count += int_length; //adjusts int offset for further iterations
	
	//equipment
	const int EQUIPMENT_SLOTS = player->get_equipment_slots();
	Item *equipment = new Item[EQUIPMENT_SLOTS];
	
	//copy in equipment details
	for (j = 0; j < EQUIPMENT_SLOTS; j++) {
		file.open(filename + "_char.txt");
		for (int i = 0; file.good(); i++) {
			file.getline(c, 30, '\n');
			if (i >= char_count) {
				if ((i - char_count) % (EQUIPMENT_SLOTS * 2 + 1) == 2 * j) {equipment[j].load_type(c);}
				if ((i - char_count) % (EQUIPMENT_SLOTS * 2 + 1) == (2 * j) + 1) {equipment[j].load_subtype(c);}
			}
		}
		file.close();
	}
	for (j = 0; j < EQUIPMENT_SLOTS; j++) {
		file.open(filename + "_int.txt");
		for (int i = 0; file.good(); i++) {
			file >> x;
			if (i >= int_count) {
				if ((i - int_count) % 4 == j) {equipment[j].load_level(x);}
			}
		}
		file.close();
	}
	
	//load player components into class object
	player->load_player(base_attribs, exp_gold_inv_count, boss_quest_completion, equipment);
	if (earned_count > 0) {
		game_achievements.load_achievements(earned, earned_count);
		player->load_achievements(earned, earned_count);
	}
	else {player->init_achievements();}
	if (exp_gold_inv_count[MISC_COUNT - 1] > 0) {player->load_inventory(inv);}
	
	player->update_stats(); //also handles equipment bonuses
	player->restore_health_to_max();
	if (player->get_player_class() == "Mage") {
		player->restore_mana_to_max();
		player->init_spellbook();
		player->update_stats();
	}
	player->set_quests();

	cout << "Your game has been loaded." << endl;
	cout << "Welcome back, " << player->get_name() << "!" << endl;
	
	Pause();
}
void choose_player(Player* &player) {
	Clear();
	if (saves_exist()) {
		cout << "What would you like to do?" << endl;
		cout << "(L)oad player" << endl;
		cout << "(N)ew player" << endl;
		char choice;
		while (true) {
			choice = GetChar();
			switch (choice) {
				case 'L':
				case 'l':
					load_game(player);
					return;
					break;
				case 'N':
				case 'n':
					new_player(player);
					return;
					break;
				default:
					cout << "Please enter L or N: ";
					break;
			}
		}
	}
	else {new_player(player);}
}
void save_or_load(Player* &player) {
	save_or_load_start:
	Clear();
	cout << "What would you like to do?" << endl;
	int count = 1;
	cout << count << ". Save player" << endl;
	count++;
	cout << count << ". Load player" << endl;
	count++;
	cout << count << ". Exit menu" << endl;
	
	int choice;
	while (true) {
		choice = GetInt();
		switch (choice) {
			case 1:
				save_game(player);
				goto save_or_load_start;
				break;
			case 2:
				load_game(player);
				goto save_or_load_start;
				break;
			case 3:
				return;
				break;
			default:
				cout << "Please enter a number between 1 and " << count << ": ";
				break;
		}
	}
}