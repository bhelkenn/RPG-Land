#include"Item.h"
using namespace std;

//INITIALIZATION
void Item::init_item() {
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
	if (type == "Armor") {stat_bonus = level_multiplier(6);}
	else if (type == "Weapon") {stat_bonus = level_multiplier(4);}
	else if (type == "Shield") {stat_bonus = level_multiplier(10);}
	else if (type == "Offhand") {stat_bonus = level_multiplier(3);}
	else if (type == "Potion") {stat_bonus = level_multiplier(15);}
	else {stat_bonus = 0;}
	
	//attribute bonus calculation
	attrib_bonus = level_multiplier(1);
	
	//update viewable statistics
	const int TYPE_MULTIPLIER = 2;
	const int ATK_BONUS = 2;
	if (type == "Armor") {
		if (subtype == "Mail") {
			str_bonus = attrib_bonus;
			def_bonus = stat_bonus * 3 / 2;
		}
		else if (subtype == "Leather") {
			agi_bonus = attrib_bonus;
			def_bonus = stat_bonus * 2 / 2;
		}
		else if (subtype == "Cloth") {
			int_bonus = attrib_bonus;
			def_bonus = stat_bonus * 1 / 2;
		}
		sta_bonus = attrib_bonus * TYPE_MULTIPLIER;
	}
	else if (type == "Shield") {
		def_bonus = stat_bonus;
		block_bonus = 5 * level;
		str_bonus = attrib_bonus;
		sta_bonus = attrib_bonus * TYPE_MULTIPLIER;
	}
	else if (type == "Weapon") {
		if (subtype == "Sword") {
			str_bonus = attrib_bonus * TYPE_MULTIPLIER;
			atk_bonus = stat_bonus * 3 / 2;
		}
		else if (subtype == "Dagger") {
			agi_bonus = attrib_bonus * TYPE_MULTIPLIER;
			atk_bonus = stat_bonus * 2 / 2;
			rogue_atk_bonus = atk_bonus * ATK_BONUS;
		}
		else if (subtype == "Staff") {
			mana_bonus = stat_bonus;
			int_bonus = attrib_bonus * TYPE_MULTIPLIER;
			atk_bonus = stat_bonus * 1 / 2;
		}
		sta_bonus = attrib_bonus;
	}
	else if (type == "Offhand") {
		mana_bonus = stat_bonus;
		int_bonus = attrib_bonus * TYPE_MULTIPLIER;
		sta_bonus = attrib_bonus;
	}
	
	//sets descriptions
	if (type == "Armor") {
		if (level >= 0) {
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
				else {description = "";}
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
				else {description = "";}
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
				else {description = "";}
			}
		}
		else {cout << "Not a valid level (init_item -> descriptions)" << endl;}
	}
	else if (type == "Shield") {
		if (level >= 0) {
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
			else {description = "";}
		}
		else {cout << "Not a valid level (init_item -> descriptions)" << endl;}
	}
	else if (type == "Weapon") {
		if (level >= 0) {
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
				else {description = "";}
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
				else {description = "";}
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
				else {description = "";}
			}
		}
		else {cout << "Not a valid level (init_item -> descriptions)" << endl;}
	}
	else if (type == "Offhand") {
		if (level >= 0) {
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
			else {description = "";}
		}
		else {cout << "Not a valid level (init_item -> descriptions)" << endl;}
	}
	else if (type == "Potion") {
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
			else {cout << "Not a valid level (init_item -> descriptions)" << endl;}
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
			else {cout << "Not a valid level (init_item -> descriptions)" << endl;}
		}
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
			else {cout << "Not a valid level (init_item -> descriptions)" << endl;}
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
			else {cout << "Not a valid level (init_item -> descriptions)" << endl;}
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
			else {cout << "Not a valid level (init_item -> descriptions)" << endl;}
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
			else {cout << "Not a valid level (init_item -> descriptions)" << endl;}
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
			else {cout << "Not a valid level (init_item -> descriptions)" << endl;}
		}
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
	else {cout << "Not a valid item type (init_item -> descriptions)" << endl;}
	
	//sell value is always half of the buy value...
	sell_value = buy_value / 2;
	
	//initializes as fully repaired
	damage_percentage = 0;
}
void Item::set_item(string item_type, string item_subtype, int item_level) {
	quantity = 1;
	type = item_type;
	subtype = item_subtype;
	level = item_level;
	init_item();
}

//BOOLS
bool Item::equipable() {
	if (type == "Armor" || 
		type == "Weapon" || 
		type == "Shield" || 
		type == "Offhand") {return true;}
	else {return false;}
}
bool Item::broken() {
	if (damage_percentage == 1.0) {return true;}
	else {return false;}
}

//VIEW FUNCTIONS
void Item::view_stats(string player_class) {
	Clear();
	cout << "Name: " << name << endl;
	cout << "Level: " << level << endl;
	
	if (type == "Armor") {
		cout << "Armor rating: " << def_bonus << endl;
		if (subtype == "Mail") {
			cout << "Strength: " << str_bonus << endl;
		}
		else if (subtype == "Leather") {
			cout << "Agility: " << agi_bonus << endl;
		}
		else if (subtype == "Cloth") {
			cout << "Intellect: " << int_bonus << endl;
		}
		cout << "Stamina: " << sta_bonus << endl;
	}
	else if (type == "Shield") {
		cout << "Armor rating: " << def_bonus << endl;
		cout << "Block rating: " << block_bonus << endl;
		cout << "Strength: " << str_bonus << endl;
		cout << "Stamina: " << sta_bonus << endl;
	}
	else if (type == "Weapon") {
		if (subtype == "Sword") {
			cout << "Attack rating: " << atk_bonus << endl;
			cout << "Strength: " << str_bonus << endl;
		}
		else if (subtype == "Dagger") {
			cout << "Attack rating: ";
			if (player_class == "Rogue") {cout << rogue_atk_bonus << endl;}
			else {cout << atk_bonus << endl;}
			cout << "Agility: " << agi_bonus << endl;
		}
		else if (subtype == "Staff") {
			cout << "Attack rating: " << atk_bonus << endl;
			cout << "Intellect: " << int_bonus << endl;
			cout << "Mana bonus: " << mana_bonus << endl;
		}
		cout << "Stamina: " << sta_bonus << endl;
	}
	else if (type == "Offhand") {
		cout << "Intellect: " << int_bonus << endl;
		cout << "Mana bonus: " << mana_bonus << endl;
		cout << "Stamina: " << sta_bonus << endl;
	}
	else if (type == "Potion") {
		cout << subtype << " yield: " << stat_bonus << endl;
	}
	cout << endl;
	
	cout << description << endl;
	Pause();
}

//damage and repair
void Item::damage(int x) {
	damage_percentage += (x / 100.0);
	if (damage_percentage > 1.0) {damage_percentage = 1.0;}
}
void Item::repair(int x) {
	damage_percentage -= (x / 100.0);
	if (damage_percentage < 0) {damage_percentage = 0;}
}