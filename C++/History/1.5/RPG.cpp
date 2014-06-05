#include<iostream>
#include<string>
#include<sstream> //used for GetInt() flavor function
#include<fstream> //used for saving and loading player data
#include<time.h> //used for various RNG events, like creature encounters and town placement
#include<windows.h> //used for Clear() flavor function
using namespace std;

static const int MAX_LEVEL = 5; //reduces complexity significantly by reducing the number of player calls

//flavor functions
void Pause() {
	cout << "Press ENTER to continue...";
	char c[15];
	cin.getline(c, 15);
}
void Clear() {
	HANDLE						hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO	csbi;
	DWORD						count;
	DWORD						cellCount;
	COORD						homeCoords = {0, 0};

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) {return;}

	//Get the number of cells in the current buffer
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) {return;}
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;

	//Fill the entire buffer with spaces
	if (!FillConsoleOutputCharacter(hStdOut, (TCHAR) ' ', cellCount, homeCoords, &count)) {return;}

	//Fill the entire buffer with the current colors and attributes
	if (!FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count)) {return;}

	//Move the cursor home
	SetConsoleCursorPosition(hStdOut, homeCoords);
}
int GetInt() {
	int num = 0;
	string input;
	while (true) {
		getline(cin, input);
		//convert from string to number safely.
		stringstream ss(input);
		if (ss >> num) {break;}
		cout << "Invalid entry, please try again: ";
	}
	return num;
}
char GetChar() {
	char c = 0;
	string input;
	while (true) {
		getline(cin, input);
		//verifies there's just a single char
		if (input.length() == 1) {
			c = input[0];
			break;
		}
		cout << "Invalid entry, please try again: ";
	}
	return c;
}

//damage calculation
int DamageCalculation(double atk, double def, double max_health, double crit, string name) {
	const double DMG_VARIANCE = 0.3;
	const double STARTING_POINT = 0.1; //starting point. Ex. if ATK = 90, dmg = 90 * STARTING_POINT
	
	double adj_atk = atk / atk; //Since ATK is the variable around which damage is determined, it's forced to 1.
	double dmg = atk * STARTING_POINT;
	double shift = (atk - def) / atk; //adjusts shift to match the same ratio as adj_atk
	
	//shifts damage according to disparity between ATK and DEF
	if (shift == 0) {} //no need to do anything, stays at starting point
	else {
		if (shift < 0) {
			shift *= -1.0;
			dmg *= 1.0 - shift;
		}
		else {dmg += dmg * shift;}
	}
	//establish variance
	int skew = (int)(dmg * (1 - DMG_VARIANCE));
	int result;
	if (skew != (int)dmg) {result = rand() % ((int)dmg - skew) + skew;}
	else {result = skew;}
	
	if (result < 1) {result = 1;} //forces damage to actually occur
	//checks for crits
	if (rand() % 101 < crit) {
		cout << name << " landed a critical hit!" << endl;
		result *= 2;
	}
	return result;
}

class Item {
public:
	Item() {}
	
	void set_item(string item_type, string item_subtype, int item_level) {
		quantity = 1;
		type = item_type;
		subtype = item_subtype;
		level = item_level;
		init_item();
	}
	
	void inc_quantity() {quantity++;}
	void dec_quantity() {quantity--;}

	//GET FUNCTIONS
	string get_name() {return name;}
	string get_type() {return type;}
	string get_subtype() {return subtype;}
	int get_sell_value() {return sell_value;}
	int get_buy_value() {return buy_value;}
	int get_level() {return level;}
	int get_block_bonus() {return block_bonus;}
	int get_atk_bonus() {return atk_bonus;}
	int get_health_bonus() {return health_bonus;}
	int get_quantity() {return quantity;}
	int get_damage() {return (int)(damage_percentage * 100.0);}
	
	//LOAD FUNCTIONS
	void load_name(char *c) {name = c;}
	void load_type(char *c) {type = c;}
	void load_subtype(char *c) {subtype = c;}
	void load_sell_value(int x) {sell_value = x;}
	void load_buy_value(int x) {buy_value = x;}
	void load_level(int x) {level = x;}
	void load_quantity(int x) {quantity = x;}
	
	//BOOLS
	bool equipable() {
		if (type == "Armor" || 
			type == "Weapon" || 
			type == "Shield" || 
			type == "Offhand") {return true;}
		else {return false;}
	}
	bool broken() {
		if (damage_percentage == 1.0) {return true;}
		else {return false;}
	}
	
	//returns value of bonus given by an item
	int stat_value() {return stat_bonus;}
	int attrib_value() {return attrib_bonus;}
	
	//VIEW FUNCTIONS
	void view_stats(string player_class) {
		Clear();
		cout << "Name: " << name << endl;
		cout << "Level: " << level << endl;
		
		if (type == "Armor") {
			cout << "Armor rating: " << def_bonus << endl;
			cout << "Health bonus: " << health_bonus << endl;
			if (subtype == "Mail") {
				cout << "Strength: " << str_bonus << endl;
				cout << "Stamina: " << sta_bonus << endl;
			}
			else if (subtype == "Leather") {
				cout << "Agility: " << agi_bonus << endl;
			}
			else if (subtype == "Cloth") {
				cout << "Intellect: " << int_bonus << endl;
			}
		}
		else if (type == "Shield") {
			cout << "Armor rating: " << def_bonus << endl;
			cout << "Block rating: " << block_bonus << endl;
			cout << "Strength: " << str_bonus << endl;
			cout << "Stamina: " << sta_bonus << endl;
		}
		else if (type == "Weapon") {
			if (subtype == "Sword") {
				if (player_class == "Warrior") {atk_bonus *= 2;}
				cout << "Attack rating: " << atk_bonus << endl;
				cout << "Strength: " << str_bonus << endl;
				cout << "Stamina: " << sta_bonus << endl;
			}
			else if (subtype == "Dagger") {
				if (player_class == "Rogue") {atk_bonus *= 5;}
				cout << "Attack rating: " << atk_bonus << endl;
				cout << "Agility: " << agi_bonus << endl;
			}
			else if (subtype == "Staff") {
				if (player_class == "Mage") {mana_bonus *= 2;}
				cout << "Attack rating: " << atk_bonus << endl;
				cout << "Intellect: " << int_bonus << endl;
				cout << "Mana bonus: " << mana_bonus << endl;
			}
		}
		else if (type == "Offhand") {
			cout << "Intellect: " << int_bonus << endl;
			cout << "Mana bonus: " << mana_bonus << endl;
		}
		else if (type == "Potion") {
			cout << subtype << " yield: " << stat_bonus << endl;
		}
		cout << endl;
		
		cout << description << endl;
		Pause();
	}
	
	//damage and repair
	void damage(int x) {
		damage_percentage += (x / 100.0);
		if (damage_percentage > 1.0) {damage_percentage = 1.0;}
	}
	void repair(int x) {
		damage_percentage -= (x / 100.0);
		if (damage_percentage < 0) {damage_percentage = 0;}
	}
	
private:
	//VARIABLES
	string name, type, subtype, description;
	int sell_value, buy_value, level, quantity, attrib_bonus, stat_bonus;
	int def_bonus, atk_bonus, health_bonus, mana_bonus, block_bonus;
	int str_bonus, agi_bonus, int_bonus, sta_bonus;
	double damage_percentage; //from 0 to 1, determining the amount of repair needed
	
	void init_item() {
		//known: type, subtype, level
		//assumed quantity of 1
		
		//known types: "Armor", "Weapon", "Shield", "Offhand", "Potion", "Room", "Trash", "Boss Drop"
		//known subtypes: "Mail", "Leather", "Cloth", "Sword", "Dagger", "Staff", "Orb", "Health", "Mana", "Shield", "Room", "broken tooth", "hairball",
		//"string", "tail fluff", "lint", "Fractured Tooth", "Poop Nugget", "Torn Claw", "Flea Collar", "Golden Apple"
		//known levels: 1, 2, 3, 4, 5
		//conditions to init name and buy/sell value based on knowns
		
		string s; //used to assemble names
		//name assembly
		if (type == "Armor") {
			if (level > 0) { //validates as a "real" item
				//assemble prefix
				if (level == 1) {s = "Crappy ";}
				else if (level == 2) {s = "Cheap ";}
				else if (level == 3) {s = "Basic ";}
				else if (level == 4) {s = "Expensive ";}
				else if (level == 5) {s = "Luxurious ";}
				else {cout << "Not a valid level (Armor -> name)" << endl;}
			
				//add subtype to prefix
				if (subtype == "Mail") {s += subtype + " ";}
				else if (subtype == "Leather") {s += subtype + " ";}
				else if (subtype == "Cloth") {s += subtype + " ";}
				else {cout << "Not a valid subtype (Armor -> name)" << endl;}
			
				//add type to the end
				s += type;
			
				//this is the name of the item
				name = s;
			}
			//for unequipped slots
			else {name = "empty";}
		}
		else if (type == "Weapon") {
			if (level > 0) { //validates as a "real" item
				//since prefix will change based on the subtype, will need
				//to determine the subtype before beginning name assembly
			
				//determine subtype first
				if (subtype == "Sword" || subtype == "Dagger") {
					//assemble prefix based on subtype
					if (level == 1) {s = "Copper ";}
					else if (level == 2) {s = "Bronze ";}
					else if (level == 3) {s = "Iron ";}
					else if (level == 4) {s = "Steel ";}
					else if (level == 5) {s = "Diamond ";}
					else {cout << "Not a valid level (Sword/Dagger -> name)" << endl;}
					s += subtype;
				}
				else if (subtype == "Staff") {
					//assemble prefix based on subtype
					if (level == 1) {s = "Foam ";}
					else if (level == 2) {s = "Rubber ";}
					else if (level == 3) {s = "Wooden ";}
					else if (level == 4) {s = "Metal ";}
					else if (level == 5) {s = "Mage's ";}
					else {cout << "Not a valid level (Staff -> name)" << endl;}
					s += subtype;
				}
				else {cout << "Not a valid subtype (Weapon -> name)" << endl;}
			
				//since there is no need to specify these as weapons....
				//this is the name of the item
				name = s;
			}
			//for unequipped slots
			else {name = "empty";}
		}
		else if (type == "Shield") {
			if (level > 0) { //validates as a "real" item
				//assemble prefix
				if (level == 1) {s = "Plastic ";}
				else if (level == 2) {s = "Wooden ";}
				else if (level == 3) {s = "Iron ";}
				else if (level == 4) {s = "Steel ";}
				else if (level == 5) {s = "Jade ";}
				else {cout << "Not a valid level (Shield -> name)" << endl;}
			
				//add type
				s += type;
			
				//this is the name
				name = s;
			}
			//for unequipped slots
			else {name = "empty";}
		}
		else if (type == "Offhand") {
			if (level > 0) { //validates as a "real" item
				//assemble prefix
				if (level == 1) {s = "Foam ";}
				else if (level == 2) {s = "Rubber ";}
				else if (level == 3) {s = "Wooden ";}
				else if (level == 4) {s = "Metal ";}
				else if (level == 5) {s = "Mage's ";}
				else {cout << "Not a valid level (Offhand -> name)" << endl;}
				s += subtype;
				
				//since there is no need to specify these as offhands....
				//this is the name
				name = s;
			}
			else {name = "empty";}
		}
		else if (type == "Potion") {
			//assemble prefix
			if (level == 1) {s = "Crappy ";}
			else if (level == 2) {s = "Cheap ";}
			else if (level == 3) {s = "Basic ";}
			else if (level == 4) {s = "Expensive ";}
			else if (level == 5) {s = "Luxurious ";}
			else {cout << "Not a valid level (Potion -> name)" << endl;}
			
			//add subtype to prefix
			if (subtype == "Health") {s += subtype + " ";}
			else if (subtype == "Mana") {s += subtype + " ";}
			else {cout << "Not a valid subtype (Potion -> name)" << endl;}
			
			//add type to the end
			s += type;
			
			//this is the name
			name = s;
		}
		else if (type == "Room") {
			//assemble prefix
			if (level == 1) {s = "Crappy ";}
			else if (level == 2) {s = "Cheap ";}
			else if (level == 3) {s = "Basic ";}
			else if (level == 4) {s = "Expensive ";}
			else if (level == 5) {s = "Luxurious ";}
			else {cout << "Not a valid level (Room -> name)" << endl;}
			
			//add type
			s += type;
			
			//this is the name
			name = s;
		}
		else if (type == "Trash") {
			//assemble prefix
			if (level == 1) {s = "Slimy ";}
			else if (level == 2) {s = "Pungent ";}
			else if (level == 3) {s = "Greasy ";}
			else if (level == 4) {s = "Rotten ";}
			else if (level == 5) {s = "Putrid ";}
			else {cout << "Not a valid level (Trash -> name)" << endl;}
			
			//add subtype
			s += subtype;
			
			//this is the name
			name = s;
		}
		else if (type == "Boss Drop") {name = subtype;}
		else {cout << "Not a valid item type (name)" << endl;}
		
		//buy value assembly
		if (type == "Armor") {
			//further classify by subtype
			if (subtype == "Mail") {buy_value = 45 * level;}
			else if (subtype == "Leather") {buy_value = 30 * level;}
			else if (subtype == "Cloth") {buy_value = 15 * level;}
			else {cout << "Not a valid subtype (Armor -> buy value)" << endl;}
		}
		else if (type == "Weapon") {
			//further classify by subtype
			if (subtype == "Sword") {buy_value = 100 * level;}
			else if (subtype == "Dagger") {buy_value = 50 * level;}
			else if (subtype == "Staff") {buy_value = 75 * level;}
			else {cout << "Not a valid subtype (Weapon -> buy value)" << endl;}
		}
		else if (type == "Shield") {
			//no subtypes to filter by
			buy_value = 30 * level;
		}
		else if (type == "Offhand") {
			//no subtypes to filter by
			buy_value = 30 * level;
		}
		else if (type == "Potion") {
			//no need to classify by subtype
			//health and mana potions should cost the same
			buy_value = 5 * level;
		}
		else if (type == "Room") {
			//no subtypes to filter by
			buy_value = 10 * level;
		}
		else if (type == "Trash") {
			if (subtype == "broken tooth") {buy_value = 6 * level;}
			else if (subtype == "hairball") {buy_value = 4 * level;}
			else if (subtype == "string") {buy_value = 8 * level;}
			else if (subtype == "tail fluff") {buy_value = 16 * level;}
			else if (subtype == "lint") {buy_value = 10 * level;}
			else {cout << "Not a valid subtype (Trash -> buy value)" << endl;}
		}
		else if (type == "Boss Drop") {buy_value = 200 * level;}
		else {cout << "Not a valid item type (buy value)" << endl;}
		
		//stat value calculation
		if (type == "Armor") {
			if (subtype == "Mail") {stat_bonus = level_multiplier(8);}
			else if (subtype == "Leather") {stat_bonus = level_multiplier(5);}
			else if (subtype == "Cloth") {stat_bonus = level_multiplier(3);}
		}
		else if (type == "Weapon") {
			if (subtype == "Sword") {stat_bonus = level_multiplier(5);}
			else if (subtype == "Dagger") {stat_bonus = level_multiplier(3);}
			else if (subtype == "Staff") {stat_bonus = level_multiplier(2);}
		}
		else if (type == "Shield") {stat_bonus = level_multiplier(15);}
		else if (type == "Offhand") {stat_bonus = level_multiplier(2);}
		else if (type == "Potion") {stat_bonus = level_multiplier(15);}
		else {stat_bonus = 0;}
		
		//attribute bonus calculation
		attrib_bonus = level_multiplier(1);
		
		//update viewable statistics
		if (type == "Armor") {
			def_bonus = stat_bonus;
			health_bonus = stat_bonus / 5;
			if (subtype == "Mail") {
				str_bonus = attrib_bonus;
				sta_bonus = attrib_bonus;
			}
			else if (subtype == "Leather") {
				agi_bonus = attrib_bonus;
			}
			else if (subtype == "Cloth") {
				int_bonus = attrib_bonus;
			}
		}
		else if (type == "Shield") {
			def_bonus = stat_bonus;
			block_bonus = 5 * level;
			str_bonus = attrib_bonus;
			sta_bonus = attrib_bonus;
		}
		else if (type == "Weapon") {
			atk_bonus = stat_bonus;
			if (subtype == "Sword") {
				str_bonus = attrib_bonus;
				sta_bonus = attrib_bonus;
			}
			else if (subtype == "Dagger") {
				agi_bonus = attrib_bonus;
			}
			else if (subtype == "Staff") {
				int_bonus = attrib_bonus;
				mana_bonus = stat_bonus;
			}
		}
		else if (type == "Offhand") {
			int_bonus = attrib_bonus;
			mana_bonus = stat_bonus;
		}
		
		//sets descriptions
		if (type == "Armor") {
			if (level > 0) {
				if (subtype == "Mail") {
					if (level == 1) {
						description = "Rusted almost beyond recognizing, this armor MAY provide some protection. ";
						description += "It may be too brittle to slow much of anything down, but at least it's better than plainsclothes.";
					}
					else if (level == 2) {
						description = "The mail is tarnished and loose. At least it isn't rusted through.";
					}
					else if (level == 3) {
						description = "Your average mail armor. It appears to be kept in decent condition, and will withstand a few good hits.";
					}
					else if (level == 4) {
						description = "This mail armor looks pristine. Its mail loops are even, tight, and hard. ";
						description += "It's hard to imagine anything managing to penetrate it.";
					}
					else if (level == 5) {
						description = "Ornamented and dusted with what appears to be precious gem fragments, this armor looks capable of withstanding ";
						description += "anything. Plus, it would look quite heroic on you.";
					}
				}
				else if (subtype == "Leather") {
					if (level == 1) {
						description = "Worn and stiff, the leather looks like an old man's skin after he's been dead for awhile. ";
						description += "You're VERY excited to wear this.";
					}
					else if (level == 2) {
						description = "This leather has seen better days. It looks faded and worn, but still intact. ";
						description += "At least it doesn't crumble at all when you touch it.";
					}
					else if (level == 3) {
						description = "Your average leather armor. It appears to be kept in decent condition, well-oiled and limber.";
					}
					else if (level == 4) {
						description = "Treated and cured, this armor looks professionally crafted. Wearing it, you feel both armored and dextrous. ";
						description += "This should serve you well in the field.";
					}
					else if (level == 5) {
						description = "Dyed matte black, this exceptional armor turns you into a shadow, barely distinguishable from the night. ";
						description += "You have never before seen nor felt its equal.";
					}
				}
				else if (subtype == "Cloth") {
					if (level == 1) {
						description = "Threadbare and bleached, this robe looks like something your grandmother wore 10 years ago.";
					}
					else if (level == 2) {
						description = "Very simple garb; it looks more like a pillowcase than a robe. At least it isn't see-through.";
					}
					else if (level == 3) {
						description = "This robe resembles the garb of your average Mage. ";
						description += "It allows for more intricate and complex movement, and the cloth seems of decent quality.";
					}
					else if (level == 4) {
						description = "Tailored to your frame, this both allows for freedom of movement and looks somewhat mesmerizing to witness in movement. ";
						description += "You feel some of the magic imbued within the cloth.";
					}
					else if (level == 5) {
						description = "Tailored and exquisitely woven, this robe is designed to capture and hold a foe's attention. ";
						description += "The magic imbued within the cloth feels potent, as if by itself, it could reflect an incoming attack or freeze an opponent.";
					}
				}
			}
			else {cout << "Not a valid level (init_item -> descriptions)" << endl;}
		}
		else if (type == "Shield") {
			if (level == 1) {
				description = "This shield looks like a toy or prop. It doesn't look capable of blocking anything.";
			}
			else if (level == 2) {
				description = "This shield is very heavy, and appears sturdy enough. It will probably block a few attacks before splitting.";
			}
			else if (level == 3) {
				description = "Made out of iron, this shield is dense and rigid. Neither ornamental or sloppy in its crafting, it should serve you well.";
			}
			else if (level == 4) {
				description = "Made of steel, this shield is half the thickness of its iron counterpart without sacrificing any defensive advantage. ";
				description += "This drastically reduces the weight of the shield, making it easier to block incoming attacks.";
			}
			else if (level == 5) {
				description = "Made of jade, this shield is both lightweight and rock-hard. ";
				description += "Patterned with turtle etchings, this is both aesthetically and functionally excellent.";
			}
			else {cout << "Not a valid level (init_item -> descriptions)" << endl;}
		}
		else if (type == "Weapon") {
			if (level > 0) {
				if (subtype == "Sword") {
					if (level == 1) {
						description = "Made of dull copper, this blunt sword looks like it would take several strikes before it could successfully cut through butter.";
					}
					else if (level == 2) {
						description = "Made of bronze, the look of the sword outshines its effectiveness in battle. However, it is, at least, sharp.";
					}
					else if (level == 3) {
						description = "Made of heavy iron, this sword is difficult to swing and manipulate, but it is effective. It will serve you well enough.";
					}
					else if (level == 4) {
						description = "Made of finely-wrought steel, this sword is slimmer and lighter than iron, allowing the wielder to strike more quickly and accurately.";
					}
					else if (level == 5) {
						description = "Made of pure diamond, this sword is made of the sharpest material known. ";
						description += "You have no idea how it could possibly have been crafted, but there's no way anything can withstand its sharp point.";
					}
				}
				else if (subtype == "Dagger") {
					if (level == 1) {
						description = "Made of dull copper, this blunt dagger looks more like a butter knife. Stabbing an opponent with this seems laughable.";
					}
					else if (level == 2) {
						description = "Made of bronze, this dagger looks more ornamental than useful. However, you're able to pierce a nearby orange with it, so that's something.";
					}
					else if (level == 3) {
						description = "Made of heavy iron, this dagger feels more like a sword. A bit unwieldy, but serviceable.";
					}
					else if (level == 4) {
						description = "Made of finely-wrought steel, this dagger is slim and ridged. It looks designed to deal damage upon both entry and exit.";
					}
					else if (level == 5) {
						description = "Made of pure diamond, this crystalline dagger looks like death perfected. ";
						description += "Lighter almost than air and deadlier than steel, you feel excited at the prospect of testing its limits.";
					}
				}
				else if (subtype == "Staff") {
					if (level == 1) {
						description = "This...item...looks like a pool noodle. With any luck, you can embarrass opponents with manly displays of your staff endowment.";
					}
					else if (level == 2) {
						description = "The term 'wiffle bat' comes to mind. The only thing that's going to channel through this is impotent rage.";
					}
					else if (level == 3) {
						description = "A basic wooden staff. Can channel rudimentary magics, and doubles as a walking stick!";
					}
					else if (level == 4) {
						description = "Made of dense metal, this staff conducts heat well, powering up your spells. ";
						description += "The weight also lends additional force to your movements, further increasing spell potency.";
					}
					else if (level == 5) {
						description = "Made of unknown material, it shivers with arcane power. Heavy to anyone without skill in the Arts, ";
						description += "it is still capable of striking and moving with great purpose. None. Shall. Pass.";
					}
				}
			}
			else {cout << "Not a valid level (init_item -> descriptions)" << endl;}
		}
		else if (type == "Offhand") {
			if (level == 1) {
				description = "This looks like something your dog would chew on. ";
				description += "The shape is correct, but you don't think this will help boost your magic much.";
			}
			else if (level == 2) {
				description = "Well, it's made of better stuff than foam, but it still looks like a destressing tool. ";
			}
			else if (level == 3) {
				description = "A wooden orb is not something you see every day. ";
				description += "It looks unusual enough to provide some boost to your magics.";
			}
			else if (level == 4) {
				description = "Made of dense, conductive metal, this is designed to help you ";
				description += "boost your arcane levels above average levels.";
			}
			else if (level == 5) {
				description = "A true Mage's orb, the arcane potential is condensed into a perfect sphere. ";
				description += "You feel power coursing through the moment your skin touches the cool surface.";
			}
			else {cout << "Not a valid level (init_item -> descriptions)" << endl;}
		}
		else if (type == "Potion") {
			if (level > 0) {
				if (subtype == "Health") {
					if (level == 1) {
						description = "This looks like bug juice. It might taste okay...";
					}
					else if (level == 2) {
						description = "Should've had a V8? Why, when you can have this for a steal!";
					}
					else if (level == 3) {
						description = "Ruby-red liquid fills the vial to the brim. It looks invigorating.";
					}
					else if (level == 4) {
						description = "As you study the vial, you feel slightly stronger. ";
						description += "Your lungs feel fuller; you feel lighter.";
					}
					else if (level == 5) {
						description = "A mysterious scarlet light shines from the vial. Mere proximity to the potion ";
						description += "makes your heart race, and you feel adrenaline flood your system.";
					}
				}
				else if (subtype == "Mana") {
					if (level == 1) {
						description = "This looks and smells like toilet cleaner. You're supposed to drink this??";
					}
					else if (level == 2) {
						description = "The vial seems opaque; you can't tell what color the liquid inside is. ";
						description += "This doesn't inspire much confidence...";
					}
					else if (level == 3) {
						description = "A clear vial filled with blue liquid. It looks refreshing.";
					}
					else if (level == 4) {
						description = "Though the sapphire liquid is dark, a faint glow seems to come from the vial. ";
						description += "As you stare at the glow, you feel calmer.";
					}
					else if (level == 5) {
						description = "A mysterious indigo glow infuses the liquid within the vial. ";
						description += "Simply looking at the potion gives you a sense of clarity and energy.";
					}
				}
			}
			else {cout << "Not a valid level (init_item -> descriptions)" << endl;}
		}
		else if (type == "Room") {
			if (level == 1) {
				description = "There are pig sties cleaner than this room.";
			}
			else if (level == 2) {
				description = "Drabby wallpaper and faded wood floors adorn this otherwise ordinary room.";
			}
			else if (level == 3) {
				description = "A perfectly ordinary, plain room. It could use some color.";
			}
			else if (level == 4) {
				description = "Everything in this room is pristine. ";
				description += "It's spacious, and a nice smell is coming from somewhere.";
			}
			else if (level == 5) {
				description = "An opulent room, with no holds barred in fashion and trappings. ";
				description += "You feel more relaxed just entering this Cadillac of suites.";
			}
			else {cout << "Not a valid level (init_item -> descriptions)" << endl;}
		}
		else if (type == "Trash") {
			if (level > 0) {
				if (subtype == "broken tooth") {
					if (level == 1) {
						description = "This tooth is slimy. Why is it slimy??";
					}
					else if (level == 2) {
						description = "This tooth stinks. Who knew a tooth could stink?";
					}
					else if (level == 3) {
						description = "Did someone try to deep-fry a tooth?";
					}
					else if (level == 4) {
						description = "This tooth is rotted nearly through.";
					}
					else if (level == 5) {
						description = "The stench coming from such a small tooth is staggering.";
					}
				}
				else if (subtype == "hairball") {
					if (level == 1) {
						description = "Fresh from a cat. But where are the cats?";
					}
					else if (level == 2) {
						description = "A little stomach acid is accompanying this special find.";
					}
					else if (level == 3) {
						description = "Somewhere along the line, fast food must have been involved.";
					}
					else if (level == 4) {
						description = "It takes a certain amount of food infused into a hairball to make it rot.";
					}
					else if (level == 5) {
						description = "The stench from this hairball is nothing short of impressive.";
					}
				}
				else if (subtype == "string") {
					if (level == 1) {
						description = "It looks like this string has been in a shallow pond for some time.";
					}
					else if (level == 2) {
						description = "This string has been wherever it's been a bit ";
						description += "too long. It smells like belly button.";
					}
					else if (level == 3) {
						description = "This string must have been mistaken for a noodle. Gross.";
					}
					else if (level == 4) {
						description = "This is so rotted, it's a miracle that it still keeps its shape.";
					}
					else if (level == 5) {
						description = "This smells like someone ate something greasy and then threw up on it.";
					}
				}
				else if (subtype == "tail fluff") {
					if (level == 1) {
						description = "This is more like tail sludge.";
					}
					else if (level == 2) {
						description = "This fluff must have come from a skunk at some point.";
					}
					else if (level == 3) {
						description = "This is what happens when oil spills meet bunnies. Sad.";
					}
					else if (level == 4) {
						description = "Rotten implies organic matter. Fluff implies animal rears.";
					}
					else if (level == 5) {
						description = "Like someone ate a bunny and then puked it up. That's what this is like.";
					}
				}
				else if (subtype == "lint") {
					if (level == 1) {
						description = "Animal saliva and human clothing make for a disgusting combination.";
					}
					else if (level == 2) {
						description = "For a moment, you wonder if this came from you. ";
						description += "But, no, it smells worse.";
					}
					else if (level == 3) {
						description = "This looks and feels like something you could have";
						description += "scratched off your scalp at some point.";
					}
					else if (level == 4) {
						description = "This might have smelled like bacon at some point. A long time ago.";
					}
					else if (level == 5) {
						description = "What little is left of an unlucky adventurer's shirt. ";
						description += "It smells like it was swallowed at some point in its travels.";
					}
				}
			}
			else {cout << "Not a valid level (init_item -> descriptions)" << endl;}
		}
		else if (type == "Boss Drop") {
			if (level == 1) {
				description = "Cracked, shattered, and full of cavities. ";
				description += "An ancient tooth from an ancient Pug.";
			}
			else if (level == 2) {
				description = "Compressed and hardened almost to the consistency of coal. ";
				description += "The furnace whence this nugget came was powerful indeed.";
			}
			else if (level == 3) {
				description = "Sharp and jagged, this claw is still capable of tearing cloth. ";
			}
			else if (level == 4) {
				description = "It's pink. It's a good thing dogs are color-blind.";
			}
			else if (level == 5) {
				description = "On closer inspection, this isn't actually an apple. ";
				description += "It is, in fact, made of solid gold, yet miraculously lightweight.";
			}
			else {cout << "Not a valid level (init_item -> descriptions)" << endl;}
		}
		
		//sell value is always half of the buy value...
		sell_value = buy_value / 2;
		
		//initializes as fully repaired
		damage_percentage = 0;
	}
	
	int level_multiplier(int x) {return x * 2 * level;}
};

class Spell {
public:
	Spell() {}
	
	void set_spell(string spell_name, string spell_type, int mag, int spell_cost, int player_level) {
		name = spell_name;
		type = spell_type;
		level = player_level;
		magnitude = mag;
		cost = spell_cost;
	}
	
	void inc_level() {
		level++;
		update_spell();
	}
	
	//GET FUNCTIONS
	int get_magnitude() {return magnitude;}
	int get_cost() {return cost;}
	string get_name() {return name;}
	string get_type() {return type;}
	
	//LOAD FUNCTIONS
	void load_magnitude(int x) {magnitude = x;}
	
private:
	//VARIABLES
	string name, type;
	int magnitude, cost, level;
	
	void update_spell() {
		magnitude /= (level - 1);
		magnitude *= level;
		cost /= (level - 1);
		cost *= level;
	}
};

//creature classes
class Creature {
public:
	Creature () {}
	
	//GET FUNCTIONS
	string get_name() {return name;}
	int get_exp() {return exp;}
	int get_atk() {return atk;}
	int get_def() {return def;}
	int get_health() {return health;}
	int get_accuracy() {return creature_acc;}
	int get_crit() {return crit;}
	int get_money() {return money;}
	int get_escape_chance() {return escape_chance;}
	int get_loot_count() {return loot_count;}
	Item *get_loot() {return loot;}
	bool get_has_loot() {return has_loot;}
	
	void det_creature(int player_level) {
		//creature determination can be any level up to current level
		level = rand() % player_level + 1;
	
		//determines type of creature encountered
		int x = rand() % 101;
		if (x <= 65) {type = "Balanced";} //bunny encounter - 65% chance
		else if (x > 65 && x <= 90) {type = "Offensive";} //ferret encounter - 25% chance
		else {type = "Defensive";} //midget encounter - 10% chance
		
		//initialize now that type is established
		stat_pool = 150 + 100 * (level - 1);
		set_type();
		has_loot = generate_loot();
	}
	
	//COMBAT
	void take_damage(int attack, int acc, int critical, string player_name) {
		if (rand() % 101 < dodge) {
			cout << name << " dodged your attack!" << endl;
			return;
		}
		if (rand() % 101 > acc) {
			cout << "Your attack missed!" << endl;
			return;
		}
		else {
			int x = DamageCalculation((double)attack, (double)def, (double)max_health, (double)critical, player_name);
			if (x > 0) {
				cout << name << " takes " << x << " damage!" << endl;
				health -= x;
			}
			else {cout << name << " took no damage." << endl;}
		}
		
	}
	void take_spell_dmg(int magnitude, int acc, int critical) {
		if (rand() % 101 < dodge) {
			cout << name << " dodged your attack!" << endl;
			return;
		}
		if (rand() % 101 > acc) {
			cout << "Your attack missed!" << endl;
			return;
		}
		else {
			//establish base damage
			int dmg = magnitude;
			//check for critical hits
			if (rand() % 101 < critical) {
				cout << "Your spell landed a critical hit!" << endl;
				dmg *= 2;
			}
			int skew = (int)(dmg * (1 - DMG_VARIANCE / 100.0));
			if (skew != dmg) {dmg = rand() % (dmg - skew) + skew;}
			else {dmg = skew;}
			cout << name << " takes " << dmg << " damage!" << endl;
			health -= dmg;
		}
	}
	bool is_dead() {
		if (health <= 0) {return true;}
		else {return false;}
	}
	
protected:
	//VARIABLES
	Item* loot;
	string name, type;
	int exp, atk, def, health, max_health, stat_pool, level;
	int creature_acc, dodge, parry, crit, money, escape_chance, loot_count;
	static const int DMG_VARIANCE = 15; //read as % variance
	bool has_loot;

	virtual bool generate_loot() {
		const int TRASH = 5;
		const int POTION = 2;
		const int EQUIPMENT = 8;
		int trash_chance = 35;
		int potion_chance = 15;
		int equipment_chance = 10;
		loot_count = 0;
		
		string trash[TRASH] = {"broken tooth", "hairball", "string", "tail fluff", "lint"};
		string potion[POTION] = {"Health", "Mana"};
		string equipment_type[EQUIPMENT] = {"Armor", "Armor", "Armor", "Weapon", "Weapon", "Weapon", "Offhand", "Shield"};
		string equipment_subtype[EQUIPMENT] = {"Mail", "Leather", "Cloth", "Sword", "Dagger", "Staff", "Orb", "Shield"};
		
		Item *tmp;
		//trash
		for (int i = 0; i < TRASH; i++) {
			if (rand() % 101 < trash_chance) {
				if (loot_count > 0) {
					tmp = new Item[loot_count];
					for (int j = 0; j < loot_count; j++) {tmp[j] = loot[j];}
				}
				loot_count++;
				loot = new Item[loot_count];
				if (loot_count + 1 > 0) {
					for (int k = 0; k < loot_count - 1; k++) {loot[k] = tmp[k];}
				}
				loot[loot_count - 1].set_item("Trash", trash[i], level);
			}
		}
		//potions
		for (int i = 0; i < POTION; i++) {
			if (rand() % 101 < potion_chance) {
				if (loot_count > 0) {
					tmp = new Item[loot_count];
					for (int j = 0; j < loot_count; j++) {tmp[j] = loot[j];}
				}
				loot_count++;
				loot = new Item[loot_count];
				if (loot_count + 1 > 0) {
					for (int k = 0; k < loot_count - 1; k++) {loot[k] = tmp[k];}
				}
				loot[loot_count - 1].set_item("Potion", potion[i], level);
			}
		}
		//equipment
		for (int i = 0; i < EQUIPMENT; i++) {
			if (rand() % 101 < equipment_chance) {
				if (loot_count > 0) {
					tmp = new Item[loot_count];
					for (int j = 0; j < loot_count; j++) {tmp[j] = loot[j];}
				}
				loot_count++;
				loot = new Item[loot_count];
				if (loot_count + 1 > 0) {
					for (int k = 0; k < loot_count - 1; k++) {loot[k] = tmp[k];}
				}
				loot[loot_count - 1].set_item(equipment_type[i], equipment_subtype[i], level);
			}
		}
		
		if (loot_count > 0) {return true;}
		else {return false;}
	}
	
	//HELPER FUNCTIONS TO ESTABLISH STANCE
	virtual void defensive() {
		creature_acc = 85;
		atk = stat_pool / 3;
		def = stat_pool * 2 / 3;
		max_health = stat_pool;
		dodge = 10;
		parry = 10;
		crit = 2;
		escape_chance = 85;
	}
	virtual void offensive() {
		creature_acc = 95;
		atk = stat_pool * 2 / 3;
		def = stat_pool / 3;
		max_health = stat_pool / 4;
		dodge = 2;
		parry = 2;
		crit = 10;
		escape_chance = 65;
	}
	virtual void balanced() {
		creature_acc = 90;
		atk = stat_pool / 2;
		def = stat_pool / 2;
		max_health = stat_pool / 2;
		dodge = 5;
		parry = 5;
		crit = 5;
		escape_chance = 75;
	}
	
	virtual void set_type() {
		string s;
		if (level >= 1) {s += "Cute ";}
		if (level >= 2) {s += "Fluffy ";}
		if (level >= 3) {s += "Crabby ";}
		if (level >= 4) {s += "Chubby ";}
		if (level == 5) {s += "DANGEROUS ";}
		
		atk = def = 0;
		if (type == "Offensive") {
			s += "ferret";
			name = s;
			exp = 10 * level;
			offensive();
		}
		else if (type == "Defensive") {
			s += "midget";
			name = s;
			exp = 15 * level;
			defensive();
		}
		else if (type == "Balanced") {
			s += "bunny";
			name = s;
			exp = 5 * level;
			balanced();
		}
		else {cout << "Not a valid type (Creature -> set_type)" << endl;}
		money = exp * 3;
		health = max_health;
	}
};
class Boss: public Creature {
public:
	Boss() {}
	Boss(int boss_level) {
		level = boss_level;
		creature_acc = 100;
		stat_pool = 150 + 100 * (level - 1);
		set_type();
		exp = 30 * level;
		money = exp * 5;
		has_loot = generate_loot();
	}
	
	//GET FUNCTIONS
	string get_flavor() {return attack_flavor;}

private:
	//VARIABLES
	string attack_flavor;
	
	bool generate_loot() {
		const int BOSS = 5;
		const int TRASH = 5;
		const int POTION = 2;
		const int EQUIPMENT = 8;
		int trash_chance = 50;
		int potion_chance = 20;
		int equipment_chance = 30;
		
		string boss[BOSS] = {"Fractured Tooth", "Poop Nugget", "Torn Claw", "Flea Collar", "Golden Apple"};
		string trash[TRASH] = {"broken tooth", "hairball", "string", "tail fluff", "lint"};
		string potion[POTION] = {"Health", "Mana"};
		string equipment_type[EQUIPMENT] = {"Armor", "Armor", "Armor", "Weapon", "Weapon", "Weapon", "Offhand", "Shield"};
		string equipment_subtype[EQUIPMENT] = {"Mail", "Leather", "Cloth", "Sword", "Dagger", "Staff", "Orb", "Shield"};
		
		//boss
		loot_count = 1;
		loot = new Item[loot_count];
		loot[loot_count - 1].set_item("Boss Drop", boss[level - 1], level);
		Item *tmp;
		//trash
		for (int i = 0; i < TRASH; i++) {
			if (rand() % 101 < trash_chance) {
				tmp = new Item[loot_count];
				for (int j = 0; j < loot_count; j++) {tmp[j] = loot[j];}
				
				loot_count++;
				loot = new Item[loot_count];
				
				for (int k = 0; k < loot_count - 1; k++) {loot[k] = tmp[k];}
				
				loot[loot_count - 1].set_item("Trash", trash[i], level);
			}
		}
		//potions
		for (int i = 0; i < POTION; i++) {
			if (rand() % 101 < potion_chance) {
				tmp = new Item[loot_count];
				for (int j = 0; j < loot_count; j++) {tmp[j] = loot[j];}
				
				loot_count++;
				loot = new Item[loot_count];
				
				for (int k = 0; k < loot_count - 1; k++) {loot[k] = tmp[k];}
				
				loot[loot_count - 1].set_item("Potion", potion[i], level);
			}
		}
		//equipment
		for (int i = 0; i < EQUIPMENT; i++) {
			if (rand() % 101 < equipment_chance) {
				tmp = new Item[loot_count];
				for (int j = 0; j < loot_count; j++) {tmp[j] = loot[j];}
				
				loot_count++;
				loot = new Item[loot_count];
				
				for (int k = 0; k < loot_count - 1; k++) {loot[k] = tmp[k];}
				
				loot[loot_count - 1].set_item(equipment_type[i], equipment_subtype[i], level);
			}
		}
		
		delete[] tmp;
		tmp = 0;
		
		return true;
	}
	
	//HELPER FUNCTIONS TO ESTABLISH STANCE
	void defensive() {
		atk = stat_pool / 3;
		def = stat_pool * 2 / 3;
		max_health = stat_pool;
		dodge = 25;
		parry = 25;
		crit = 5;
		escape_chance = 40;
	}
	void offensive() {
		atk = stat_pool * 2 / 3;
		def = stat_pool / 3;
		max_health = stat_pool / 4;
		dodge = 5;
		parry = 5;
		crit = 25;
		escape_chance = 20;
	}
	void balanced() {
		atk = stat_pool / 2;
		def = stat_pool / 2;
		max_health = stat_pool / 2;
		dodge = 10;
		parry = 10;
		crit = 10;
		escape_chance = 30;
	}
	
	void set_type() {
		if (level == 1) {
			name = "Omar, the Ancient";
			attack_flavor = "Omar breathes on you. His putrid stench overwhelms you!";
			balanced();
		}
		else if (level == 2) {
			name = "Clarissa, the Gaseous";
			attack_flavor = "Clarissa flatulates. Your eyes water from her gaseous assault!";
			defensive();
		}
		else if (level == 3) {
			name = "Simba, the Feline";
			attack_flavor = "Simba claws you viciously, drawing blood. He meows affectionately at you!";
			offensive();
		}
		else if (level == 4) {
			name = "Ninja, the Labrador";
			attack_flavor = "Ninja tackles you and begins licking you with his slobbery tongue. Get off!";
			balanced();
		}
		else if (level == 5) {
			name = "Bob, your Uncle";
			attack_flavor = "Uncle Bob starts telling a story about bobbing for apples. The horror!";
			balanced();
		}
		else {cout << "Not a valid level (Boss -> set_type())" << endl;}
		max_health *= 3;
		health = max_health;
	}
};

class Quest {
public:
	Quest() {complete = false;}
	Quest(int quest_level) {
		level = quest_level;
		exp = 50 * level;
		gold = 100 * level;
		Boss boss(level);
		set_quest(boss.get_name());
	}
	
	//GET FUNCTIONS
	bool get_complete() {return complete;}
	int get_exp() {return exp;}
	int get_gold() {return gold;}
	int get_level() {return level;}
	string get_name() {return name;}
	string get_description() {return description;}
	
	void complete_quest() {complete = true;}
	
private:
	string description, name;
	int level, exp, gold;
	bool complete;
	
	void set_quest(string boss_name) {
		name = "WANTED: " + boss_name;
		if (level == 1) {
			description = "As you start to leave town, a woman comes running and screaming towards you from just outside the entrance to town. ";
			description += "As she nears, you notice that she is disheveled and manic, her clothes torn. You step into her path and stop her. ";
			description += "She stops, gasping, and tells you of the villanous " + boss_name + " that attempted to destroy her virtue. You promise ";
			description += "to bring swift and brutal justice to " + boss_name + " and return to the fair maiden. She swoons in your arms..... Yeah, ";
			description += "it's probably best to start bringing justice.";
		}
		else if (level >= 2) {
			description = "You've licked your wounds and recovered. As you start to leave town again, the woman ";
			description += "from your previous adventure comes running towards you again. She tells you of the villanous ";
			description += boss_name + " that is now terrorizing damsels everywhere. You promise once more to bring swift and brutal ";
			description += "justice to " + boss_name + " and return to her. She hugs you, elated... It's probably best to get out there.";
		}
		complete = false;
	}
};

//player classes
class Player {
public:
	Player() {}
	
	//INITIALIZATION
	void set_name() {
		cout << "What is your name? ";
		char c[30];
		cin.getline(c, 30);
		name = c;
		cout << "Hi, " << name << "!" << endl;
		Pause();
		Clear();
	}
	void set_class() {
		//introduction to classes
		cout << "In RPG Land, there are three classes you can play. These three "
		<< "classes are commonly known as Warriors, Rogues, and Mages." << endl;
		Pause();
		set_class_start:
		Clear();
		//print out the options to choose from
		cout << "Please choose a class to learn more about:\n" << endl;
		int count = 1;
		cout << count << ". Warrior" << endl;
		count++;
		cout << count << ". Rogue" << endl;
		count++;
		cout << count << ". Mage" << endl;
		
		//switch "choice", confirmation "decision"
		int choice;
		char decision;
		while (true) {
			choice = GetInt();
			switch (choice) {
				//warrior choice
				case 1:
					warrior_description();
					cout << "Would you like to choose the Warrior (Y/N)? ";
					while (true) {
						decision = GetChar();
						if (decision == 'Y' || decision == 'y') {
							player_class = "Warrior";
							goto set_class_confirm;
						}
						else if (decision == 'N' || decision == 'n') {goto set_class_start;}
						else {cout << "Please enter either Y or N: ";}
					}
					break;
				//rogue choice
				case 2:
					rogue_description();
					cout << "Would you like to choose the Rogue (Y/N)? ";
					while (true) {
						decision = GetChar();
						if (decision == 'Y' || decision == 'y') {
							player_class = "Rogue";
							goto set_class_confirm;
						}
						else if (decision == 'N' || decision == 'n') {goto set_class_start;}
						else {cout << "Please enter either Y or N: ";}
					}
					break;
				//mage choice
				case 3:
					mage_description();
					cout << "Would you like to choose the Mage (Y/N)? ";
					while (true) {
						decision = GetChar();
						if (decision == 'Y' || decision == 'y') {
							player_class = "Mage";
							goto set_class_confirm;
						}
						else if (decision == 'N' || decision == 'n') {goto set_class_start;}
						else {cout << "Please enter either Y or N: ";}
					}
					break;
				default:
					cout << "Please enter a number between 1 and " << count << ": ";
					break;
			}
		}
		set_class_confirm:
		cout << "\nYou've chosen the " << player_class << " class! Congratulations!\n" << endl;
		Pause();
	}
	void set_quests() {
		Quest quest;
		boss_quest = quest;
		load_quest_active(false);
	}
	
	//these are referenced within base class even though they vary based on class type
	//Hence, these are set as "virtual". Cannot be set as "pure virtual" since the base
	//class is instantiated.
	virtual void set_player() {
		set_name();
		set_class();
	}
	virtual void update_stats() {}
	virtual void init_spellbook() {}
	virtual void restore_mana(int x) {}
	virtual void restore_mana_to_max() {}
	virtual int get_current_mana() {return 0;}
	virtual void level_up() {
		cout << "You've leveled up! ";
		level++;
		exp_to_next_level = LEVEL_INCREMENT * level * level;
		cout << "You are now Level " << level << "!" << endl;
		points = 5;
		Pause();
		attribs_menu();
	}
	virtual void view_stats() {
		Clear();
		cout << "Your current stats are: " << endl;
		cout << "ATK: " << atk << endl;
		cout << "DEF: " << def << endl;
		cout << "Health: " << current_health << "/" << max_health << endl;
		cout << "\nExperience: " << exp << "/" << exp_to_next_level << endl;
	}
	virtual void view_mana() {}
	//COMBAT
	virtual int cast_spell(bool in_combat) {return 0;}
	virtual bool get_did_cast_spell() {return true;}
	virtual void lower_def() {}
	virtual void boost_block() {}
	virtual void lower_block() {}
	virtual void boost_atk() {}
	virtual void lower_atk() {}
	//EQUIPMENT MANIPULATION
	virtual void unequip_item(Item item) {}
	virtual void equip_item(Item item) {}
	
	void equipment_menu() {
		Item *inv;
		
		equipment_menu_start:
		view_equipment();
		
		cout << "\nWhat would you like to do?\n" << endl;
		
		int count = 1;
		cout << count << ". Equip an item" << endl;
		count++;
		cout << count << ". Unequip an item" << endl;
		count++;
		cout << count << ". View detailed information about an item" << endl;
		count++;
		cout << count << ". Leave menu" << endl;
		
		int equipable_items_count = num_of_equipable_items();
		bool no_equipment = true;
		for (int i = 0; i < EQUIPMENT_SLOTS; i++) {
			if (equipment[i].get_level() > 0) {no_equipment = false;}
		}
		
		int choice;
		while (true) {
			choice = GetInt();
			switch (choice) {
				case 1:
					if (equipable_items_count > 0) {
						int equip_choice;
						while (true) {
							view_equipment();
							cout << endl;
							inv = view_equipable_items();
							cout << equipable_items_count + 1 << ". Cancel request" << endl;
							cout << "\nWhich item would you like to equip?" << endl;
							equip_choice = GetInt();
							equip_choice--;
							if (equip_choice < equipable_items_count && equip_choice >= 0) {
								if (inv[equip_choice].get_level() <= level) {
									equip_item(inv[equip_choice]);
									goto equipment_menu_start;
								}
								else {
									cout << "You aren't at a sufficient level to equip this!" << endl;
									Pause();
									Clear();
								}
							}
							else if (equip_choice == equipable_items_count) {goto equipment_menu_start;}
							else {cout << "Please enter a number between 1 and " << equipable_items_count + 1 << ": ";}
						}
					}
					else {
						cout << "\nYou have no equippable items.\n" << endl;
						Pause();
						goto equipment_menu_start;
					}
					delete[] inv;
					inv = 0;
					break;
				case 2:
					if (no_equipment) {
						cout << "You have nothing equipped; you cannot unequip anything." << endl;
						Pause();
						goto equipment_menu_start;
					}
					view_equipment();
					cout << EQUIPMENT_SLOTS + 1 << ". Cancel request" << endl;
					cout << "\nWhich equipment would you like to unequip?" << endl;
					int unequip_choice;
					while (true) {
						unequip_choice = GetInt();
						unequip_choice--;
						if (unequip_choice < EQUIPMENT_SLOTS && unequip_choice >= 0) {
							unequip_item(equipment[unequip_choice]);
							goto equipment_menu_start;
						}
						else if (unequip_choice == EQUIPMENT_SLOTS) {
							goto equipment_menu_start;
						}
					}
					break;
				case 3:
					if (no_equipment) {
						cout << "You have nothing equipped; there is no detail to view." << endl;
						Pause();
						goto equipment_menu_start;
					}
					view_equipment();
					cout << EQUIPMENT_SLOTS + 1 << ". Cancel request" << endl;
					cout << "\nWhich item would you like to view detailed information about?" << endl;
					int info_choice;
					info_choice = GetInt();
					info_choice--;
					equipment[info_choice].view_stats(player_class);
					goto equipment_menu_start;
					break;
				case 4:
					return;
					break;
				default:
					cout << "Please enter a number between 1 and " << count << ": ";
					break;
			}
		}
	}
	
	//HEALTH MANIPULATION
	void restore_health_to_max() {current_health = max_health;}
	void restore_health(int x) {
		current_health += x;
		if (current_health > max_health) {restore_health_to_max();}
	}
	void lose_health(int x) {current_health -= x;}
	
	//INVENTORY MANIPULATION
	void inc_inventory(Item item) {
		bool in_inventory = false;
		int index;
		for (int i = 0; i < items_in_inventory; i++) {
			if (inventory[i].get_name() == item.get_name()) {
				in_inventory = true;
				index = i;
			}
		}
		if (in_inventory) {inventory[index].inc_quantity();}
		else {add_to_inventory(item);}
	}
	void dec_inventory(Item item) {
		for (int i = 0; i < items_in_inventory; i++) {
			if (inventory[i].get_name() == item.get_name()) {
				inventory[i].dec_quantity();
				if (inventory[i].get_quantity() == 0) {
					remove_from_inventory(inventory[i]);
				}
			}
		}
	}

	//COMBAT
	void take_damage(int attack, int accuracy, int critical, string creature_name) {
		if (rand() % 101 < block && equipment[2].broken() == false) {
			cout << "You block " << creature_name << "'s attack!" << endl;
			equipment[2].damage(1); //only applies to Warriors, so damages shield slot
			return;
		}
		if (rand() % 101 < dodge) {
			cout << "You dodge " << creature_name << "'s attack!" << endl;
			return;
		}
		if (rand() % 101 < parry) {
			cout << "You parry " << creature_name << "'s attack!" << endl;
			if (player_class == "Rogue") {
				if (equipment[1].broken() && equipment[2].broken()) {}
				else if (equipment[1].broken()) {equipment[2].damage(1);}
				else if (equipment[2].broken()) {equipment[1].damage(1);}
				else {
					int x = rand() % 2 + 1; //either 1 or 2
					equipment[x].damage(1); //randomly damage a weapon
				}
			}
			else {
				if (equipment[1].broken() == false) {equipment[1].damage(1);} //damage weapon slot
			}
			return;
		}
		if (rand() % 101 > accuracy) {
			cout << creature_name << " missed you!" << endl;
			return;
		}
		else {
			int x = DamageCalculation((double)attack, (double)def, (double)max_health, (double)critical, creature_name);
			if (x > 0) {
				cout << "You take " << x << " damage!" << endl;
				lose_health(x);
				equipment[0].damage(1); //always Armor slot
			}
			else {cout << "You didn't take any damage." << endl;}
		}
	}
	void dies() {
		cout << "You've died!" << endl;
		lose_exp(50);
		for (int i = 0; i < EQUIPMENT_SLOTS; i++) {equipment[i].damage(20);}
		Pause();
		if (exp < LEVEL_INCREMENT * (level - 1)) {exp = LEVEL_INCREMENT * (level - 1);}
		
		while (boosted > 0) {
			if (player_class == "Warrior") {lower_block();}
			else if (player_class == "Rogue") {lower_atk();}
			else if (player_class == "Mage") {lower_def();}
		}
	}

	void repair(int x) {equipment[x].repair(100);}
	
	//VIEW FUNCTIONS
	void view_attrib_description() {
		int choice;
		while (true) {
			Clear();
			cout << "What attribute would you like to learn more about?\n" << endl;
			int count = 1;
			cout << count << ". Strength" << endl;
			count++;
			cout << count << ". Agility" << endl;
			count++;
			cout << count << ". Intellect" << endl;
			count++;
			cout << count << ". Stamina" << endl;
			count++;
			cout << count << ". Return to the previous menu" << endl;
			
			view_attrib_description_choice:
			choice = GetInt();
			switch (choice) {
				case 1:
					cout << "STRENGTH:" << endl;
					cout << description_strength << endl;
					Pause();
					continue;
					break;
				case 2:
					cout << "AGILITY:" << endl;
					cout << description_agility << endl;
					Pause();
					continue;
					break;
				case 3:
					cout << "INTELLECT:" << endl;
					cout << description_intellect << endl;
					Pause();
					continue;
					break;
				case 4:
					cout << "STAMINA" << endl;
					cout << description_stamina << endl;
					Pause();
					continue;
					break;
				case 5:
					return;
					break;
				default:
					cout << "Please enter a number between 1 and " << count << ": ";
					goto view_attrib_description_choice;
					break;
			}
		}
	}
	void view_inventory() {
		view_inventory_start:
		Clear();
		if (items_in_inventory == 0) {
			cout << "You have no items in your inventory." << endl;
			Pause();
			return;
		}
		else {
			cout << "You have the following items in your inventory:" << endl;
			for (int i = 0; i < items_in_inventory; i++) {
				cout << i + 1 << ". " << inventory[i].get_name() << " (Requires Level " << inventory[i].get_level() << ") x" << inventory[i].get_quantity();
				if (inventory[i].equipable()) {cout << " - can be equipped" << endl;}
				else {cout << endl;}
			}
		}
		cout << endl;
		view_gold();
		
		cout << "\nWould you like to view detailed information about an item (Y/N)? ";
		char decision;
		while (true) {
			decision = GetChar();
			if (decision == 'Y' || decision == 'y') {
				cout << "\nWhich item would you like to view detailed information about?" << endl;
				int info_choice;
				while (true) {
					info_choice = GetInt();
					info_choice--;
					if (info_choice < items_in_inventory && info_choice >= 0) {
						inventory[info_choice].view_stats(player_class);
						goto view_inventory_start;
					}
					else {cout << "Please enter a number between 1 and " << items_in_inventory << ": ";}
				}
			}
			else if (decision == 'N' || decision == 'n') {return;}
			else {cout << "Please enter Y or N: ";}
		}
	}
	void view_equipment() {
		Clear();
		cout << "You have the following equipped:" << endl;
		for (int i = 0; i < EQUIPMENT_SLOTS; i++) {
			cout << i + 1 << ". " << equipment[i].get_type() << " slot: " << equipment[i].get_name();
			if (equipment[i].broken()) {cout << " - BROKEN";}
			else if (equipment[i].get_level() > 0) {cout << " - " << 100 - equipment[i].get_damage() << "/100";}
			cout << endl;
		}
	}
	Item* view_equipable_items() {
		int count = 0;
		Item *inv;
		for (int i = 0; i < items_in_inventory; i++) {
			if (inventory[i].equipable()) {count++;}
		}
		inv = new Item[count];
		int j = 0;
		for (int i = 0; i < items_in_inventory; i++) {
			if (inventory[i].equipable()) {
				inv[j] = inventory[i];
				j++;
			}
		}
		cout << "You have the following equipable items:" << endl;
		for (int i = 0; i < count; i++) {
			cout << i + 1 << ". " << inv[i].get_name()
			<< " x" << inv[i].get_quantity()<< endl;
		}
		return inv;
	}
	void view_gold() {cout << "You currently have " << gold << " gold." << endl;}
	void view_quest_log() {
		Clear();
		if (quest_active) {
			cout << boss_quest.get_name() << endl << endl;
			cout << "Description:" << endl;
			cout << boss_quest.get_description() << endl;
			cout << "\nReward for completing:" << endl;
			cout << boss_quest.get_exp() << " experience" << endl;
			cout << boss_quest.get_gold() << " gold" << endl;
			cout << "\nCompleted: ";
			if (boss_quest.get_complete() == false) {cout << "No" << endl;}
			else {cout << "Yes" << endl;}
		}
		else {cout << "You don't have an active quest." << endl;}
	}
	void view_health() {cout << "Your health: " << current_health << "/" << max_health << endl;}
	
	//GOLD-EXP MANIPULATION
	void remove_gold(int x) {gold -= x;}
	void add_gold(int x) {
		gold += x;
		cout << "You've collected " << x << " gold!" << endl;
	}
	void gain_exp(int exp_gain) {
		exp += exp_gain;
		cout << "You've gained " << exp_gain << " experience!" << endl;
		if (exp >= exp_to_next_level && level < MAX_LEVEL) {level_up();}
	}
	void lose_exp(int exp_loss) {
		exp -= exp_loss;
		cout << "You've lost " << exp_loss << " experience!" << endl;
	}
	void earn_explore_bonus(int area) {
		cout << "You've explored the entire area!" << endl;
		gain_exp(area);
		add_gold(area * 2);
		Pause();
	}
	
	//QUESTING
	void quest_calculation_and_acceptance() {
		Pause();
		for (int i = 0; i < level; i++) {
			if (boss_quests_complete[i] == false && quest_active == false) {//haven't completed it yet
				Quest proposed_quest(i + 1);
				cout << proposed_quest.get_description() << endl;
				cout << "\nQuest accepted!" << endl;
				Pause();
				boss_quest = proposed_quest;
				load_quest_active(true);
				return; //prevents accepting additional boss quests, which would overwrite previous ones
			}
		} //if the end is reached with an accepted quest, it means they're all done up to current level
	}
	bool quest_complete() {
		if (boss_quest.get_complete()) {return true;}
		else {return false;}
	}
	void complete_active_quest() {boss_quest.complete_quest();}
	void turn_in_quest() {
		Boss boss(boss_quest.get_level());
		cout << "A woman rushes over to you, and it takes you a moment to realize that she is the same person "
		<< "who told you about " << boss.get_name() << ". She's cleaned up, and is looking much more attractive." << endl;
		cout << "Damsel: 'Did you get that foul " << boss.get_name() << "?' You nod, and her face splits into a beautiful, "
		<< "rather seductive grin." << endl;
		cout << "Damsel: 'Great, let me repay you for your kind deed...'" << endl;
		cout << "\nQuest complete! You've earned the following rewards:" << endl;
		cout << boss_quest.get_exp() << " experience" << endl;
		cout << boss_quest.get_gold() << " gold" << endl;
		gain_exp(boss_quest.get_exp());
		add_gold(boss_quest.get_gold());
		load_quest_active(false);
		boss_quests_complete[boss_quest.get_level() - 1] = true;
	}
	
	//attribute overview and manipulation
	void attribs_menu() {
		Clear();
		if (points > 0) {
			cout << "You have unspent points!\n" << endl;
			Pause();
		}
		attribs_menu_start:
		view_attributes();
		
		cout << "\nWhat would you like to do? " << endl;
		if (points > 0) {cout << "(A)llocate points to attributes" << endl;}
		else if (points == 0) {cout << "(R)eturn without doing anything" << endl;}
		else {cout << "points out of bounds (attribs_menu)" << endl;}
		cout << "(L)earn more about an attribute and how it affects your class" << endl;
		
		char choice;
		while (true) {
			choice = GetChar();
			if (points > 0) {
				switch (choice) {
					case 'L':
					case 'l':
						view_attrib_description();
						goto attribs_menu_start;
						break;
					case 'A':
					case 'a':
						allocate_points();
						goto attribs_menu_start;
						break;
					default:
						cout << "Please enter L or A: ";
						break;
				}
			}
			else if (points == 0) {
				switch (choice) {
					case 'L':
					case 'l':
						view_attrib_description();
						goto attribs_menu_start;
						break;
					case 'R':
					case 'r':
						return;
						break;
					default:
						cout << "Please enter L or R: ";
						break;
				}
			}
			else {cout << "points out of bounds (attribs_menu)" << endl;}
		}
	}
	
	//LEVELING
	void allocate_points() {
		allocate_points_start:
		while (points > 0) {
			//overview
			view_attributes();
		
			cout << "\nYou have " << points << " points remaining. ";
			cout << "Please select an attribute to add to:" << endl;
			
			//list of available attributes to add to
			int count = 1;
			cout << count << ". Strength" << endl;
			count++;
			cout << count << ". Agility" << endl;
			count++;
			cout << count << ". Intellect" << endl;
			count++;
			cout << count << ". Stamina" << endl;
			int choice;
			while (true) {
				choice = GetInt();
				switch (choice) {
					case 1:
						base_str++;
						update_stats();
						points--;
						goto allocate_points_start;
						break;
					case 2:
						base_agi++;
						update_stats();
						points--;
						goto allocate_points_start;
						break;
					case 3:
						base_int++;
						update_stats();
						points--;
						goto allocate_points_start;
						break;
					case 4:
						base_sta++;
						update_stats();
						points--;
						goto allocate_points_start;
						break;
					default:
						cout << "Please enter a number between 1 and " << count << ": ";
						break;
				}
			}
		}
		view_attributes();
		cout << "\nAll available points have been allocated." << endl;
		Pause();
	}

	//BOOLS
	bool use_item() {
		Clear();
		Item *potions;
		int count = 0;
		
		//establish number of potions in inventory
		for (int i = 0; i < items_in_inventory; i++) {
			if (inventory[i].get_type() == "Potion") {count++;}
		}
		
		//what if you have no items?
		if (count == 0) {
			cout << "You don't have any useable items!" << endl;
			Pause();
			return false;
		}
		
		//set temporary array with correct size and fill it
		potions = new Item[count];
		int index = 0;
		for (int i = 0; i < items_in_inventory; i++) {
			if (inventory[i].get_type() == "Potion") {
				potions[index] = inventory[i];
				index++;
			}
		}
		
		//choose a potion to use
		use_item_choice:
		Clear();
		cout << "What potion would you like to use:\n" << endl;
		for (int i = 0; i < count; i++) {
			cout << i + 1 << ". " << potions[i].get_name() << " x"
			<< potions[i].get_quantity() << endl;
		}
		//adds an option to cancel
		cout << count + 1 << ". Cancel request (go back to the previous screen)" << endl;
		
		int choice;
		while (true) {
			//decrement by one for index
			choice = GetInt();
			choice--;
			if (choice < count && choice >= 0) {
				//determine change to health / mana
				if (potions[choice].get_level() <= level) {
					if (potions[choice].get_subtype() == "Health") {
						int gain = 30 * potions[choice].get_level();
						restore_health(gain);
						cout << "You've used a " << potions[choice].get_name() << " and restored "
						<< gain << " health!" << endl;
					}
					else if (potions[choice].get_subtype() == "Mana") {
						if (player_class == "Mage") {
							int gain = 30 * potions[choice].get_level();
							restore_mana(gain);
							cout << "You've used a " << potions[choice].get_name() << " and restored "
							<< gain << " mana!" << endl;
						}
						else {
							cout << "You've used a " << potions[choice].get_name()
							<< ", but it had no effect..." << endl;
						}
					}
				}
				else {
					cout << "You aren't at a sufficient level to use this item!" << endl;
					Pause();
					goto use_item_choice;
				}
				Pause();
				//decrement item from inventory
				dec_inventory(potions[choice]);
				return true;
			}
			else if (choice == count) {return false;} //cancels out
			else {cout << "Please enter a number between 1 and " << count + 1 << ": ";}
		}
	}
	bool is_dead() {
		if (current_health <= 0) {return true;}
		else {return false;}
	}
	
	//GET FUNCTIONS
	int* get_base_attribs() {
		base_attribs[0] = base_str;
		base_attribs[1] = base_agi;
		base_attribs[2] = base_int;
		base_attribs[3] = base_sta;
		return base_attribs;
	}
	int* get_exp_gold_meters_inv_count() {
		exp_gold_meters_inv_count[0] = exp;
		exp_gold_meters_inv_count[1] = gold;
		exp_gold_meters_inv_count[2] = max_health;
		exp_gold_meters_inv_count[3] = max_mana;
		exp_gold_meters_inv_count[4] = items_in_inventory;
		return exp_gold_meters_inv_count;
	}
	int get_exp() {return exp;}
	int get_gold() {return gold;}
	int get_level() {return level;}
	int get_atk() {return atk;}
	int get_def() {return def;}
	int get_max_health() {return max_health;}
	int get_max_mana() {return max_mana;}
	int get_current_health() {return current_health;}
	int get_items_in_inventory() {return items_in_inventory;}
	int get_accuracy() {return accuracy;}
	int get_crit() {return crit;}
	int get_boosted() {return boosted;}
	int get_equipment_slots() {return EQUIPMENT_SLOTS;}
	string get_name() {return name;}
	string get_player_class() {return player_class;}
	Item* get_inventory() {return inventory;}
	Item* get_equipment() {return equipment;}
	bool* get_boss_quests_complete() {return boss_quests_complete;}
	bool get_quest_active() {return quest_active;}
	Quest get_boss_quest() {return boss_quest;}
	
	//LOAD FUNCTIONS
	void load_base_attribs(int* &x) {
		for (int i = 0; i < 4; i++) {base_attribs[i] = x[i];}
		base_str = base_attribs[0];
		base_agi = base_attribs[1];
		base_int = base_attribs[2];
		base_sta = base_attribs[3];
	}
	void load_exp_gold_meters_inv_count(int* &x) {
		for (int i = 0; i < 5; i++) {exp_gold_meters_inv_count[i] = x[i];}
		exp = exp_gold_meters_inv_count[0];
		gold = exp_gold_meters_inv_count[1];
		max_health = exp_gold_meters_inv_count[2];
		max_mana = exp_gold_meters_inv_count[3];
		items_in_inventory = exp_gold_meters_inv_count[4];
	}
	void load_name(char *c) {name = c;}
	void load_inventory(Item* &items) {
		inventory = new Item[items_in_inventory];
		for (int i = 0; i < items_in_inventory; i++) {
			inventory[i] = items[i];
		}
	}
	void load_equipment(Item* &items) {
		for (int i = 0; i < EQUIPMENT_SLOTS; i++) {
			equipment[i].set_item(items[i].get_type(), items[i].get_subtype(), items[i].get_level()); //init_item
		}
	}
	void load_level(int xp) {
		level = 1;
		exp_to_next_level = LEVEL_INCREMENT * level * level;
		for (; xp >= exp_to_next_level && level < MAX_LEVEL; level++) {
			exp_to_next_level = LEVEL_INCREMENT * level * level;
		}
	}
	void load_quest_active(bool active) {quest_active = active;}
	void load_boss_quest_completion(bool* &b) {
		for (int i = 0; i < MAX_LEVEL; i++) {boss_quests_complete[i] = b[i];}
	}
	
protected:
	//VARIABLES
	static const int EQUIPMENT_SLOTS = 3;
	Item equipment[EQUIPMENT_SLOTS];
	Item *inventory;
	Quest boss_quest;
	string name, player_class;
	string description_strength, description_agility, description_intellect, description_stamina;
	int base_attribs[4]; //used for load / save functionality
	int exp_gold_meters_inv_count[5]; //used for load / save functionality
	int base_str, base_agi, base_int, base_sta; //baseline attributes
	int str_bonus, agi_bonus, int_bonus, sta_bonus; //equipment attribute bonuses
	int strength, agility, intellect, stamina; //fully adjusted attributes
	int points, exp, items_in_inventory, gold, level;
	int atk, def, max_health, max_mana, current_health;
	int equip_atk_bonus, equip_def_bonus, equip_health_bonus;
	int crit, dodge, parry, block, accuracy, exp_to_next_level;
	int boosted;
	static const int ATTRIB_BONUS = 5;
	static const int CLASS_BONUS = 10;
	static const int LEVEL_INCREMENT = 50;
	static const int DMG_VARIANCE = 15; //read as % variance
	bool boss_quests_complete[MAX_LEVEL];
	bool quest_active;
	
	//CLASS DESCRIPTIONS
	void warrior_description() {
		cout << "Warriors are melee fighters highly trained in the art "
		<< "of weaponry. They are unmatched in one-on-one physical combat, "
		<< "have unusually high stamina, and can wear any type of armor. They "
		<< "are the only class that can use shields.\n" << endl;
		
		cout << "A warrior's primary attribute is Strength." << endl;
	}
	void rogue_description() {
		cout << "Rogues are highly trained in the art of subterfuge and stealth. "
		<< "They specialize in taking their enemy by surprise, and dealing "
		<< "copious amounts of damage by exploiting this advantage. Rogues cannot "
		<< "wear heavy armor or use shields.\n" << endl;
		
		cout << "A rogue's primary attribute is Agility." << endl;
	}
	void mage_description() {
		cout << "Mages are masters of the elements and arcane spells. They use "
		<< "both offensive and defensive spells to both decimate their foes and "
		<< "protect and heal themselves. To assist them in channelling these potent "
		<< "magics, mages elect to wear flowing robes of magically-imbued cloth and "
		<< "eschew more traditional forms of protection.\n" << endl;
		
		cout << "A mage's primary attribute is Intellect." << endl;
	}
	
	//establishes pre-customization baseline for attributes
	void init_attribs() {base_str = base_agi = base_int = base_sta = 10;}

	void view_attributes() {
		Clear();
		cout << "Your current attributes are: " << endl;
		cout << "Strength: " << strength << endl;
		cout << "Agility: " << agility << endl;
		cout << "Intellect: " << intellect << endl;
		cout << "Stamina: " << stamina << endl;
	}
	
	//INVENTORY MANIPULATION
	void add_to_inventory(Item item) {
		//create temp array with current number of items in inventory
		Item *tmp_array;
		tmp_array = new Item[items_in_inventory];
		for (int i = 0; i < items_in_inventory; i++) {
			tmp_array[i] = inventory[i];
		}
		
		//increment the items in inventory counter by one
		items_in_inventory++;
		
		//set inventory to the new size and copy current items back in
		inventory = new Item[items_in_inventory];
		for (int i = 0; i < items_in_inventory - 1; i++) {
			inventory[i] = tmp_array[i];
		}
		
		//purge temp array to prevent memory leaks
		delete[] tmp_array;
		tmp_array = 0;
		
		//add the new item to the last (new) index in inventory
		inventory[items_in_inventory - 1] = item;
	}
	void remove_from_inventory(Item item) {
		//create temp array with one less index
		Item *tmp_array;
		tmp_array = new Item[items_in_inventory - 1];
		int count = 0;
		
		//moves over the items to be kept
		for (int i = 0; i < items_in_inventory; i++) {
			if (inventory[i].get_name() != item.get_name()) {
				tmp_array[count] = inventory[i];
				count++;
			}
		}
		
		//decrement the items in inventory counter by one
		items_in_inventory--;
		
		//set inventory to the new size and copy updated items back in
		inventory = new Item[items_in_inventory];
		for (int i = 0; i < items_in_inventory; i++) {
			inventory[i] = tmp_array[i];
		}
		
		//purge temp array to prevent memory leaks
		delete[] tmp_array;
		tmp_array = 0;
	}
	int num_of_equipable_items() {
		int count = 0;
		for (int i = 0; i < items_in_inventory; i++) {
			if (inventory[i].equipable()) {count++;}
		}
		return count;
	}
};
class Warrior: public Player {
public:
	Warrior(string player_name) {
		name = player_name;
		player_class = "Warrior";
		set_descriptions();
		init_attribs();
		points = 0;
		base_str += CLASS_BONUS;
		base_sta += CLASS_BONUS;
		boosted = 0;
	}
	
	void set_player() {
		level = 1;
		exp_to_next_level = LEVEL_INCREMENT * level * level;
		exp = items_in_inventory = 0;
		gold = 300;
		points = 10;
		update_stats();
		attribs_menu();
		current_health = max_health;
		init_equipment();
		for (int i = 0; i < MAX_LEVEL; i++) {boss_quests_complete[i] = false;}
	}
	
	//COMBAT
	void boost_block() {
		block += 30;
		boosted = 1;
		cout << "You've boosted your block chance by 30%!" << endl;
	}
	void lower_block() {
		block -= 30;
		boosted = 0;
	}
	
	//establishes stats and attributes, and takes bonuses into consideration
	void update_stats() {
		//update equipment bonuses
		block = equipment[2].get_block_bonus(); //will correct to 0 if no equipment
		if (equipment[0].get_level() > 0 && equipment[2].get_level() > 0) {
			if (equipment[0].broken() && equipment[2].broken()) {equip_def_bonus = 0;}
			else if (equipment[0].broken()) {equip_def_bonus = equipment[2].stat_value();}
			else if (equipment[2].broken()) {equip_def_bonus = equipment[0].stat_value();}
			else {equip_def_bonus = equipment[0].stat_value() + equipment[2].stat_value();}
		}
		else if (equipment[0].get_level() > 0 && equipment[0].broken() == false) {
			equip_def_bonus = equipment[0].stat_value();
		}
		else if (equipment[2].get_level() > 0 && equipment[2].broken() == false) {
			equip_def_bonus	= equipment[2].stat_value();
		}
		else {equip_def_bonus = 0;}
		
		if (equipment[1].get_level() > 0 && equipment[1].broken() == false) {
			equip_atk_bonus = equipment[1].get_atk_bonus();
		}
		else {equip_atk_bonus = 0;}
		
		if (equipment[0].get_level() > 0 && equipment[0].broken() == false) {
			equip_health_bonus = equipment[0].get_health_bonus();
		}
		else {equip_health_bonus = 0;}
		
		//establish attribute bonuses
		str_bonus = agi_bonus = int_bonus = sta_bonus = 0; //initialize
		if (equipment[0].get_level() > 0 && equipment[0].broken() == false) {
			if (equipment[0].get_subtype() == "Mail") {
				str_bonus += equipment[0].attrib_value();
				sta_bonus += equipment[0].attrib_value();
			}
			else if (equipment[0].get_subtype() == "Leather") {
				agi_bonus += equipment[0].attrib_value();
			}
			else if (equipment[0].get_subtype() == "Cloth") {
				int_bonus += equipment[0].attrib_value();
			}
		}
		if (equipment[1].get_level() > 0 && equipment[1].broken() == false) {
			if (equipment[1].get_subtype() == "Sword") {
				str_bonus += equipment[1].attrib_value();
				sta_bonus += equipment[1].attrib_value();
			}
			else if (equipment[1].get_subtype() == "Dagger") {
				agi_bonus += equipment[1].attrib_value();
			}
			else if (equipment[1].get_subtype() == "Staff") {
				int_bonus += equipment[1].attrib_value();
			}
		}
		if (equipment[2].get_level() > 0 && equipment[2].broken() == false) {
			str_bonus += equipment[2].attrib_value();
			sta_bonus += equipment[2].attrib_value();
		}
		
		//update attributes
		strength = base_str + str_bonus + equip_atk_bonus / ATTRIB_BONUS;
		agility = base_agi + agi_bonus + equip_def_bonus / ATTRIB_BONUS;
		intellect = base_int + int_bonus;
		stamina = base_sta + sta_bonus + equip_health_bonus;
		
		//sets ATK and DEF
		atk = strength * 2 + agility + equip_atk_bonus;
		def = agility * 2 + equip_def_bonus;
		
		//sets max health and mana
		max_health = 10 + stamina * 2 + equip_health_bonus;
		max_mana = 0;
		
		//sets crit chance
		crit = 5;
		
		//sets dodge and parry chances
		dodge = 5;
		parry = 5;
		
		//sets accuracy
		accuracy = 90;
	}
	
	//EQUIPMENT MANIPULATION
	void unequip_item(Item item) {
		if (item.get_level() > 0) {
			if (item.get_type() == "Armor") {
				equipment[0].set_item("Armor", "Cloth", 0);
				inc_inventory(item);
				update_stats();
				cout << "\nYou've removed " << item.get_name() << "." << endl;
				Pause();
			}
			else if (item.get_type() == "Weapon") {
				equipment[1].set_item("Weapon", "Staff", 0);
				inc_inventory(item);
				update_stats();
				cout << "\nYou've removed " << item.get_name() << "." << endl;
				Pause();
			}
			else if (item.get_type() == "Shield") {
				equipment[2].set_item("Shield", "Shield", 0);
				inc_inventory(item);
				update_stats();
				cout << "\nYou've removed " << item.get_name() << "." << endl;
				Pause();
			}
		}
		else {
			cout << "You don't have anything to unequip from that slot." << endl;
			Pause();
		}
	}
	void equip_item(Item item) {
		//failures
		if (item.get_type() == "Offhand") {
			cout << "You cannot equip offhand items" << endl;
			Pause();
			return;
		}
		//assuming no failures...
		else if (item.get_type() == "Armor") {
			if (equipment[0].get_level() > 0) {unequip_item(equipment[0]);}
			equipment[0] = item;
		}
		else if (item.get_type() == "Weapon") {
			if (equipment[1].get_level() > 0) {unequip_item(equipment[1]);}
			equipment[1] = item;
		}
		else if (item.get_type() == "Shield") {
			if (equipment[2].get_level() > 0) {unequip_item(equipment[2]);}
			equipment[2] = item;
		}
		update_stats();
		dec_inventory(item);
		cout << "\nYou've equipped " << item.get_name() << "." << endl;
		Pause();
	}
	
private:
	void init_equipment() {
		equipment[0].set_item("Armor", "Cloth", 0);
		equipment[1].set_item("Weapon", "Staff", 0);
		equipment[2].set_item("Shield", "Shield", 0);
	}
	
	//provides descriptions of the four base attributes
	void set_descriptions() {
		description_strength = "Strength adds to your attack rating. ";
		description_strength += "One point of STR equates to two ATK points.";
		description_agility = "Agility adds to your attack rating, as well as your defense rating.";
		description_agility += "One point of AGI equates to one ATK point and two DEF points.";
		description_intellect = "Intellect has no effect.";
		description_stamina = "Stamina adds to your maximum health. ";
		description_stamina += "One point of STA equates to two health points.";
	}
};
class Rogue: public Player {
public:
	Rogue(string player_name) {
		name = player_name;
		player_class = "Rogue";
		set_descriptions();
		init_attribs();
		points = 0;
		base_agi += CLASS_BONUS;
		boosted = 0;
	}
	
	void set_player() {
		level = 1;
		exp_to_next_level = LEVEL_INCREMENT * level * level;
		exp = items_in_inventory = 0;
		gold = 300;
		points = 10;
		update_stats();
		attribs_menu();
		current_health = max_health;
		init_equipment();
		for (int i = 0; i < MAX_LEVEL; i++) {boss_quests_complete[i] = false;}
	}
	
	//COMBAT
	void boost_atk() {
		atk *= 2;
		boosted = 1;
		cout << "You've doubled your ATK!" << endl;
	}
	void lower_atk() {
		atk /= 2;
		boosted = 0;
	}
	
	//establishes stats and attributes, and takes bonuses into consideration
	void update_stats() {
		//update equipment bonuses
		if (equipment[0].get_level() > 0 && equipment[0].broken() == false) {
			equip_def_bonus = equipment[0].stat_value();
			equip_health_bonus = equipment[0].get_health_bonus();
		}
		else {equip_def_bonus = equip_health_bonus = 0;}
		
		if (equipment[1].get_level() > 0 && equipment[2].get_level() > 0) {
			if (equipment[1].broken() && equipment[2].broken()) {equip_atk_bonus = 0;}
			else if (equipment[1].broken()) {
				equip_atk_bonus = equipment[2].get_atk_bonus();
			}
			else if (equipment[2].broken()) {
				equip_atk_bonus = equipment[1].get_atk_bonus();
			}
			else {
				int weapon_gain1 = equipment[1].get_atk_bonus();
				int weapon_gain2 = equipment[2].get_atk_bonus();
				equip_atk_bonus = weapon_gain1 + weapon_gain2;
			}
		}
		else if (equipment[1].get_level() > 0 && equipment[1].broken() == false) {
			equip_atk_bonus = equipment[1].get_atk_bonus();
		}
		else if (equipment[2].get_level() > 0 && equipment[2].broken() == false) {
			equip_atk_bonus = equipment[2].get_atk_bonus();
		}
		else {equip_atk_bonus = 0;}
		
		//establish attribute bonuses
		str_bonus = agi_bonus = int_bonus = sta_bonus = 0; //initialize
		if (equipment[0].get_level() > 0 && equipment[0].broken() == false) {
			if (equipment[0].get_subtype() == "Leather") {
				agi_bonus += equipment[0].attrib_value();
			}
			else if (equipment[0].get_subtype() == "Cloth") {
				int_bonus += equipment[0].attrib_value();
			}
		}
		if (equipment[1].get_level() > 0 && equipment[1].broken() == false) {
			if (equipment[1].get_subtype() == "Sword") {
				str_bonus += equipment[1].attrib_value();
				sta_bonus += equipment[1].attrib_value();
			}
			else if (equipment[1].get_subtype() == "Dagger") {
				agi_bonus += equipment[1].attrib_value();
			}
			else if (equipment[1].get_subtype() == "Staff") {
				int_bonus += equipment[1].attrib_value();
			}
		}
		if (equipment[2].get_level() > 0 && equipment[2].broken() == false) {
			if (equipment[2].get_subtype() == "Sword") {
				str_bonus += equipment[2].attrib_value();
				sta_bonus += equipment[2].attrib_value();
			}
			else if (equipment[2].get_subtype() == "Dagger") {
				agi_bonus += equipment[2].attrib_value();
			}
			else if (equipment[2].get_subtype() == "Staff") {
				int_bonus += equipment[2].attrib_value();
			}
		}
		
		//update attributes
		strength = base_str + str_bonus;
		agility = base_agi + agi_bonus + equip_atk_bonus / ATTRIB_BONUS;
		intellect = base_int + int_bonus;
		stamina = base_sta + sta_bonus + equip_health_bonus;
		
		//sets ATK and DEF
		atk = agility * 2 + strength + equip_atk_bonus;
		def = agility + equip_def_bonus;
		
		//sets max health and mana
		max_health = 10 + stamina + equip_health_bonus;
		max_mana = 0;
		
		const int CRIT_LEVEL_BONUS = 3;
		const int PARRY_WEAPON_BONUS = 5;
		//sets crit and parry chance
		if (equipment[1].get_level() > 0 && equipment[2].get_level() > 0) {
			if (equipment[1].broken() && equipment[2].broken()) {
				crit = 10;
				parry = 5;
			}
			else if (equipment[1].broken()) {
				crit = 10 + CRIT_LEVEL_BONUS * level;
				parry = 5 + PARRY_WEAPON_BONUS;
			}
			else if (equipment[2].broken()) {
				crit = 10 + CRIT_LEVEL_BONUS * level;
				parry = 5 + PARRY_WEAPON_BONUS;
			}
			else {
				crit = 10 + CRIT_LEVEL_BONUS * 2 * level;
				parry = 5 + PARRY_WEAPON_BONUS * 2;
			}
		}
		else if (equipment[1].get_level() > 0 && equipment[1].broken() == false) {
			crit = 10 + CRIT_LEVEL_BONUS * level;
			parry = 5 + PARRY_WEAPON_BONUS;
		}
		else if (equipment[2].get_level() > 0 && equipment[2].broken() == false) {
			crit = 10 + CRIT_LEVEL_BONUS * level;
			parry = 5 + PARRY_WEAPON_BONUS;
		}
		else {
			crit = 10;
			parry = 5;
		}
		
		//sets dodge chance and accuracy
		dodge = 10;
		accuracy = 90;
	}

	//EQUIPMENT MANIPULATION
	void unequip_item(Item item) {
		if (item.get_level() > 0) {
			if (item.get_type() == "Armor") {
				equipment[0].set_item("Armor", "Cloth", 0);
				inc_inventory(item);
				update_stats();
				cout << "\nYou've removed " << item.get_name() << "." << endl;
				Pause();
			}
			else if (item.get_type() == "Weapon" && equipment[1].get_name() == item.get_name() && equipment[2].get_name() == item.get_name()) {
				equipment[2].set_item("Weapon", "Staff", 0);
				inc_inventory(item);
				update_stats();
				cout << "\nYou've removed " << item.get_name() << "." << endl;
				Pause();
			}
			else if (item.get_type() == "Weapon" && equipment[1].get_name() == item.get_name()) {
				equipment[1].set_item("Weapon", "Staff", 0);
				inc_inventory(item);
				update_stats();
				cout << "\nYou've removed " << item.get_name() << "." << endl;
				Pause();
			}
			else if (item.get_type() == "Weapon" && equipment[2].get_name() == item.get_name()) {
				equipment[2].set_item("Weapon", "Staff", 0);
				inc_inventory(item);
				update_stats();
				cout << "\nYou've removed " << item.get_name() << "." << endl;
				Pause();
			}
		}
		else {
			cout << "You don't have anything to unequip from that slot." << endl;
			Pause();
		}
	}
	void equip_item(Item item) {
		//failures
		if (item.get_type() == "Offhand") {
			cout << "You cannot equip offhand items" << endl;
			Pause();
			return;
		}
		else if (item.get_type() == "Shield") {
			cout << "You cannot equip shields." << endl;
			Pause();
			return;
		}
		else if (item.get_type() == "Armor") {
			if (item.get_subtype() == "Mail") {
				cout << "You cannot equip mail armor." << endl;
				Pause();
				return;
			}
			//assuming no failures...
			if (equipment[0].get_level() > 0) {unequip_item(equipment[0]);}
			equipment[0] = item;
		}
		//assuming no failures...
		else if (item.get_type() == "Weapon") {
			if (equipment[1].get_level() > 0 && equipment[2].get_level() > 0) {
				unequip_item(equipment[2]);
				equipment[2] = item;
			}
			else if (equipment[1].get_level() > 0) {equipment[2] = item;}
			else if (equipment[2].get_level() > 0) {equipment[1] = item;}
			else {equipment[1] = item;}
		}
		update_stats();
		dec_inventory(item);
		cout << "\nYou've equipped " << item.get_name() << "." << endl;
		Pause();
	}
	
private:
	void init_equipment() {
		equipment[0].set_item("Armor", "Cloth", 0);
		equipment[1].set_item("Weapon", "Staff", 0);
		equipment[2].set_item("Weapon", "Staff", 0);
	}
	
	//provides descriptions of the four base attributes
	void set_descriptions() {
		description_strength = "Strength adds to your attack rating. ";
		description_strength += "One point of STR equates to one ATK point.";
		description_agility = "Agility adds to your attack rating, as well as your defense rating.";
		description_agility += "One point of AGI equates to two ATK points and one DEF point.";
		description_intellect = "Intellect has no effect.";
		description_stamina = "Stamina adds to your maximum health. ";
		description_stamina += "One point of STA equates to one health point.";
	}
};
class Mage: public Player {
public:
	Mage(string player_name) {
		name = player_name;
		player_class = "Mage";
		set_descriptions();
		init_attribs();
		points = 0;
		base_int += CLASS_BONUS;
		boosted = 0;
	}
	
	void set_player() {
		level = 1;
		exp_to_next_level = LEVEL_INCREMENT * level * level;
		exp = items_in_inventory = 0;
		gold = 300;
		points = 10;
		update_stats();
		attribs_menu();
		current_health = max_health;
		current_mana = max_mana;
		init_equipment();
		init_spellbook();
		for (int i = 0; i < MAX_LEVEL; i++) {boss_quests_complete[i] = false;}
	}
	
	void init_spellbook() {
		const int LOWEST_MAG = 20; //starting point for magnitude algorithm
		const int LOWEST_COST = 15; //starting point for cost algorithm
		const int MAG_STEP = 30; //jump increment between magnitudes
		string names[SPELL_COUNT] = {	"Fireball", 				"Salve", 					"Steelskin"};
		string types[SPELL_COUNT] = {	"Offensive", 				"Healing", 					"Defensive"};
		int magnitudes[SPELL_COUNT] = {	LOWEST_MAG + MAG_STEP * 1, 	LOWEST_MAG + MAG_STEP * 0, 	LOWEST_MAG + MAG_STEP * 2};
		int costs[SPELL_COUNT] = {		LOWEST_COST,				LOWEST_COST * 2,			LOWEST_COST * 3};
		
		for (int i = 0; i < SPELL_COUNT; i++) {
			spellbook[i].set_spell(names[i], types[i], magnitudes[i], costs[i], level);
		}
	}
	
	//establishes stats and attributes, and takes bonuses into consideration
	void update_stats() {
		//update equipment bonuses
		if (equipment[0].get_level() > 0 && equipment[0].broken() == false) {
			equip_def_bonus = equipment[0].stat_value();
			equip_health_bonus = equipment[0].get_health_bonus();
		}
		else {equip_def_bonus = equip_health_bonus = 0;}
		
		if (equipment[1].get_level() > 0 && equipment[2].get_level() > 0) {
			if (equipment[1].broken() && equipment[2].broken()) {equip_atk_bonus = equip_mana_bonus = 0;}
			else if (equipment[1].broken()) {equip_mana_bonus = equipment[2].stat_value();}
			else if (equipment[2].broken()) {
				equip_mana_bonus = equipment[1].get_atk_bonus();
				equip_atk_bonus = equipment[1].get_atk_bonus();
			}
			else {
				int weapon_gain = equipment[1].get_atk_bonus();
				int offhand_gain = equipment[2].stat_value();
				if (equipment[1].get_subtype() == "Staff") {
					equip_mana_bonus = weapon_gain + offhand_gain;
				}
				else {equip_mana_bonus = offhand_gain;}
				equip_atk_bonus = weapon_gain;
			}
		}
		else if (equipment[1].get_level() > 0 && equipment[1].broken() == false) {
			equip_mana_bonus = equipment[1].get_atk_bonus();
			equip_atk_bonus = equipment[1].get_atk_bonus();
		}
		else if (equipment[2].get_level() > 0 && equipment[2].broken() == false) {
			equip_mana_bonus = equipment[2].stat_value();
		}
		else {equip_atk_bonus = equip_mana_bonus = 0;}
		
		//establish attribute bonuses
		str_bonus = agi_bonus = int_bonus = sta_bonus = 0; //initialize
		if (equipment[0].get_level() > 0 && equipment[0].broken() == false) {
			int_bonus += equipment[0].attrib_value();
		}
		if (equipment[1].get_level() > 0 && equipment[1].broken() == false) {
			if (equipment[1].get_subtype() == "Sword") {
				str_bonus += equipment[1].attrib_value();
				sta_bonus += equipment[1].attrib_value();
			}
			else if (equipment[1].get_subtype() == "Dagger") {agi_bonus += equipment[1].attrib_value();}
			else if (equipment[1].get_subtype() == "Staff") {int_bonus += equipment[1].attrib_value();}
		}
		if (equipment[2].get_level() > 0 && equipment[2].broken() == false) {
			int_bonus += equipment[2].attrib_value();
		}
		
		//update attributes
		strength = base_str + str_bonus + equip_atk_bonus / ATTRIB_BONUS;
		agility = base_agi + agi_bonus + equip_def_bonus / ATTRIB_BONUS;
		intellect = base_int + int_bonus + equip_mana_bonus / ATTRIB_BONUS;
		stamina = base_sta + sta_bonus + equip_health_bonus;
		
		//sets ATK and DEF
		atk = strength + agility + equip_atk_bonus;
		def = agility * 2 + equip_def_bonus;
		
		//sets max health and mana
		max_health = 10 + stamina + equip_health_bonus;
		max_mana = intellect * 2 + equip_mana_bonus;
		
		//sets crit chance
		crit = 5;
		
		//sets dodge and parry chances
		dodge = 5;
		parry = 5;
		
		//sets accuracy
		accuracy = 90;
		
		//updates spellpower
		spellpower = intellect / ATTRIB_BONUS;
		for (int i = 0; i < SPELL_COUNT; i++) {
			int x = spellbook[i].get_magnitude();
			spellbook[i].load_magnitude(x + spellpower);
		}
	}
	
	//EQUIPMENT MANIPULATION
	void unequip_item(Item item) {
		if (item.get_level() > 0) {
			if (item.get_type() == "Armor") {
				equipment[0].set_item("Armor", "Cloth", 0);
				inc_inventory(item);
				update_stats();
				cout << "\nYou've removed " << item.get_name() << "." << endl;
				Pause();
			}
			else if (item.get_type() == "Weapon") {
				equipment[1].set_item("Weapon", "Staff", 0);
				inc_inventory(item);
				update_stats();
				cout << "\nYou've removed " << item.get_name() << "." << endl;
				Pause();
			}
			else if (item.get_type() == "Offhand") {
				equipment[2].set_item("Offhand", "Orb", 0);
				inc_inventory(item);
				update_stats();
				cout << "\nYou've removed " << item.get_name() << "." << endl;
				Pause();
			}
		}
		else {
			cout << "You don't have anything to unequip from that slot." << endl;
			Pause();
		}
	}
	void equip_item(Item item) {
		//failures
		if (item.get_type() == "Shield") {
			cout << "You cannot equip shields." << endl;
			Pause();
			return;
		}
		else if (item.get_type() == "Armor") {
			if (item.get_subtype() != "Cloth") {
				cout << "You cannot equip leather or mail armor." << endl;
				Pause();
				return;
			}
			//assuming no failures...
			if (equipment[0].get_level() > 0) {unequip_item(equipment[0]);}
			equipment[0] = item;
		}
		//assuming no failures...
		else if (item.get_type() == "Weapon") {
			if (equipment[1].get_level() > 0) {unequip_item(equipment[1]);}
			equipment[1] = item;
		}
		else if (item.get_type() == "Offhand") {
			if (equipment[2].get_level() > 0) {unequip_item(equipment[2]);}
			equipment[2] = item;
		}
		update_stats();
		dec_inventory(item);
		cout << "\nYou've equipped " << item.get_name() << "." << endl;
		Pause();
	}
	
	//VIEW FUNCTIONS
	void view_stats() {
		Clear();
		cout << "Your current stats are: " << endl;
		cout << "ATK: " << atk << endl;
		cout << "DEF: " << def << endl;
		cout << "Health: " << current_health << "/" << max_health << endl;
		cout << "Mana: " << current_mana << "/" << max_mana << endl;
		cout << "Spellpower: " << spellpower << endl;
		cout << "\nExperience: " << exp << "/" << exp_to_next_level << endl;
		
	}
	void view_mana() {cout << "Your mana: " << current_mana << "/" << max_mana << endl << endl;}
	
	//MANA MANIPULATION
	void restore_mana(int x) {
		current_mana += x;
		if (current_mana > max_mana) {restore_mana_to_max();}
	}
	void restore_mana_to_max() {current_mana = max_mana;}
	void lose_mana(int x) {current_mana -= x;}
	
	//COMBAT
	void boost_def(int x) {
		def += x;
		boosted += 1;
	}
	void lower_def() {
		def -= spellbook[2].get_magnitude();
		boosted -= 1;
	}
	int cast_spell(bool in_combat) {
		cast_spell_start:
		Clear();
		cout << "What spell would you like to cast?" << endl;
		view_spells();
		cout << SPELL_COUNT + 1 << ". Cancel request (go to the previous menu)" << endl;
		
		int spell_dmg = 0;
		int choice;
		while (true) {
			choice = GetInt();
			choice--;
			if (choice < SPELL_COUNT && choice >= 0) {
				if (spellbook[choice].get_cost() > current_mana) {
					cout << "You don't have enough mana to cast this. YOU NEED MORE JUICE!!!" << endl;
					Pause();
					goto cast_spell_start;
				}
				else {
					if (spellbook[choice].get_type() == "Offensive") {
						if (in_combat == true) {
							cout << "You cast " << spellbook[choice].get_name() << "!" << endl;
							spell_dmg = spellbook[choice].get_magnitude();
							lose_mana(spellbook[choice].get_cost());
							did_cast_spell = true;
						}
						else {
							cout << "You can't cast " << spellbook[choice].get_name() << " out of combat!" << endl;
							Pause();
							goto cast_spell_start;
						}
					}
					else if (spellbook[choice].get_type() == "Healing") {
						cout << "You cast " << spellbook[choice].get_name() << "!" << endl;
						restore_health(spellbook[choice].get_magnitude());
						cout << "You've restored " << spellbook[choice].get_magnitude() << " health!" << endl;
						Pause();
						lose_mana(spellbook[choice].get_cost());
						did_cast_spell = true;
					}
					else if (spellbook[choice].get_type() == "Defensive") {
						if (in_combat == true) {
							cout << "You cast " << spellbook[choice].get_name() << "!" << endl;
							boost_def(spellbook[choice].get_magnitude());
							cout << "You've boosted your DEF by " << spellbook[choice].get_magnitude() << endl;
							Pause();
							lose_mana(spellbook[choice].get_cost());
							did_cast_spell = true;
						}
						else {
							cout << "You can't cast " << spellbook[choice].get_name() << " out of combat!" << endl;
							Pause();
							goto cast_spell_start;
						}
					}
					return spell_dmg;
				}
			}
			else if (choice == SPELL_COUNT) {
				did_cast_spell = false;
				return spell_dmg;
			}
			else {cout << "Please enter a number between 1 and " << SPELL_COUNT + 1 << ": ";}
		}
	}
	
	void level_up() {
		Player::level_up();
		for (int i = 0; i < SPELL_COUNT; i++) {spellbook[i].inc_level();}
	}
	
	//GET FUNCTIONS
	bool get_did_cast_spell() {return did_cast_spell;}
	Spell* get_spellbook() {return spellbook;}
	int get_current_mana() {return current_mana;}
	
private:
	//VARIABLES
	static const int SPELL_COUNT = 3;
	Spell spellbook[SPELL_COUNT];
	int current_mana, equip_mana_bonus, spellpower;
	bool did_cast_spell;

	
private:
	void init_equipment() {
		equipment[0].set_item("Armor", "Cloth", 0);
		equipment[1].set_item("Weapon", "Staff", 0);
		equipment[2].set_item("Offhand", "Orb", 0);
	}
	
	void view_spells() {
		for (int i = 0; i < SPELL_COUNT; i++) {
			cout << i + 1 << ". " << spellbook[i].get_name() << " - " << spellbook[i].get_cost() << " mana" << endl;
		}
	}
	
	//provides descriptions of the four base attributes
	void set_descriptions() {
		description_strength = "Strength adds to your attack rating. ";
		description_strength += "One point of STR equates to one ATK point.";
		description_agility = "Agility adds to your attack rating, as well as your defense rating.";
		description_agility += "One point of AGI equates to one ATK point and two DEF points.";
		description_intellect = "Intellect adds to your spellpower and mana pool. ";
		description_intellect += "One point of INT equates to two mana points and one-fifth points of spellpower.";
		description_stamina = "Stamina adds to your maximum health. ";
		description_stamina += "One point of STA equates to one health point.";
	}
};

//shop definition and subtypes
class Shop {
protected:
	//VARIABLES
	Item *inventory;
	string display_inventory_flavor, buy_flavor, sell_flavor, entrance_flavor, leave_flavor, menu_flavor;
	int size_of_inventory, level;
	
	//shop inventory
	void display_inventory() {
		Clear();
		cout << display_inventory_flavor << endl << endl;
		for (int i = 0; i < size_of_inventory; i++) {
			cout << i + 1 << ". " << inventory[i].get_name() << " (Requires Level "
			<< inventory[i].get_level() << ") - " << inventory[i].get_buy_value() << " gold" << endl;
		}
	}

	//buying menu
	void buy(Player* &player) {
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

	//selling menu
	void sell(Player* &player) {
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
};
class Blacksmith: public Shop {
public:
	Blacksmith(int shop_level) {
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

	//top level menu
	void menu(Player* &player) {
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

private:
	//initializes inventory
	void init_inventory() {
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
	
	void repair(Player* &player) {
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
};
class Inn: public Shop {
public:
	Inn(int shop_level) {
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
	
	//top level menu
	void menu(Player* &player) {
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

private:
	//VARIABLES
	Item room;
	
	//initializes inventory
	void init_inventory() {
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
	
	//establishes room for rent
	void init_room() {room.set_item("Room", "Room", level);}
	
	//room menu
	void stay(Player* &player) {
		Clear();
		cout << "Innkeeper: 'Our " << room.get_name() << " is available to rent for " << room.get_buy_value()
		<< " gold. We also offer 'specials' for some of our more...distinguished guests.'\n" << endl;
		player->view_gold();
		cout << "\nWould you like to stay the night (Y/N)? ";
		
		char choice;
		while (true) {
			choice = GetChar();
			switch (choice) {
				case 'Y':
				case 'y':
					if (player->get_gold() >= room.get_buy_value()) {
						player->remove_gold(room.get_buy_value());
						player->restore_health_to_max();
						if (player->get_player_class() == "Mage") {player->restore_mana_to_max();}
						Clear();
						cout << "You've stayed in the " << room.get_name() << ", and your health and mana have "
						<< "recovered fully. Anything else, such as certain people sneaking into your room in the middle "
						<< "of the night, you'd rather not think about too much." << endl;
						Pause();
						return;
					}
					else {
						cout << "You don't have enough gold." << endl;
						Pause();
						return;
					}
					break;
				case 'N':
				case 'n':
					cout << "You decided not to stay the night. Perhaps it had something to do with how the innkeeper has been "
					<< "staring at you this entire time...." << endl;
					Pause();
					return;
					break;
				default:
					cout << "Please enter Y or N: ";
			}
		}
	}
};

//MAPS AND DUNGEONS
class Map {
public:
	Map(int player_level) {
		level = player_level;
		size = 3 + level; //number of spaces in a row
		area = size * size;
		space = 'O';
		init_map();
		set_town();
		set_boss();
		explore_bonus_earned = false;
	}
	
	void init_map() {
		map = new char[area];
		for (int i = 0; i < area; i++) {
			map[i] = space;
		}
	}
	
	void draw_map() {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				cout << map[i * size + j] << " ";
			}
			cout << endl;
		}
	}
	
	int move_player(int start, char direction) {
		int finish;
		if (direction == 'N') {finish = start - size;}
		if (direction == 'W') {finish = start - 1;}
		if (direction == 'E') {finish = start + 1;}
		if (direction == 'S') {finish = start + size;}
		
		if (map[start] != 'T') {map[start] = 'X';}
		map[finish] = '*';
		return finish;
	}
	
	void enter_town(int position) {
		if (position == town) {
			cout << "You stumble back to the entrance of town. You can't believe you managed to survive out there "
			<< "amidst nature's chaos!" << endl;
			Pause();
		}
	}
	
	bool explored_map() {
		for (int i = 0; i < area; i++) {
			if (map[i] == 'O') {return false;}
		}
		return true; //only is reached if none of the map coords are 'O'
	}
	
	//verifies that player can move in a particular direction
	bool can_move_north(int position) {
		if (position > size - 1) {return true;}
		else {return false;}
	}
	bool can_move_south(int position) {
		if (position < (area) - size) {return true;}
		else {return false;}
	}
	bool can_move_west(int position) {
		if (position % size > 0) {return true;}
		else {return false;}
	}
	bool can_move_east(int position) {
		if (position % size != size - 1) {return true;}
		else {return false;}
	}
	
	//GET FUNCTIONS
	int get_town() {return town;}
	int get_boss() {return boss;}
	int get_area() {return area;}
	int get_level() {return level;}
	bool get_explore_bonus_earned() {return explore_bonus_earned;}
	bool get_boss_present() {return boss_present;}
	
	//SET FUNCTIONS
	void set_explore_bonus_earned() {explore_bonus_earned = true;}
	
	void killed_boss() {boss_present = false;}
	
private:
	//VARIABLES
	char *map;
	char space;
	int level, size, town, area, boss;
	bool explore_bonus_earned, boss_present;
	
	//SET FUNCTIONS
	void set_town() {
		town = rand() % area;
		map[town] = 'T';
	}
	void set_boss() {
		boss = rand() % area;
		while (boss == town) {boss = rand() % area;}
		boss_present = true;
	}
};
class Dungeon {
public:
	Dungeon(int dungeon_level) {
		level = dungeon_level;
		complexity = STAGES * level;
		stage[0] = "You can't see anything in front of you, but you know your quarry lies in wait further in.";
		stage[1] = "As you move further in, you notice a lightening of the darkness surrounding you. You must be on the right track.";
		stage[2] = "You're able to start making shapes out of the darkness. You can see the floor, parts of the walls. You feel like you're getting close.";
		fork = "You come across what appears to be a fork. You pick a direction at random.";
		end = "Abruptly, you break into a large room. A strange light comes from the creature ";
		end += "before you, as it rears to attack!";
		init_flavor();
	}
	
	void move() {
		int success_chance = 70;
		int progress = 0;
		
		Clear();
		cout << entrance << endl;
		Pause();
		while (progress <= complexity) {
			cout << fork << endl;
			if (rand() % 101 < success_chance) {
				cout << success << endl;
				Pause();
				for (int i = 0; i < STAGES; i++) {
					if (progress == level * i) {
						Clear();
						cout << stage[i] << endl;
						Pause();
					}
				}
				progress++;
			}
			else {
				cout << deadend << endl;
				Pause();
			}
		}
		cout << end << endl;
	}
	
private:
	static const int STAGES = 3;
	string stage[STAGES];
	string success, deadend, fork, entrance, end;
	int complexity, level;
	
	void init_flavor() {
		if (level == 1) {
			entrance = "You enter a dark passage, hearing a distant grunting.";
			success = "You move forward, and encounter no resistance or barriers. ";
			success += "You hear groaning further down the passage. ";
			deadend = "As you move forward, you come up against a wall. It's a dead-end! ";
			deadend += "The echoes from the moans and grunts disorient you further as you attempt to backtrack.";
		}
		else if (level == 2) {
			entrance = "You enter a dank cave. The smell is almost overwhelming!";
			success = "You move forward, and encounter no resistance or barriers. ";
			success += "You smell something approaching that of manure. You believe it's coming ";
			success += "from the direction you just traveled.";
			deadend = "As you move forward, you come up against a wall. It's a dead-end! ";
			deadend += "Another wave of staggering stench buffets you. You close your eyes against it, ";
			deadend += "even though it's pitch-black. You back up, bumping into a wall, and trace it back ";
			deadend += "to the last fork.";
		}
		else if (level == 3) {
			entrance = "You enter a pitch-black grotto. You hearing mewling deeper in.";
			success = "You move forward, and encounter no resistance or barriers. ";
			success += "Mewls come from deeper in the grotto. You squint, trying to see something in the blackness.";
			deadend += "Following a particularly loud yowl, you slam against a wall and stagger. You shake your ";
			deadend += "and back up.";
		}
		else if (level == 4) {
			entrance = "You enter a dim cavern. You can barely see anything in front of you.";
			success = "Rumbles sound from the deep parts of the cavern. You edge forward warily.";
			deadend += "As you inch forward, the floor abruptly gives way. You fall into the depression. ";
			deadend += "You climb back out, brushing yourself off.";
		}
		else if (level == 5) {
			entrance = "You enter an utterly dark and humid hallway. You hear a distant laugh further in.";
			success = "You hear what sounds like a party further down the hallway. As you move forward, ";
			success += "you think you detect a lightening of the gloom around you.";
			deadend += "The hallway curves around, abruptly ending. You back up to the fork.";
		}
		else {cout << "Not a valid level (Dungeon)" << endl;}
	}	
};

//LOAD-SAVE FUNCTIONS
bool saves_exist() {
	ifstream file;
	file.open("save_games.txt");
	if (file.is_open()) {return true;}
	else {return false;}
}
void save_game(Player* &player) {
	string filename;
	fstream file;
	save_game_start:
	Clear();
	cout << "To create a save game, enter a name: ";
	getline(cin, filename);
	
	string filename_short = filename;
	
	int player_level = player->get_level();
	switch (player_level) {
		case 1:
			filename += ", Level 1";
			break;
		case 2:
			filename += ", Level 2";
			break;
		case 3:
			filename += ", Level 3";
			break;
		case 4:
			filename += ", Level 4";
			break;
		case 5:
			filename += ", Level 5";
			break;
	}
	
	//save name of game to a separate file
	file.open("save_games.txt", fstream::in);
	bool save_exists = false;
	char line[30];
	for (int i = 0; file.good(); i++) {
		file.getline(line, 30, '\n');
		if (line == filename) {save_exists = true;}
	}
	file.close();
	if (save_exists) {
		cout << "A game already exists with this name. Overwrite (Y/N)? ";
		char decision;
		while (true) {
			decision = GetChar();
			if (decision == 'Y' || decision == 'y') {break;}
			else if (decision == 'N' || decision == 'n') {goto save_game_start;}
			else {cout << "Please enter Y or N: ";}
		}
	}
	else {
		file.open("save_games.txt", fstream::out |  fstream::app);
		file << filename_short << ", Level " << player_level << endl;
		file.close();
	}
	
	cout << "\nSaving game...." << endl;
	
	//attributes and stats
	int *base_attribs = player->get_base_attribs();
	int *exp_gold_meters_inv_count = player->get_exp_gold_meters_inv_count();
	const int NUM_OF_BASE_ATTRIBS = 4;
	const int NUM_OF_MISC = 5;
	file.open(filename_short + "_int.txt", fstream::out);
	for (int i = 0; i < NUM_OF_BASE_ATTRIBS; i++) {file << base_attribs[i] << endl;}
	for (int i = 0; i < NUM_OF_MISC; i++) {file << exp_gold_meters_inv_count[i] << endl;}
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
	
	//inventory - only if there is any
	if (exp_gold_meters_inv_count[NUM_OF_MISC - 1] > 0) {
		Item *item = player->get_inventory();
		for (int i = 0; i < exp_gold_meters_inv_count[NUM_OF_MISC - 1]; i++) {
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
	const int EQUIPMENT_SLOTS = 3;
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
	string filename;
	ifstream file;
	Clear();
	cout << "Please choose a game to load:" << endl;
	
	//display list of saved games
	string line;
	const int NUM_OF_SAVES = 3;
	string saves[NUM_OF_SAVES];
	string saves_short[NUM_OF_SAVES];
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
	char c[20];
	string name;
	string loaded_class;
	for (int i = 0; file.good(); i++) {
		file.getline(c, 20, '\n');
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
	
	int x;
	const int NUM_OF_BASE_ATTRIBS = 4;
	int *base_attribs = new int[NUM_OF_BASE_ATTRIBS];
	file.open(filename + "_int.txt");
	for (int i = 0; i < NUM_OF_BASE_ATTRIBS; i++) {
		file >> x;
		base_attribs[i] = x;
	}
	file.close();
	player->load_base_attribs(base_attribs);
	
	int int_count = NUM_OF_BASE_ATTRIBS;
	
	const int NUM_OF_MISC = 5;
	int *exp_gold_meters_inv_count = new int[NUM_OF_MISC];
	file.open(filename + "_int.txt");
	int j = 0;
	for (int i = 0; i < int_count + NUM_OF_MISC; i++) {
		file >> x;
		if (i == int_count + j) {
			exp_gold_meters_inv_count[j] = x;
			j++;
		}
	}
	file.close();
	player->load_exp_gold_meters_inv_count(exp_gold_meters_inv_count);
	
	int_count += NUM_OF_MISC;
	
	//quests - always MAX_LEVEL number
	bool *boss_quest_completion = new bool[MAX_LEVEL];
	file.open(filename + "_int.txt");
	for (int i = 0; file.good(); i++) {
		file >> x;
		for (int j = 0; j < MAX_LEVEL; j++) {
			if (i == int_count + j) {
				if (x == 1) {boss_quest_completion[j] = true;}
				else {boss_quest_completion[j] = false;}
			}
		}
	}
	file.close();
	player->load_boss_quest_completion(boss_quest_completion);
	
	int_count += MAX_LEVEL;
	
	//inventory
	int int_length, char_length; //number of inventory lines
	if (exp_gold_meters_inv_count[NUM_OF_MISC - 1] > 0) {
		char_length = exp_gold_meters_inv_count[NUM_OF_MISC - 1] * 2;
		int_length = exp_gold_meters_inv_count[NUM_OF_MISC - 1] * 2;
		Item *inv;
		inv = new Item[exp_gold_meters_inv_count[NUM_OF_MISC - 1]];
		
		int j = 0; //index of inventory
		char c_inv[30];
		for (j = 0; j < exp_gold_meters_inv_count[NUM_OF_MISC - 1]; j++) {
			file.open(filename + "_char.txt");
			for (int i = 0; file.good(); i++) {
				file.getline(c_inv, 30, '\n');
				if ((i >= 2) && (i < 2 + char_length)) { //ensures it stays within inventory bounds
					if ((i - 2) % char_length == 2 * j) {inv[j].load_type(c_inv);}
					if ((i - 2) % char_length == (2 * j) + 1) {inv[j].load_subtype(c_inv);}
				}
			}
			file.close();
		}
		
		int x_inv;
		for (j = 0; j < exp_gold_meters_inv_count[NUM_OF_MISC - 1]; j++) {
			file.open(filename + "_int.txt");
			for (int i = 0; file.good(); i++) {
				file >> x_inv;
				if ((i >= int_count) && (i < int_count + int_length)) { //ensures it stays within inventory bounds
					if ((i - int_count) % int_length == 2 * j) {inv[j].load_level(x_inv);}
					if ((i - int_count) % int_length == (2 * j) + 1) {inv[j].load_quantity(x_inv);}
				}
			}
			file.close();
		}

		for (int i = 0; i < exp_gold_meters_inv_count[NUM_OF_MISC - 1]; i++) {
			int num = inv[i].get_quantity(); //backup quantity
			inv[i].set_item(inv[i].get_type(), inv[i].get_subtype(), inv[i].get_level()); //init_item
			inv[i].load_quantity(num); //restore original quantity
		}
		
		player->load_inventory(inv);
	}
	//if no inventory
	else {
		char_length = 0;
		int_length = 0;
	}
	
	//equipment
	const int EQUIPMENT_SLOTS = 3;
	Item *equipment = new Item[EQUIPMENT_SLOTS];
	
	//copy in equipment details
	char c_equip[30];
	j = 0; //index of equipment
	for (j = 0; j < EQUIPMENT_SLOTS; j++) {
		file.open(filename + "_char.txt");
		for (int i = 0; file.good(); i++) {
			file.getline(c_equip, 30, '\n');
			if (i >= 2 + char_length) {
				if ((i - 2 - char_length) % 7 == 2 * j) {equipment[j].load_type(c_equip);}
				if ((i - 2 - char_length) % 7 == (2 * j) + 1) {equipment[j].load_subtype(c_equip);}
			}
		}
		file.close();
	}
	int x_equip;
	for (j = 0; j < EQUIPMENT_SLOTS; j++) {
		file.open(filename + "_int.txt");
		for (int i = 0; file.good(); i++) {
			file >> x_equip;
			if (i >= int_count + int_length) {
				if ((i - int_count - int_length) % 4 == j) {equipment[j].load_level(x_equip);}
			}
		}
		file.close();
	}
	
	//initialize proper equipment
	player->load_equipment(equipment);
	
	player->load_level(exp_gold_meters_inv_count[0]);
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
void load_or_create_player_menu(Player* &player) {
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
					else {cout << "Oops! (load_or_create_player_menu)" << endl;}
					player->set_quests();
					return;
					break;
				default:
					cout << "Please enter L or N: ";
					break;
			}
		}
	}
	else {
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
		else {cout << "Oops! (load_or_create_player_menu)" << endl;}
		player->set_quests();
	}
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

//MENUS + helper functions
void player_menu(Player* &player) {
	player_menu_start:
	Clear();

	cout << "Player menu:\n" << endl;
	cout << "What would you like to do?\n" << endl;
	
	int count = 1;
	cout << count << ".  View attributes" << endl;
	count++;
	cout << count << ".  View stats" << endl;
	count++;
	cout << count << ".  View inventory" << endl;
	count++;
	cout << count << ".  View quest log" << endl;
	count++;
	cout << count << ".  Use item" << endl;
	if (player->get_player_class() == "Mage") {
		count++;
		cout << count << ".  Cast a spell" << endl;
	}
	count++;
	cout << count << ".  Equipment" << endl;
	count++;
	cout << count << ".  Load / Save Player" << endl;
	count++;
	cout << count << ".  Exit menu" << endl;
	count++;
	if (player->get_player_class() == "Mage") {cout << count << ". Quit game" << endl;}
	else {cout << count << ".  Quit game" << endl;}
	
	int choice;
	while (true) {
		choice = GetInt();
		if (player->get_player_class() == "Mage") {
			switch (choice) {
				case 1:
					player->attribs_menu();
					goto player_menu_start;
					break;
				case 2:
					player->view_stats();
					Pause();
					goto player_menu_start;
					break;
				case 3:
					player->view_inventory();
					goto player_menu_start;
					break;
				case 4:
					player->view_quest_log();
					Pause();
					goto player_menu_start;
					break;
				case 5:
					player->use_item();
					goto player_menu_start;
					break;
				case 6:
					if (player->get_player_class() == "Mage") {player->cast_spell(false);}
					goto player_menu_start;
					break;
				case 7:
					player->equipment_menu();
					goto player_menu_start;
					break;
				case 8:
					save_or_load(player);
					goto player_menu_start;
					break;
				case 9:
					return;
					break;
				case 10:
					Clear();
					cout << "Are you sure you sure you want to quit? Don't be that guy, man. "
					<< "Are you really that much of a pathetic, lowlife loser (Y/N)? ";
					char decision;
					while (true) {
						decision = GetChar();
						if (decision == 'Y' || decision == 'y') {exit(0);}
						else if (decision == 'N' || decision == 'n') {
							cout << "That's the spirit, champ!" << endl;
							Pause();
							goto player_menu_start;
						}
						else {cout << "Please enter Y or N: ";}
					}
					break;
				default:
					cout << "Please enter a number between 1 and " << count << ": ";
					break;
			}
		}
		else {
			switch (choice) {
				case 1:
					player->attribs_menu();
					goto player_menu_start;
					break;
				case 2:
					player->view_stats();
					Pause();
					goto player_menu_start;
					break;
				case 3:
					player->view_inventory();
					goto player_menu_start;
					break;
				case 4:
					player->view_quest_log();
					Pause();
					goto player_menu_start;
					break;
				case 5:
					player->use_item();
					goto player_menu_start;
					break;
				case 6:
					player->equipment_menu();
					goto player_menu_start;
					break;
				case 7:
					save_or_load(player);
					goto player_menu_start;
					break;
				case 8:
					return;
					break;
				case 9:
					Clear();
					cout << "Are you sure you sure you want to quit? Don't be that guy, man. "
					<< "Are you really that much of a pathetic, lowlife loser (Y/N)? ";
					char decision;
					while (true) {
						decision = GetChar();
						if (decision == 'Y' || decision == 'y') {exit(0);}
						else if (decision == 'N' || decision == 'n') {
							cout << "That's the spirit, champ!" << endl;
							Pause();
							goto player_menu_start;
						}
					}
					break;
				default:
					cout << "Please enter a number between 1 and " << count << ": ";
					break;
			}
		}
	}
}
void creature_killed(Player* &player, Creature &creature) {
	Item *loot = creature.get_loot();
	int loot_count = creature.get_loot_count();
	while (player->get_boosted() > 0) {
		if (player->get_player_class() == "Warrior") {player->lower_block();}
		else if (player->get_player_class() == "Rogue") {player->lower_atk();}
		else if (player->get_player_class() == "Mage") {player->lower_def();}
	}
	cout << "You've killed the " << creature.get_name() << "!" << endl;
	player->gain_exp(creature.get_exp());
	player->add_gold(creature.get_money());
	if (creature.get_has_loot()) {
		for (int i = 0; i < loot_count; i++) {
			cout << "You've looted " << loot[i].get_name() << " from the " << creature.get_name() << "!" << endl;
			player->inc_inventory(loot[i]);
		}
	}
	Pause();
}
void boss_killed(Player* &player, Map &map, Boss &boss) {
	Item *loot = boss.get_loot();
	int loot_count = boss.get_loot_count();
	while (player->get_boosted() > 0) {
		if (player->get_player_class() == "Warrior") {player->lower_block();}
		else if (player->get_player_class() == "Rogue") {player->lower_atk();}
		else if (player->get_player_class() == "Mage") {player->lower_def();}
	}
	cout << "You've killed " << boss.get_name() << "!" << endl;
	player->gain_exp(boss.get_exp());
	player->add_gold(boss.get_money());
	for (int i = 0; i < loot_count; i++) {
		cout << "You've looted " << loot[i].get_name() << " from " << boss.get_name() << "!" << endl;
		player->inc_inventory(loot[i]);
	}
	map.killed_boss();
	player->complete_active_quest();
	Pause();
}
int fight_display_menu(Player* &player) {
	Clear();
	cout << "What would you like to do?" << endl;
	
	int count = 1;
	cout << count << ". Attack" << endl;
	count++;
	if (player->get_player_class() == "Mage") {cout << count << ". Cast a spell" << endl;}
	else {cout << count << ". Use your class ability" << endl;}
	count++;
	cout << count << ". Use item" << endl;
	count++;
	cout << count << ". Run away\n" << endl;
	
	player->view_health();
	player->view_mana();
	
	return count;
}
void fight_creature(Player* &player) {
	Creature creature;
	creature.det_creature(player->get_level());
	
	cout << "You've encountered a " << creature.get_name() << "!\n" << endl;
	Pause();
	
	fight_creature_start:
	int count = fight_display_menu(player);
	
	cout << creature.get_name() << "'s health: " << creature.get_health() << endl;
	
	int spell_dmg;
	int choice;
	while (true) {
		choice = GetInt();
		switch (choice) {
			case 1:
				creature.take_damage(player->get_atk(), player->get_accuracy(), player->get_crit(), player->get_name());
				if (creature.is_dead()) {
					creature_killed(player, creature);
					return;
				}
				player->take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit(), creature.get_name());
				if (player->is_dead()) {
					player->dies();
					return;
				}
				Pause();
				goto fight_creature_start;
				break;
			case 2:
				if (player->get_player_class() == "Warrior") {
					if (player->get_boosted() == 0) {player->boost_block();}
					else {
						cout << "This ability is already active!" << endl;
						Pause();
						goto fight_creature_start;
					}
					player->take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit(), creature.get_name());
					if (player->is_dead()) {
						player->dies();
						return;
					}
					Pause();
				}
				else if (player->get_player_class() == "Rogue") {
					if (player->get_boosted() == 0) {player->boost_atk();}
					else {
						cout << "This ability is already active!" << endl;
						Pause();
						goto fight_creature_start;
					}
					player->take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit(), creature.get_name());
					if (player->is_dead()) {
						player->dies();
						return;
					}
					Pause();
				}
				else if (player->get_player_class() == "Mage") {
					spell_dmg = player->cast_spell(true);
					if (player->get_did_cast_spell()) {
						creature.take_spell_dmg(spell_dmg, player->get_accuracy(), player->get_crit());
						if (creature.is_dead()) {
							creature_killed(player, creature);
							return;
						}
						player->take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit(), creature.get_name());
						if (player->is_dead()) {
							player->dies();
							return;
						}
						Pause();
					}
				}
				goto fight_creature_start;
				break;
			case 3:
				if (player->use_item()) {
					player->take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit(), creature.get_name());
					if (player->is_dead()) {
						player->dies();
						return;
					}
					Pause();
				}
				goto fight_creature_start;
				break;
			case 4:
				if (rand() % 101 < creature.get_escape_chance()) {
					cout << "You flee like a coward! Were you that scared of a " << creature.get_name() << "?" << endl;
					Pause();
					return;
				}
				else {
					cout << "You tried to run, but the " << creature.get_name() << " bit your ankle and you fell over. LAME." << endl;
					Pause();
					player->take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit(), creature.get_name());
					if (player->is_dead()) {
						player->dies();
						return;
					}
					Pause();
					goto fight_creature_start;
				}
				break;
			default:
				cout << "Please enter a number between 1 and " << count << ": ";
				break;
		}
	}
}
void fight_boss(Player* &player, Map& map) {
	Boss boss(map.get_level());
	cout << "You've encountered " << boss.get_name() << "!\n" << endl;
	Pause();
	
	fight_boss_start:
	int count = fight_display_menu(player);
	
	cout << boss.get_name() << "'s health: " << boss.get_health() << endl;
	
	int spell_dmg;
	int choice;
	while (true) {
		choice = GetInt();
		switch (choice) {
			case 1:
				boss.take_damage(player->get_atk(), player->get_accuracy(), player->get_crit(), player->get_name());
				if (boss.is_dead()) {
					boss_killed(player, map, boss);
					return;
				}
				cout << boss.get_flavor() << endl;
				player->take_damage(boss.get_atk(), boss.get_accuracy(), boss.get_crit(), boss.get_name());
				if (player->is_dead()) {
					player->dies();
					return;
				}
				Pause();
				goto fight_boss_start;
				break;
			case 2:
				if (player->get_player_class() == "Warrior") {
					if (player->get_boosted() == 0) {player->boost_block();}
					else {
						cout << "This ability is already active!" << endl;
						Pause();
						goto fight_boss_start;
					}
					cout << boss.get_flavor() << endl;
					player->take_damage(boss.get_atk(), boss.get_accuracy(), boss.get_crit(), boss.get_name());
					if (player->is_dead()) {
						player->dies();
						return;
					}
					Pause();
				}
				else if (player->get_player_class() == "Rogue") {
					if (player->get_boosted() == 0) {player->boost_atk();}
					else {
						cout << "This ability is already active!" << endl;
						Pause();
						goto fight_boss_start;
					}
					cout << boss.get_flavor() << endl;
					player->take_damage(boss.get_atk(), boss.get_accuracy(), boss.get_crit(), boss.get_name());
					if (player->is_dead()) {
						player->dies();
						return;
					}
					Pause();
				}
				else if (player->get_player_class() == "Mage") {
					spell_dmg = player->cast_spell(true);
					if (player->get_did_cast_spell()) {
						boss.take_spell_dmg(spell_dmg, player->get_accuracy(), player->get_crit());
						if (boss.is_dead()) {
							boss_killed(player, map, boss);
							return;
						}
						cout << boss.get_flavor() << endl;
						player->take_damage(boss.get_atk(), boss.get_accuracy(), boss.get_crit(), boss.get_name());
						if (player->is_dead()) {
							player->dies();
							return;
						}
						Pause();
					}
				}
				goto fight_boss_start;
				break;
			case 3:
				if (player->use_item()) {
					cout << boss.get_flavor() << endl;
					player->take_damage(boss.get_atk(), boss.get_accuracy(), boss.get_crit(), boss.get_name());
					if (player->is_dead()) {
						player->dies();
						return;
					}
					Pause();
				}
				goto fight_boss_start;
				break;
			case 4:
				if (rand() % 101 < boss.get_escape_chance()) {
					cout << "You flee like a coward! Were you that scared of " << boss.get_name() << "?" << endl;
					Pause();
					return;
				}
				else {
					cout << "You tried to run, but " << boss.get_name() << " bit your ankle and you fell over. LAME." << endl;
					Pause();
					player->take_damage(boss.get_atk(), boss.get_accuracy(), boss.get_crit(), boss.get_name());
					if (player->is_dead()) {
						player->dies();
						return;
					}
				}
				Pause();
				goto fight_boss_start;
				break;
			default:
				cout << "Please enter a number between 1 and " << count << ": ";
				break;
		}
	}
}
void move_results(Player* &player, Map& map, int position, string flavor) {
	int creature_chance = 35;
	if (position == map.get_boss() && map.get_boss_present()) {
		Dungeon instance(map.get_level());
		instance.move();
		fight_boss(player, map);
		player->update_stats();
	}
	else if (rand() % 101 < creature_chance) {
		fight_creature(player);
		player->update_stats();
	}
	else {
		cout << flavor << endl;
		Pause();
	}
	if (map.explored_map() && map.get_explore_bonus_earned() == false) {
		player->earn_explore_bonus(map.get_area());
		map.set_explore_bonus_earned();
	}
}
void wild_menu(Player* &player) {
	Map map(player->get_boss_quest().get_level()); //initialize the map based on level of quest boss
	int position = map.get_town();
	string north_flavor = "You move north; so far, the coast seems clear. Who's really scared of bunnies, anyway?";
	string west_flavor = "You move west; so far, nothing has eaten you. Flowers can't eat you, right?";
	string east_flavor = "You move east; so far, everything is fine. Grass just makes you itch, after all.";
	string south_flavor = "You move south; so far, all you see are bees buzzing around the flowers. Oh man, BEES!?";
	
	wild_menu_start:
	
	Clear();
	cout << "You have entered the wild! It's a large and scary meadow, "
	<< "filled with gross things and cute creatures.\n" << endl;
	Pause();
	
	wild_menu_after_move:
	//handles death situations (returns to town)
	if (player->is_dead()) {
		player->restore_health_to_max();
		if (player->get_player_class() == "Mage") {player->restore_mana_to_max();}
		return;
	}
	Clear();
	cout << "Map:" << endl;
	map.draw_map();
	
	cout << "What would you like to do?" << endl;
	cout << "(N) Move north" << endl;
	cout << "(W) Move west" << endl;
	cout << "(E) Move east" << endl;
	cout << "(S) Move south" << endl;
	cout << "1. Return to town" << endl;
	cout << "2. Player menu" << endl;
	
	char choice;
	while (true) {
		choice = GetChar();
		switch (choice) {
			case 'N':
			case 'n':
				if (map.can_move_north(position)) {
					position = map.move_player(position, 'N');
					if (position == map.get_town()) {
						map.enter_town(position);
						return;
					}
					move_results(player, map, position, north_flavor);
					goto wild_menu_after_move;
				}
				else {cout << "You can't move north. Please try another direction: ";}
				break;
			case 'W':
			case 'w':
				if (map.can_move_west(position)) {
					position = map.move_player(position, 'W');
					if (position == map.get_town()) {
						map.enter_town(position);
						return;
					}
					move_results(player, map, position, west_flavor);
					goto wild_menu_after_move;
				}
				else {cout << "You can't move west. Please try another direction: ";}
				break;
			case 'E':
			case 'e':
				if (map.can_move_east(position)) {
					position = map.move_player(position, 'E');
					if (position == map.get_town()) {
						map.enter_town(position);
						return;
					}
					move_results(player, map, position, east_flavor);
					goto wild_menu_after_move;
				}
				else {cout << "You can't move east. Please try another direction: ";}
				break;
			case 'S':
			case 's':
				if (map.can_move_south(position)) {
					position = map.move_player(position, 'S');
					if (position == map.get_town()) {
						map.enter_town(position);
						return;
					}
					move_results(player, map, position, south_flavor);
					goto wild_menu_after_move;
				}
				else {cout << "You can't move south. Please try another direction: ";}
				break;
			case '1':
				return;
				break;
			case '2':
				player_menu(player);
				goto wild_menu_start;
				break;
			default:
				cout << "Please enter N, W, E, S, 1, or 2: ";
				break;
		}
	}
}
void town_menu(Player* &player) {
	town_menu_start:
	Clear();
	Blacksmith town_blacksmith(player->get_level());
	Inn town_inn(player->get_level());
	
	cout << "You have entered town.\n" << endl;
	
	if (player->get_quest_active() && player->quest_complete()) {
		player->turn_in_quest();
		Pause();
		Clear();
		cout << "Now that that's done... ";
	}
	
	cout << "What would you like to do?\n" << endl;
	
	int count = 1;
	cout << count << ". Go to the blacksmith" << endl;
	count++;
	cout << count << ". Go to the inn" << endl;
	count++;
	cout << count << ". Leave the town (enter the wild)" << endl;
	count++;
	cout << count << ". Player menu" << endl;
	
	int choice;
	while (true) {
		choice = GetInt();
		switch (choice) {
			case 1:
				town_blacksmith.menu(player);
				goto town_menu_start;
				break;
			case 2:
				town_inn.menu(player);
				goto town_menu_start;
				break;
			case 3:
				player->quest_calculation_and_acceptance();
				wild_menu(player);
				goto town_menu_start;
				break;
			case 4:
				player_menu(player);
				goto town_menu_start;
				break;
			default:
				cout << "Please enter a number between 1 and " << count << ": ";
		}
	}
}

int main() {
	srand((unsigned int)time(0)); //just set once to ensure random generation
	
	cout << "Welcome to RPG Land!\n" << endl;
	Player *player = new Player;
	load_or_create_player_menu(player);
	
	town_menu(player);
	
	return 0;
}