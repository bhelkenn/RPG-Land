import java.util.Scanner;
import java.io.File;

class Global {
	public static void Pause() {
		System.out.println("Press ENTER to continue...");
		new Scanner(System.in).nextLine();
	}
	public static void Clear() {
		for (int i = 0; i < 50; i++) {System.out.println();}
	}
}

class Item {
	public Item() {}
	public Item(String item_type, String item_subtype, int item_level) {
		quantity = 1;
		type = item_type;
		subtype = item_subtype;
		level = item_level;
		init_item();
	}
	
	public void inc_quantity() {quantity++;}
	public void dec_quantity() {quantity--;}
	
	//GET FUNCTIONS
	public String get_name() {return name;}
	public String get_type() {return type;}
	public String get_subtype() {return subtype;}
	public int get_sell_value() {return sell_value;}
	public int get_buy_value() {return buy_value;}
	public int get_level() {return level;}
	public int get_quantity() {return quantity;}
	
	//LOAD FUNCTIONS
	public void load_name(String s) {name = s;}
	public void load_type(String s) {type = s;}
	public void load_subtype(String s) {subtype = s;}
	public void load_sell_value(int x) {sell_value = x;}
	public void load_buy_value(int x) {buy_value = x;}
	public void load_level(int x) {level = x;}
	public void load_quantity(int x) {quantity = x;}
	
	public boolean equipable() {
		if (type == "Armor") {return true;}
		else if (type == "Weapon") {return true;}
		else if (type == "Shield") {return true;}
		else {return false;}
	}
	
	//returns value of bonus given by an item
	public int item_stat_value() {
		if (subtype == "Sword") {return level_multiplier(5);}
		else if (subtype == "Dagger") {return level_multiplier(3);}
		else if (subtype == "Staff") {return level_multiplier(2);}
		else if (subtype == "Cloth") {return level_multiplier(3);}
		else if (subtype == "Leather") {return level_multiplier(5);}
		else if (subtype == "Mail") {return level_multiplier(8);}
		else if (type == "Shield") {return level_multiplier(15);}
		else {return 0;}
	}
	
	//VARIABLES
	private String type, subtype, name;
	private int level, quantity, buy_value, sell_value;
	
	private void init_item() {
		//known: type, subtype, level
		//assumed quantity of 1
		
		//known types: "Armor", "Weapon", "Shield", "Potion", "Room", "Trash"
		//known subtypes: "Mail", "Leather", "Cloth", "Sword", "Dagger", "Staff", "Health",
		//"Mana", "none", "broken tooth", "hairball", "string", "tail fluff", "lint"
		//known levels: 1, 2, 3, 4, 5
		//conditions to init name and buy/sell value based on knowns
		
		//name assembly
		if (type == "Armor") {
			if (level != 0) { /* validates as a "real" item */
				String s = new String(); /*used to assemble the name*/
			
				//assemble prefix
				if (level == 1) {s = "Crappy ";}
				else if (level == 2) {s = "Cheap ";}
				else if (level == 3) {s = "Basic ";}
				else if (level == 4) {s = "Expensive ";}
				else if (level == 5) {s = "Luxurious ";}
				else {System.out.println("Not a valid level");}
			
				//add subtype to prefix
				if (subtype == "Mail") {s += subtype + " ";}
				else if (subtype == "Leather") {s += subtype + " ";}
				else if (subtype == "Cloth") {s += subtype + " ";}
				else {System.out.println("Not a valid subtype (Armor -> name)");}
			
				//add type to the end
				s += type;
			
				//this is the name of the item
				name = s;
			}
			//for unequipped slots
			else {name = "empty";}
		}
		else if (type == "Weapon") {
			if (level != 0) { /* validates as a "real" item */
				String s = new String(); /*used to assemble the name*/
			
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
					else {System.out.println("Not a valid level");}
					s += subtype;
				}
				else if (subtype == "Staff") {
					//assemble prefix based on subtype
					if (level == 1) {s = "Foam ";}
					else if (level == 2) {s = "Rubber ";}
					else if (level == 3) {s = "Wooden ";}
					else if (level == 4) {s = "Metal ";}
					else if (level == 5) {s = "Mage's ";}
					else {System.out.println("Not a valid level");}
					s += subtype;
				}
				else {System.out.println("Not a valid subtype (Weapon -> name)");}
			
				//since there is no need to specify these as weapons....
				//this is the name of the item
				name = s;
			}
			//for unequipped slots
			else {name = "empty";}
		}
		else if (type == "Shield") {
			if (level != 0) { /* validates as a "real" item */
				String s = new String(); /*used to assemble the name*/
				
				//assemble prefix
				if (level == 1) {s = "Plastic ";}
				else if (level == 2) {s = "Wooden ";}
				else if (level == 3) {s = "Iron ";}
				else if (level == 4) {s = "Steel ";}
				else if (level == 5) {s = "Jade ";}
				else {System.out.println("Not a valid level");}
			
				//add type
				s += type;
			
				//this is the name
				name = s;
			}
			//for unequipped slots
			else {name = "empty";}
		}
		else if (type == "Potion") {
			String s = new String(); /*used to assemble the name*/
			
			//assemble prefix
			if (level == 1) {s = "Crappy ";}
			else if (level == 2) {s = "Cheap ";}
			else if (level == 3) {s = "Basic ";}
			else if (level == 4) {s = "Expensive ";}
			else if (level == 5) {s = "Luxurious ";}
			else {System.out.println("Not a valid level");}
			
			//add subtype to prefix
			if (subtype == "Health") {s += subtype + " ";}
			else if (subtype == "Mana") {s += subtype + " ";}
			else {System.out.println("Not a valid subtype (Potion -> name)");}
			
			//add type to the end
			s += type;
			
			//this is the name
			name = s;
		}
		else if (type == "Room") {
			String s = new String(); /*used to assemble the name*/
			
			//assemble prefix
			if (level == 1) {s = "Crappy ";}
			else if (level == 2) {s = "Cheap ";}
			else if (level == 3) {s = "Basic ";}
			else if (level == 4) {s = "Expensive ";}
			else if (level == 5) {s = "Luxurious ";}
			else {System.out.println("Not a valid level");}
			
			//add type
			s += type;
			
			//this is the name
			name = s;
		}
		else if (type == "Trash") {
			String s = new String(); /*used to assemble the name*/
			
			//assemble prefix
			if (level == 1) {s = "Slimy ";}
			else if (level == 2) {s = "Pungent ";}
			else if (level == 3) {s = "Greasy ";}
			else if (level == 4) {s = "Rotten ";}
			else if (level == 5) {s = "Putrid ";}
			else {System.out.println("Not a valid level");}
			
			//add subtype
			s += subtype;
			
			//this is the name
			name = s;
		}
		else {System.out.println("Not a valid item type (name)");}
		
		//buy value assembly
		if (type == "Armor") {
			//further classify by subtype
			if (subtype == "Mail") {buy_value = 45 * level;}
			else if (subtype == "Leather") {buy_value = 30 * level;}
			else if (subtype == "Cloth") {buy_value = 15 * level;}
			else {System.out.println("Not a valid subtype (Armor -> buy value)");}
		}
		else if (type == "Weapon") {
			//further classify by subtype
			if (subtype == "Sword") {buy_value = 100 * level;}
			else if (subtype == "Dagger") {buy_value = 50 * level;}
			else if (subtype == "Staff") {buy_value = 75 * level;}
			else {System.out.println("Not a valid subtype (Weapon -> buy value)");}
		}
		else if (type == "Shield") {
			//no subtypes to filter by
			buy_value = 50 * level;
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
			else {System.out.println("Not a valid subtype (Trash -> buy value)");}
		}
		else {System.out.println("Not a valid item type (buy value)");}
		
		//sell value is always half of the buy value...
		sell_value = buy_value / 2;
	}
	
	private int level_multiplier(int x) {return (int)(Math.pow(x * 2, level));}
}

class Spell {
	//constructors, one empty for bulk creation, and the other one to assign case-by-case
	public Spell() {}
	public Spell(String spell_name, String spell_type, int mag, int player_level) {
		name = spell_name;
		type = spell_type;
		level = player_level;
		base_mag = mag;
		cost = magnitude * 2 * level;
	}
	
	public void inc_level() {level++;}
	public void inc_magnitude(int x) {
		magnitude = base_mag + x;
		cost = magnitude * 2 * level;
	}
	
	//GET FUNCTIONS
	public int get_magnitude() {return magnitude;}
	public int get_cost() {return cost;}
	public String get_name() {return name;}
	public String get_type() {return type;}
	
	//LOAD FUNCTIONS
	public void load_magnitude(int x) {magnitude = x;}
	
	//VARIABLES
	private String name, type;
	private int base_mag, magnitude, cost, level;
}

class Shop {
	//top level menu
	public Player menu(Player player) {
		Global.Clear();
		System.out.println(entrance_flavor);
		start:
		while (true) {
			System.out.println(menu_flavor);
			System.out.println("\n1. Buy / Browse goods");
			System.out.println("2. Sell");
			System.out.println("3. Leave shop");
	
			char choice = new Scanner(System.in).next().charAt(0);
			switch (choice) {
				case '1':
				case 'B':
				case 'b':
					player = buy(player);
					Global.Clear();
					continue start;
				case '2':
				case 'S':
				case 's':
					player = sell(player);
					Global.Clear();
					continue start;
				case '3':
				case 'L':
				case 'l':
					System.out.println(leave_flavor);
					Global.Pause();
					return player;
				default:
					System.out.print("Invalid entry. ");
					choice = new Scanner(System.in).next().charAt(0);
			}
		}
	}

	//VARIABLES
	protected Item[] inventory;
	protected int level;
	protected String display_inventory_flavor, sell_flavor;
	protected String entrance_flavor, leave_flavor, menu_flavor;
	
	//shop inventory
	protected void display_inventory() {
		Global.Clear();
		System.out.println(display_inventory_flavor + "\n");
		for (int i = 0; i < inventory.length; i++) {
			System.out.print((i + 1) + ". " + inventory[i].get_name() + " - ");
			System.out.println(inventory[i].get_buy_value() + " gold");
		}
	}

	//buying menu
	protected Player buy(Player player) {
		start:
		while (true) {
			display_inventory();
			System.out.println((inventory.length + 1) + ". Exit menu\n");		
			player.view_gold();
		
			int choice = new Scanner(System.in).nextInt();
			choice--;
			if (choice < inventory.length) {
				if (player.get_gold() >= inventory[choice].get_buy_value()) {
					player.inc_inventory(inventory[choice]);
					player.remove_gold(inventory[choice].get_buy_value());
					System.out.print("You've purchased " + inventory[choice].get_name());
					System.out.println(" for " + inventory[choice].get_buy_value() + " gold.");
				}
				else {System.out.println("You don't have enough gold.");}
				Global.Pause();
				continue start;
			}
			else if (choice == inventory.length) {return player;}
			else {
				System.out.print("Invalid entry. ");
				choice = new Scanner(System.in).nextInt();
				choice--;
			}
		}
	}

	//selling menu
	protected Player sell(Player player) {
		start:
		while (true) {
			Global.Clear();
			if (player.get_inventory().length > 0) {
				Item inv[] = player.get_inventory();
				System.out.println(sell_flavor + "\n");
				for (int i = 0; i < inv.length; i++) {
					System.out.print((i + 1) + ". " + inv[i].get_name() + " x" + inv[i].get_quantity());
					System.out.println(" - " + inv[i].get_sell_value() + " gold");
				}
				System.out.println((inv.length + 1) + ". Exit menu\n");
				
				int choice = new Scanner(System.in).nextInt();
				choice--;
				if (choice < inv.length) {
					System.out.print("You would like to sell " + inv[choice].get_name() + " for ");
					System.out.print(inv[choice].get_sell_value() + " gold. Is that acceptable (Y/N)?");
					char decision = new Scanner(System.in).next().charAt(0);
					while (true) {
						switch (decision) {
							case 'Y':
							case 'y':
								player.dec_inventory(inv[choice]);
								player.add_gold(inv[choice].get_sell_value());
								System.out.print("You have sold " + inv[choice].get_name());
								System.out.println(" for " + inv[choice].get_sell_value() + " gold. \n");
								Global.Pause();
								continue start;
							case 'N':
							case 'n':
								continue start;
							default:
								System.out.print("Please enter Y or N: ");
								decision = new Scanner(System.in).next().charAt(0);
						}
					}
				}
				else if (choice == inv.length) {return player;}
				else {
					System.out.print("Invalid entry");
					choice = new Scanner(System.in).nextInt();
				}
			}
			else {
				System.out.println("You don't have any items to sell.");
				Global.Pause();
				return player;
			}
		}
	}
}

class Blacksmith extends Shop {
	public Blacksmith(int shop_level) {
		level = shop_level;
		init_inventory();
		display_inventory_flavor = "Blacksmith: 'Here's what I have to offer:'";
		sell_flavor = "Blacksmith: 'What would you like to sell to me?' ";
		entrance_flavor = "You enter the blacksmith. The proprietor notices you enter.";
		menu_flavor = "Blacksmith: 'Welcome to my shop! What would you like?'";
		leave_flavor = "You decide to leave the blacksmith's shop. ";
		leave_flavor += "The proprietor scowls at your retreating back.";
	}
	
	//initializes inventory
	private void init_inventory() {
		inventory = new Item[7];
		inventory[0] = new Item("Weapon", "Sword", level);
		inventory[1] = new Item("Weapon", "Dagger", level);
		inventory[2] = new Item("Weapon", "Staff", level);
		inventory[3] = new Item("Armor", "Cloth", level);
		inventory[4] = new Item("Armor", "Leather", level);
		inventory[5] = new Item("Armor", "Mail", level);
		inventory[6] = new Item("Shield", "none", level);
	}
};

class Inn extends Shop {
	public Inn(int shop_level) {
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
	
	private Item room;
	
	//initializes inventory
	private void init_inventory() {
		inventory = new Item[2];
		inventory[0] = new Item("Potion", "Health", level);
		inventory[1] = new Item("Potion", "Mana", level);
	}
	
	//establishes room for rent
	private void init_room() {room = new Item("Room", "none", level);}
	
	//room menu
	private Player stay(Player player) {
		Global.Clear();
		System.out.print("Innkeeper: 'Our " + room.get_name() + " is available to rent for " + room.get_buy_value());
		System.out.println(" gold. We also offer 'specials' for some of our more...distinguished guests.'\n");
		player.view_gold();
		System.out.print("\nWould you like to stay the night (Y/N)? ");
		
		char choice = new Scanner(System.in).next().charAt(0);
		while (true) {
			if (choice == 'Y' || choice == 'y') {
				if (player.get_gold() >= room.get_buy_value()) {
					player.remove_gold(room.get_buy_value());
					player.restore_health_to_max();
					player.restore_mana_to_max();
					Global.Clear();
					System.out.print("You've stayed in the " + room.get_name() + ", and your health and mana have ");
					System.out.print("recovered fully. Anything else, such as certain people sneaking into your room ");
					System.out.println("in the middle of the night, you'd rather not think about too much.");
					Global.Pause();
					return player;
				}
				else {
					System.out.println("You don't have enough gold.");
					Global.Pause();
					return player;
				}
			}
			else if (choice == 'N' || choice == 'n') {
				System.out.print("You decided not to stay the night. Perhaps it had something to do with how ");
				System.out.println("the innkeeper has been staring at you this entire time....");
				Global.Pause();
				return player;
			}
			else {
				System.out.print("Please enter Y or N: ");
				choice = new Scanner(System.in).next().charAt(0);
			}
		}
	}
}

class Creature {
	public Creature() {}
	public Creature(String cr_type, int cr_level) {
		level = cr_level;
		type = cr_type;
		stat_pool = 60 * level;
		set_type();
		money = exp * 2;
		has_loot = generate_loot();
	}
	
	public void take_damage(int attack, int acc, int critical) {
		if ((int)(Math.random() * 101) < dodge) {
			System.out.println(name + " dodged your attack!");
			return;
		}
		if ((int)(Math.random() * 101) > acc) {
			System.out.println("Your attack missed!");
			return;
		}
		else {
			int x;
			if (attack < def && attack > 0) {x = 1;}
			else if (attack == 0) {x = 0;}
			else {
				//establish base damage
				double dmg = 10 * (Math.pow(1.25, attack / (def * 1.0)) - 1) * level;
				//check for critical strikes
				if ((int)(Math.random() * 101) < critical) {
					System.out.println("You landed a critical hit!");
					dmg *= 2;
				}
				x = (int)dmg;
			}
			
			if (x > 0) {
				System.out.println(name + " takes " + x + " damage!");
				health -= x;
			}
			else {System.out.println(name + " took no damage.");}
		}
		
	}
	
	public void take_spell_dmg(int magnitude, int acc, int critical) {
		if ((int)(Math.random() * 101) < dodge) {
			System.out.println(name + " dodged your attack!");
			return;
		}
		if ((int)(Math.random() * 101) < parry) {
			System.out.println(name + " parried your attack!");
			return;
		}
		if ((int)(Math.random() * 101) > acc) {
			System.out.println("Your attack missed!");
			return;
		}
		else if ((int)(Math.random() * 101) <= acc) {
			//establish base damage
			int dmg = magnitude;
			//check for critical hits
			if ((int)(Math.random() * 101) < critical) {
				System.out.println("Your spell landed a critical hit!");
				dmg *= 2;
			}
			System.out.println(name + " takes " + dmg + " damage!");
			health -= magnitude;
		}
	}
	
	public boolean is_dead() {
		if (health <= 0) {return true;}
		else {return false;}
	}
	
	//GET FUNCTIONS
	public String get_name() {return name;}
	public int get_exp() {return exp;}
	public int get_atk() {return atk;}
	public int get_def() {return def;}
	public int get_health() {return health;}
	public int get_accuracy() {return creature_acc;}
	public int get_crit() {return crit;}
	public int get_money() {return money;}
	public Item get_loot() {
		System.out.println("You've looted " + loot.get_name() + " from the " + name + "!");
		return loot;
	}
	public boolean get_has_loot() {return has_loot;}
	
	//VARIABLES
	private String name, type;
	private int exp, atk, def, health, stat_pool, level, creature_acc, dodge, parry, crit, money;
	private Item loot;
	private boolean has_loot;
	
	private void set_type() {
		String s = new String();
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
			atk = stat_pool * 2 / 3;
			def = stat_pool / 3;
			health = stat_pool / 3;
			creature_acc = 95;
			dodge = 2;
			parry = 2;
			crit = 10;
		}
		else if (type == "Defensive") {
			s += "midget";
			name = s;
			exp = 15 * level;
			atk = stat_pool / 3;
			def = stat_pool * 2 / 3;
			health = stat_pool / 1;
			health *= 2;
			creature_acc = 85;
			dodge = 10;
			parry = 10;
			crit = 2;
		}
		else if (type == "Balanced") {
			s += "bunny";
			name = s;
			exp = 5 * level;
			atk = stat_pool / 2;
			def = stat_pool / 2;
			health = stat_pool / 2;
			creature_acc = 90;
			dodge = 5;
			parry = 5;
			crit = 5;
		}
		else {System.out.println("Not a valid type (Creature -> set_type)");}
	}
	
	private boolean generate_loot() {
		int num = 5;
		Item[] items = new Item[num];
		
		String[] subtypes = new String[num];
		subtypes[0] = "broken tooth";
		subtypes[1] = "hairball";
		subtypes[2] = "string";
		subtypes[3] = "tail fluff";
		subtypes[4] = "lint";
		
		String type = "Trash";
		
		for (int i = 0; i < num; i++) {items[i] = new Item(type, subtypes[i], level);}
		
		int x = (int)(Math.random() * 101);
		if (x <= 100 && x > 95) {loot = items[4];}
		else if (x <= 95 && x > 85) {loot = items[3];}
		else if (x <= 85 && x > 70) {loot = items[2];}
		else if (x <= 70 && x > 50) {loot = items[1];}
		else if (x <= 50 && x > 25) {loot = items[0];}
		else {return false;}
		return true;
	}
}

class Player {
	public Player() {}
	
	//initialize player for the first time
	public void construct() {
		init_attribs();
		set_name();
		class_menu();
		if (player_class == "Mage") {init_spellbook();}
		init_stats();
		inventory = new Item[0];
		equipment = new Item[3];
		equipment[0] = new Item("Armor", "Cloth", 0);
		equipment[1] = new Item("Weapon", "Staff", 0);
		equipment[2] = new Item("Shield", "none", 0);
		allocate_points();
		level_up();
		current_health = max_health;
		current_mana = max_mana;
	}
	
	//determine player's name
	public void set_name() {
		System.out.print("What is your name? ");
		name = new Scanner(System.in).nextLine();
		System.out.println("Hi, " + name + "!");
		Global.Pause();
		Global.Clear();
	}
	
	//determines the class the player would like to play as
	public void class_menu() {
		char choice;
		
		//introduction to the classes
		Global.Clear();
		System.out.print("In RPG Land, there are three classes you can play. These three ");
		System.out.print("classes are commonly known as Warriors, Rogues, and Mages.");
		System.out.println();
		Global.Pause();
		
		boolean loop = true;
		start:
		while (loop) {
			Global.Clear();
			//print out the options to choose from
			System.out.println("Please choose a class to learn more about:");
			System.out.println("\n1. Warrior");
			System.out.println("2. Rogue");
			System.out.println("3. Mage");
			choice = new Scanner(System.in).next().charAt(0);
			switch (choice) {
				//warrior choice
				case '1':
					warrior_description();
					System.out.print("Would you like to choose the Warrior (Y/N)? ");
					if (set_class("Warrior")) {
						warrior_bonus();
						System.out.println("\nYou've chosen the " + player_class + " class! Congratulations!");
						System.out.println();
						loop = false;
					}
					else {continue start;}
					break;
				//rogue choice
				case '2':
					rogue_description();
					System.out.print("Would you like to choose the Rogue (Y/N)? ");
					if (set_class("Rogue")) {
						rogue_bonus();
						System.out.println("\nYou've chosen the " + player_class + " class! Congratulations!");
						System.out.println();
						loop = false;
					}
					else {continue start;}
					break;
				//mage choice
				case '3':
					mage_description();
					System.out.print("Would you like to choose the Mage (Y/N)? ");
					if (set_class("Mage")) {
						mage_bonus();
						System.out.println("\nYou've chosen the " + player_class + " class! Congratulations!");
						System.out.println();
						loop = false;
					}
					else {continue start;}
					break;
				default:
					System.out.println("Invalid entry.");
					choice = new Scanner(System.in).next().charAt(0);
			}
		}
	}
	
	//allocates points as desired
	public void allocate_points() {
		char choice;
		start:
		while (points > 0) {
			//overview
			view_attributes();
		
			System.out.print("\nYou have " + points + " points remaining. ");
			while (true) {
				System.out.println("Please select an attribute to add to:");
			
				//list of available attributes to add to
				System.out.println("1. Strength");
				System.out.println("2. Agility");
				System.out.println("3. Intellect");
				System.out.println("4. Stamina");
				choice = new Scanner(System.in).next().charAt(0);
				switch (choice) {
					case '1':
						base_str++;
						points--;
						init_stats();
						continue start;
					case '2':
						base_agi++;
						points--;
						init_stats();
						continue start;
					case '3':
						base_int++;
						points--;
						init_stats();
						continue start;
					case '4':
						base_sta++;
						points--;
						init_stats();
						continue start;
					default:
						System.out.print("Invalid entry. ");
						choice = new Scanner(System.in).next().charAt(0);
				}
			}
		}
		view_attributes();
		System.out.println("\nAll available points have been allocated.");
		Global.Pause();
	}
	
	//establishes ATK, DEF, max_health, and mana levels, and takes bonuses into consideration
	public void init_stats() {
		//update attributes
		strength = base_str + equip_atk_bonus / ATTRIB_BONUS;
		if (player_class == "Rogue") {agility = base_agi + equip_atk_bonus / ATTRIB_BONUS;}
		else {agility = base_agi + equip_def_bonus / ATTRIB_BONUS;}
		intellect = base_int + equip_mana_bonus / ATTRIB_BONUS;
		stamina = base_sta + equip_health_bonus / ATTRIB_BONUS;
		
		//sets ATK
		if (player_class == "Warrior") {atk = strength * 2 + agility + equip_atk_bonus;}
		else if (player_class == "Rogue") {atk = agility * 2 + strength + equip_atk_bonus;}
		else {atk = strength + agility + equip_atk_bonus;}
		
		//sets DEF
		if (player_class == "Rogue") {def = agility + armor_rating;}
		else {def = agility * 2 + armor_rating;}
		
		//sets max_health
		if (player_class == "Warrior") {max_health = 10 + stamina * 2 + equip_health_bonus;}
		else {max_health = 10 + stamina + equip_health_bonus;}
		
		//sets max_mana
		if (player_class == "Mage") {max_mana = intellect * 5 + equip_mana_bonus;}
		else {max_mana = 0;}
		
		//sets crit chance
		crit = 5;
		
		//sets dodge, parry, and block chances
		dodge = 5;
		if (player_class == "Rogue") {dodge = 10;}
		parry = 5;
		
		//sets accuracy
		acc = 90;
		
		//updates spellpower
		if (player_class == "Mage") {
			spellpower = intellect / ATTRIB_BONUS;
			for (int i = 0; i < spellbook.length; i++) {
				spellbook[i].inc_magnitude(spellpower);
			}
		}
	}

	//initializes spellbook
	public void init_spellbook() {
		int num = 3;
		String[] names = new String[num];
		names[0] = "Fireball";
		names[1] = "Salve";
		names[2] = "Steelskin";
		String[] types = new String[num];
		types[0] = "Offensive";
		types[1] = "Healing";
		types[2] = "Defensive";
		int[] magnitudes = new int[num];
		magnitudes[0] = 25;
		magnitudes[1] = 40;
		magnitudes[2] = 50;
		
		spellbook = new Spell[num];
		for (int i = 0; i < spellbook.length; i++) {
			spellbook[i] = new Spell(names[i], types[i], magnitudes[i], level);
		}
	}
	
	//HEALTH-MANA MANIPULATION
	public void restore_health_to_max() {current_health = max_health;}
	public void restore_mana_to_max() {current_mana = max_mana;}
	public void restore_health(int x) {
		current_health += x;
		if (current_health > max_health) {restore_health_to_max();}
	}
	public void restore_mana(int x) {
		current_mana += x;
		if (current_mana > max_mana) {restore_mana_to_max();}
	}
	public void lose_health(int x) {current_health -= x;}
	public void lose_mana(int x) {current_mana -= x;}
	
	//GOLD-EXP MANIPULATION
	public void remove_gold(int x) {gold -= x;}
	public void add_gold(int x) {gold += x;}
	public void gain_exp(int cr_exp) {
		exp += cr_exp;
		System.out.println("You've gained " + cr_exp + " experience!");
		level_up();
	}
	
	//INVENTORY MANIPULATION
	public void inc_inventory(Item item) {
		boolean found = false;
		for (int i = 0; i < inventory.length; i++) {
			if (inventory[i].get_name() == item.get_name()) {
				inventory[i].inc_quantity();
				found = true;
			}
		}
		if (found == false) {add_to_inventory(item);}
	}
	public void dec_inventory(Item item) {
		for (int i = 0; i < inventory.length; i++) {
			if (inventory[i].get_name() == item.get_name()) {
				inventory[i].dec_quantity();
				if (inventory[i].get_quantity() == 0) {
					remove_from_inventory(inventory[i]);
				}
			}
		}
	}
	
	//COMBAT
	public void boost_def(int x) {def += x;}
	public Creature cast_spell(Creature creature) {
		Global.Clear();
		System.out.println("What spell would you like to cast?");
		view_spells();
		System.out.println((spellbook.length + 1) + ". Cancel request (go to the previous menu)");
		
		int choice = new Scanner(System.in).nextInt();
		choice--;
		
		while (true) {
			if (choice < spellbook.length) {
				System.out.println("You cast " + spellbook[choice].get_name() + "!");
				if (spellbook[choice].get_type() == "Offensive") {
					creature.take_spell_dmg(spellbook[choice].get_magnitude(), acc, crit);
				}
				else if (spellbook[choice].get_type() == "Healing") {
					restore_health(spellbook[choice].get_magnitude());
					System.out.println("You've restored " + spellbook[choice].get_magnitude() + " health!");
					Global.Pause();
				}
				else if (spellbook[choice].get_type() == "Defensive") {
					boost_def(spellbook[choice].get_magnitude());
					System.out.println("You've boosted your DEF by " + spellbook[choice].get_magnitude() + "!");
					Global.Pause();
				}
				lose_mana(spellbook[choice].get_cost());
				did_cast_spell = true;
				return creature;
			}
			else if (choice == spellbook.length) {
				did_cast_spell = false;
				return creature;
			}
			else {
				System.out.print("Invalid entry.");
				choice = new Scanner(System.in).nextInt();
			}
		}
	}
	public void take_damage(int attack, int accuracy, int critical) {
		if ((int)(Math.random() * 101) < block) {
			System.out.println("You block the creature's attack!");
			return;
		}
		if ((int)(Math.random() * 101) < dodge) {
			System.out.println("You dodge the creature's attack!");
			return;
		}
		if ((int)(Math.random() * 101) < parry) {
			System.out.println("You parry the creature's attack!");
			return;
		}
		if ((int)(Math.random() * 101) > accuracy) {
			System.out.println("The creature missed you!");
			return;
		}
		else {
			int x;
			if (attack < def && attack > 0) {x = 1;}
			else if (attack == 0) {x = 0;}
			else {
				//establish base damage
				double dmg = 10 * (Math.pow(1.25, attack / (def * 1.0)) - 1) * level;
				//check for crits
				if ((int)(Math.random() * 101) < critical) {
					System.out.println("The creature landed a critical hit!");
					dmg *= 2;
				}
				x = (int)dmg;
			}
			if (x > 0) {
				System.out.println("You take " + x + " damage!");
				lose_health(x);
			}
			else {System.out.println("You didn't take any damage.");}
		}
	}
	public void dies() {
		System.out.println("You've died!");
		int exp_loss = LEVEL_INCREMENT;
		exp -= exp_loss;
		System.out.println("You've lost " + exp_loss + " experience!");
		Global.Pause();
		if (exp < LEVEL_INCREMENT * (level - 1)) {exp = LEVEL_INCREMENT * (level - 1);}
	}
	
	public void cast_healing_spell() {
		if (player_class == "Mage") {
			for (int i = 0; i < spellbook.length; i++) {
				if (spellbook[i].get_type() == "Healing") {
					Global.Clear();
					System.out.print("Would you like to cast " + spellbook[i].get_name() + " (Y/N)? ");
					char choice = new Scanner(System.in).next().charAt(0);
					
					while (true) {
						if (choice == 'Y' || choice == 'y') {
							System.out.println("You cast " + spellbook[i].get_name() + "!");
							restore_health(spellbook[i].get_magnitude());
							System.out.println("You've restored " + spellbook[i].get_magnitude() + " health!");
							Global.Pause();
							lose_mana(spellbook[i].get_cost());
							return;
						}
						else if (choice == 'N' || choice == 'n') {return;}
						else {
							System.out.print("Invalid entry.");
							choice = new Scanner(System.in).next().charAt(0);
						}
					}
				}
			}
		}
		else {
			System.out.println("You don't have any spells to cast. You're not a Mage, " + name + "!");
			Global.Pause();
		}
	}
	
	//level-up logic
	public void level_up() {
		int max_level = 5;
		exp_to_next_level = LEVEL_INCREMENT * level * level;
		if (exp >= exp_to_next_level && level < max_level) {
			System.out.print("You've leveled up! ");
			level++;
			exp_to_next_level = LEVEL_INCREMENT * level * level;
			for (int i = 0; i < spellbook.length; i++) {
				spellbook[i].inc_level();
			}
			System.out.println("You are now Level " + level + "!");
			Global.Pause();
			points += level - 1;
			allocate_points();
		}
	}
	
	//VIEW FUNCTIONS
	public void view_attributes() {
		Global.Clear();
		System.out.println("Your current attributes are:");
		System.out.println("Strength: " + strength);
		System.out.println("Agility: " + agility);
		System.out.println("Intellect: " + intellect);
		System.out.println("Stamina: " + stamina);
	}
	public void view_stats() {
		Global.Clear();
		System.out.println("Your current stats are:");
		System.out.println("ATK: " + atk);
		System.out.println("DEF: " + def);
		System.out.println("Health: " + current_health + "/" + max_health);
		if (player_class == "Mage") {
			System.out.println("Mana: " + current_mana + "/" + max_mana);
			System.out.println("Spellpower: " + spellpower);
		}
		System.out.println("\nExperience: " + exp + "/" + exp_to_next_level);
	}
	public void view_inventory() {
		Global.Clear();
		if (inventory.length == 0) {System.out.println("You have no items in your inventory.");}
		else {
			System.out.println("You have the following items in your inventory:");
			for (int i = 0; i < inventory.length; i++) {
				System.out.print(inventory[i].get_name() + " x" + inventory[i].get_quantity());
				if (inventory[i].equipable()) {System.out.print(" - can be equipped");}
				else {System.out.println();}
			}
		}
	}
	public void view_equipment() {
		Global.Clear();
		System.out.println("You have the following equipped:");
		for (int i = 0; i < 3; i++) {
			System.out.println(equipment[i].get_type() + " slot: " + equipment[i].get_name());
		}
	}
	public Item[] view_equipable_items() {
		int count = 0;
		Item inv[];
		for (int i = 0; i < inventory.length; i++) {
			if (inventory[i].equipable()) {count++;}
		}
		inv = new Item[count];
		count = 0;
		for (int i = 0; i < inventory.length; i++) {
			if (inventory[i].equipable()) {
				inv[count] = inventory[i];
				count++;
			}
		}
		System.out.println("You have the following equipable items:");
		for (int i = 0; i < inv.length; i++) {
			System.out.println((i + 1) + ". " + inv[i].get_name() + " x" + inv[i].get_quantity());
		}
		return inv;
	}
	public void view_gold() {System.out.println("You currently have " + gold + " gold.");}
	public void view_spells() {
		for (int i = 0; i < spellbook.length; i++) {
			System.out.println((i + 1) + ". " + spellbook[i].get_name());
		}
	}
	
	//BOOLS
	public boolean has_inventory() {
		if (inventory.length > 0) {return true;}
		else {return false;}
	}
	public boolean use_item() {
		Global.Clear();
		int count = 0;
		
		//establish number of potions in inventory
		for (int i = 0; i < inventory.length; i++) {
			if (inventory[i].get_type() == "Potion") {count++;}
		}
		
		//what if you have no items?
		if (count == 0) {
			System.out.println("You don't have any useable items!");
			Global.Pause();
			return false;
		}
		
		//set temporary array with correct size and fill it
		Item potions[] = new Item[count];
		count = 0;
		for (int i = 0; i < inventory.length; i++) {
			if (inventory[i].get_type() == "Potion") {
				potions[count] = inventory[i];
				count++;
			}
		}
		
		//choose a potion to use
		System.out.println("What potion would you like to use:\n");
		for (int i = 0; i < potions.length; i++) {
			System.out.println((i + 1) + ". " + potions[i].get_name() + " x" + potions[i].get_quantity());
		}
		//adds an option to cancel
		System.out.println((potions.length + 1) + ". Cancel request (go back to the previous screen)");
		
		int choice = new Scanner(System.in).nextInt();
		
		while (true) {
			//decrement by one for index
			choice--;
			if (choice < potions.length) {
				//determine change to health / mana
				if (potions[choice].get_subtype() == "Health") {
					int gain = 20 * potions[choice].get_level();
					restore_health(gain);
					System.out.println("You've used a " + potions[choice].get_name() + " and restored " + gain + " health!");
				}
				else if (potions[choice].get_subtype() == "Mana") {
					int gain = 20 * potions[choice].get_level();
					restore_mana(gain);
					System.out.println("You've used a " + potions[choice].get_name() + " and restored " + gain + " mana!");
				}
				Global.Pause();
				//decrement item from inventory
				dec_inventory(potions[choice]);
				return true;
			}
			else if (choice == potions.length) {return false;} /* cancels out */
			else {
				System.out.print("Invalid entry.");
				choice = new Scanner(System.in).nextInt();
			}
		}
	}
	public boolean is_dead() {
		if (current_health <= 0) {return true;}
		else {return false;}
	}
	
	//EQUIPMENT MANIPULATION
	public void unequip_item(Item item) {
		if (item.get_type() == "Armor") {
			int armor_loss = item.item_stat_value();
			if (player_class == "Warrior") {
				equip_def_bonus -= armor_loss;
				armor_rating = equip_def_bonus;
			}
			else if (player_class == "Rogue") {
				equip_def_bonus -= armor_loss;
				armor_rating = equip_def_bonus;
			}
			else if (player_class == "Mage") {
				equip_def_bonus -= armor_loss;
				armor_rating = equip_def_bonus;
			}
			equipment[0] = new Item("Armor", "Cloth", 0);
			inc_inventory(item);
			init_stats();
			System.out.println("\nYou've removed " + item.get_name() + ".");
			Global.Pause();
		}
		else if (item.get_type() == "Weapon") {
			int weapon_loss = item.item_stat_value();
			if (player_class == "Warrior") {
				if (item.get_subtype() == "Sword") {weapon_loss *= 2;}
			}
			else if (player_class == "Rogue") {
				if (item.get_subtype() == "Dagger") {weapon_loss *= 2;}
			}
			else if (player_class == "Mage") {
				if (item.get_subtype() == "Staff") {
					weapon_loss *= 2;
					equip_mana_bonus -= weapon_loss;
				}
			}
			equip_atk_bonus -= weapon_loss;
			equipment[1] = new Item("Weapon", "Staff", 0);
			inc_inventory(item);
			init_stats();
			System.out.println("\nYou've removed " + item.get_name() + ".");
			Global.Pause();
		}
		else if (item.get_type() == "Shield") {
			int shield_loss = item.item_stat_value();
			equip_def_bonus -= shield_loss;
			armor_rating = equip_def_bonus;
			equipment[2] = new Item("Shield", "none", 0);
			inc_inventory(item);
			init_stats();
			System.out.println("\nYou've removed " + item.get_name() + ".");
			Global.Pause();
		}
	}
	public void equip_item(Item item) {
		for (int i = 0; i < equipment.length; i++) {
			if (item.get_name() == equipment[i].get_name()) {return;}
		}
		if (item.get_type() == "Armor") {
			int armor_gain;
			if (player_class == "Warrior") {
				if (equipment[0].get_level() != 0) {unequip_item(equipment[0]);}
				equipment[0] = item;
				armor_gain = equipment[0].item_stat_value();
				equip_def_bonus += armor_gain;
				armor_rating = equip_def_bonus;
				init_stats();
				dec_inventory(item);
				System.out.println("\nYou've equipped " + item.get_name() + ".");
			}
			else if (player_class == "Rogue") {
				if (item.get_subtype() == "Mail") {
					System.out.println("You cannot equip mail armor.");
				}
				else {
					if (equipment[0].get_level() != 0) {unequip_item(equipment[0]);}
					equipment[0] = item;
					armor_gain = equipment[0].item_stat_value();
					equip_def_bonus += armor_gain;
					armor_rating = equip_def_bonus;
					init_stats();
					dec_inventory(item);
					System.out.println("\nYou've equipped " + item.get_name() + ".");
				}
			}
			else if (player_class == "Mage") {
				if (item.get_subtype() == "Cloth") {
					if (equipment[0].get_level() != 0) {unequip_item(equipment[0]);}
					equipment[0] = item;
					armor_gain = equipment[0].item_stat_value();
					equip_def_bonus += armor_gain;
					armor_rating = equip_def_bonus;
					init_stats();
					dec_inventory(item);
					System.out.println("\nYou've equipped " + item.get_name() + ".");
				}
				else {
					System.out.println("You cannot equip leather or mail armor.");
				}
			}
			Global.Pause();
		}
		else if (item.get_type() == "Weapon") {
			int weapon_gain;
			if (equipment[1].get_level() != 0) {unequip_item(equipment[1]);}
			equipment[1] = item;
			weapon_gain = equipment[1].item_stat_value();
			if (player_class == "Warrior") {
				if (equipment[1].get_subtype() == "Sword") {weapon_gain *= 2;}
			}
			else if (player_class == "Rogue") {
				if (equipment[1].get_subtype() == "Dagger") {weapon_gain *= 2;}
			}
			else if (player_class == "Mage") {
				if (equipment[1].get_subtype() == "Staff") {
					weapon_gain *= 2;
					equip_mana_bonus += weapon_gain;
				}
			}
			equip_atk_bonus += weapon_gain;
			init_stats();
			dec_inventory(item);
			System.out.println("\nYou've equipped " + item.get_name() + ".");
			Global.Pause();
		}
		else if (item.get_type() == "Shield") {
			int shield_gain;
			if (player_class == "Warrior") {
				if (equipment[2].get_level() != 0) {unequip_item(equipment[2]);}
				equipment[2] = item;
				shield_gain = equipment[2].item_stat_value();
				equip_def_bonus += shield_gain;
				armor_rating = equip_def_bonus;
				block = 5;
				init_stats();
				dec_inventory(item);
				System.out.println("\nYou've equipped " + item.get_name() + ".");
			}
			else {System.out.println("You cannot equip shields.");}
			Global.Pause();
		}
	}
	public void equipment_menu() {
		start:
		while (true) {
			view_equipment();
			System.out.println("\nWhat would you like to do?");
			System.out.println("1. Equip an item");
			System.out.println("2. Unequip an item");
			System.out.println("3. Leave menu");
		
			char choice = new Scanner(System.in).next().charAt(0);
			switch (choice) {
				case '1':
				case 'E':
				case 'e':
					if (num_of_equipable_items() > 0) {
						view_equipment();
						System.out.println();
						Item inv[] = new Item[num_of_equipable_items()];
						inv = view_equipable_items();
						System.out.println("\nWhich item would you like to equip?");
						int equip_choice = new Scanner(System.in).nextInt();
						while (true) {
							if (equip_choice <= inv.length) {
								equip_item(inv[equip_choice - 1]);
								continue start;
							}
							else {
								System.out.print("Invalid entry");
								equip_choice = new Scanner(System.in).nextInt();
							}
						}
					}
					else {
						System.out.println("\nYou have no equippable items.\n");
						Global.Pause();
						continue start;
					}
				case '2':
				case 'U':
				case 'u':
					if (equipment[0].get_level() == 0 && equipment[1].get_level() == 0 && equipment[2].get_level() == 0) {
						System.out.println("You have nothing equipped; you cannot unequip anything.");
						continue start;
					}
					view_equipment();
					System.out.println("Cancel request");
					System.out.println("\nWhich equipment would you like to unequip?");
					char unequip_choice = new Scanner(System.in).next().charAt(0);
					while (true) {
						switch (unequip_choice) {
							case '1':
							case 'A':
							case 'a':
								unequip_item(equipment[0]);
								continue start;
							case '2':
							case 'W':
							case 'w':
								unequip_item(equipment[1]);
								continue start;
							case '3':
							case 'S':
							case 's':
								unequip_item(equipment[2]);
								continue start;
							case '4':
							case 'C':
							case 'c':
								continue start;
							default:
								System.out.print("Invalid entry.");
								unequip_choice = new Scanner(System.in).next().charAt(0);
						}
					}
				case '3':
				case 'L':
				case 'l':
					return;
				default:
					System.out.print("Invalid entry.");
					choice = new Scanner(System.in).next().charAt(0);
			}
		}
	}

	//GET FUNCTIONS
	public int get_base_str() {return base_str;}
	public int get_base_agi() {return base_agi;}
	public int get_base_int() {return base_int;}
	public int get_base_sta() {return base_sta;}
	public int get_points() {return points;}
	public int get_exp() {return exp;}
	public int get_gold() {return gold;}
	public int get_armor_rating() {return armor_rating;}
	public int get_level() {return level;}
	public int get_atk() {return atk;}
	public int get_def() {return def;}
	public int get_max_health() {return max_health;}
	public int get_max_mana() {return max_mana;}
	public int get_current_mana() {return current_mana;}
	public int get_current_health() {return current_health;}
	public int get_equip_atk_bonus() {return equip_atk_bonus;}
	public int get_equip_def_bonus() {return equip_def_bonus;}
	public int get_equip_health_bonus() {return equip_health_bonus;}
	public int get_equip_mana_bonus() {return equip_mana_bonus;}
	public int get_accuracy() {return acc;}
	public int get_crit() {return crit;}
	public String get_name() {return name;}
	public String get_player_class() {return player_class;}
	public Item[] get_inventory() {return inventory;}
	public Item[] get_equipment() {return equipment;}
	public Spell[] get_spellbook() {return spellbook;}
	public boolean get_did_cast_spell() {return did_cast_spell;}
	
	//LOAD FUNCTIONS
	public void load_base_str(int x) {base_str = x;}
	public void load_base_agi(int x) {base_agi = x;}
	public void load_base_int(int x) {base_int = x;}
	public void load_base_sta(int x) {base_sta = x;}
	public void load_points(int x) {points = x;}
	public void load_exp(int x) {exp = x;}
	public void load_gold(int x) {gold = x;}
	public void load_armor_rating(int x) {armor_rating = x;}
	public void load_level(int x) {level = x;}
	public void load_atk(int x) {atk = x;}
	public void load_def(int x) {def = x;}
	public void load_max_health(int x) {max_health = x;}
	public void load_max_mana(int x) {max_mana = x;}
	public void load_current_mana(int x) {current_mana = x;}
	public void load_current_health(int x) {current_health = x;}
	public void load_equip_atk_bonus(int x) {equip_atk_bonus = x;}
	public void load_equip_def_bonus(int x) {equip_def_bonus = x;}
	public void load_equip_health_bonus(int x) {equip_health_bonus = x;}
	public void load_equip_mana_bonus(int x) {equip_mana_bonus = x;}
	public void load_name(String s) {name = s;}
	public void load_player_class(String s) {player_class = s;}
	public void load_inventory(Item[] items) {
		inventory = new Item[items.length];
		for (int i = 0; i < items.length; i++) {inventory[i] = items[i];}
	}
	public void load_equipment(Item[] items) {
		equipment = new Item[items.length];
		for (int i = 0; i < items.length; i++) {equipment[i] = items[i];}
	}
	
	//VARIABLES
	private int base_str, base_agi, base_int, base_sta;
	private int strength, agility, intellect, stamina;
	private int points, exp, gold, armor_rating, level;
	private int atk, def, max_health, max_mana, current_mana, current_health;
	private int equip_atk_bonus, equip_def_bonus, equip_health_bonus, equip_mana_bonus;
	private int crit, dodge, parry, block, acc;
	private int exp_to_next_level, spellpower;
	private boolean did_cast_spell;
	private String name, player_class;
	private Item[] inventory, equipment;
	private Spell[] spellbook;
	private static int ATTRIB_BONUS = 5;
	private static int CLASS_BONUS = 10;
	private static int LEVEL_INCREMENT = 50;
	
	//establishes pre-customization baseline for attributes
	private void init_attribs() {
		base_str = base_agi = base_int = base_sta = 10;
		equip_atk_bonus = equip_def_bonus = equip_health_bonus = equip_mana_bonus = 0;
		points = 20;
		exp = armor_rating = spellpower = 0;
		level = 1;
		gold = 300;
	}

	//CLASS DESCRIPTIONS
	private void warrior_description() {
		System.out.print("Warriors are melee fighters highly trained in the art ");
		System.out.print("of weaponry. They are unmatched in one-on-one physical combat, ");
		System.out.print("have unusually high stamina, and can wear any type of armor. They ");
		System.out.print("are the only class that can use shields.");
		System.out.println("\n\nA warrior's primary attribute is Strength.");
	}
	private void rogue_description() {
		System.out.print("Rogues are highly trained in the art of subterfuge and stealth. ");
		System.out.print("They specialize in taking their enemy by surprise, and dealing ");
		System.out.print("copious amounts of damage by exploiting this advantage. Rogues cannot ");
		System.out.print("wear heavy armor or use shields.");
		System.out.println("\n\nA rogue's primary attribute is Agility.");
	}
	private void mage_description() {
		System.out.print("Mages are masters of the elements and arcane spells. They use ");
		System.out.print("both offensive and defensive spells to both decimate their foes and ");
		System.out.print("protect and heal themselves. To assist them in channelling these potent ");
		System.out.print("magics, mages elect to wear flowing robes of magically-imbued cloth and ");
		System.out.print("eschew more traditional forms of protection.");
		System.out.println("\n\nA mage's primary attribute is Intellect.");
	}
	
	//sets the class from the class_menu method
	private boolean set_class(String s) {
		char choice = new Scanner(System.in).next().charAt(0);
		while (true) {
			if (choice == 'Y' || choice == 'y') {
				player_class = s;
				return true;
			}
			else if (choice == 'N' || choice == 'n') {return false;}
			else {
				System.out.print("Please enter either Y or N: ");
				choice = new Scanner(System.in).next().charAt(0);
			}
		}
	}
	
	//CLASS PRESET BONUSES
	private void warrior_bonus() {
		base_str += CLASS_BONUS;
		base_sta += CLASS_BONUS;
	}
	private void rogue_bonus() {base_agi += CLASS_BONUS;}
	private void mage_bonus() {base_int += CLASS_BONUS;}
	
	//INVENTORY MANIPULATION
	private void add_to_inventory(Item item) {
		//create temp array with current number of items in inventory
		Item tmp_array[] = new Item[inventory.length];
		for (int i = 0; i < inventory.length; i++) {tmp_array[i] = inventory[i];}
		
		//set inventory to the new size and copy current items back in
		int num = inventory.length + 1;
		inventory = new Item[num];
		for (int i = 0; i < inventory.length - 1; i++) {inventory[i] = tmp_array[i];}
		
		//add the new item to the last (new) index in inventory
		inventory[inventory.length - 1] = item;
	}
	private void remove_from_inventory(Item item) {
		//create temp array with one less index
		Item tmp_array[] = new Item[inventory.length - 1];
		
		//moves over the items to be kept
		int count = 0;
		for (int i = 0; i < inventory.length; i++) {
			if (inventory[i].get_name() != item.get_name()) {
				tmp_array[count] = inventory[i];
				count++;
			}
		}
		
		//set inventory to the new size and copy updated items back in
		int num = inventory.length - 1;
		inventory = new Item[num];
		for (int i = 0; i < inventory.length; i++) {inventory[i] = tmp_array[i];}
	}
	private int num_of_equipable_items() {
		int count = 0;
		for (int i = 0; i < inventory.length; i++) {
			if (inventory[i].equipable()) {count++;}
		}
		return count;
	}
}

class Map {
	public Map() {
		size = 5; /*number of spaces in a row*/
		space = 'O';
		init_map();
		set_town();
	}
	
	public void init_map() {
		map = new char[size * size];
		for (int i = 0; i < size * size; i++) {map[i] = space;}
	}
	
	public void draw_map() {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				System.out.print(map[i * size + j] + " ");
			}
			System.out.println();
		}
	}
	
	public int move_player(int start, char direction) {
		int finish = 0;
		if (direction == 'N') {finish = start - size;}
		if (direction == 'W') {finish = start - 1;}
		if (direction == 'E') {finish = start + 1;}
		if (direction == 'S') {finish = start + size;}
		
		if (map[start] != 'T') {map[start] = 'X';}
		map[finish] = '*';
		return finish;
	}
	
	public void enter_town(int position) {
		if (position == town) {
			System.out.print("You stumble back to the entrance of town. You can't believe ");
			System.out.print("you managed to survive out there amidst nature's chaos!");
			System.out.println();
			Global.Pause();
		}
	}
	
	//verifies that player can move in a particular direction
	public boolean can_move_north(int position) {
		if (position > size - 1) {return true;}
		else {return false;}
	}
	public boolean can_move_south(int position) {
		if (position < (size * size) - size) {return true;}
		else {return false;}
	}
	public boolean can_move_west(int position) {
		if (position % size > 0) {return true;}
		else {return false;}
	}
	public boolean can_move_east(int position) {
		if (position % size != size - 1) {return true;}
		else {return false;}
	}
	
	//get initial position in the wild
	int get_town() {return town;}
	
	//VARIABLES
	private char space;
	private char[] map;
	private int size, town;
	
	private void set_town() {
		town = (int)(Math.random() * (size * size));
		map[town] = 'T';
	}
};

public class Main {
	public static Creature det_creature(int level) {
		Creature bunny = new Creature("Balanced", level);
		Creature ferret = new Creature("Offensive", level);
		Creature midget = new Creature("Defensive", level);
		
		//determines type of creature encountered
		int x = (int)(Math.random() * 101);
		if (x <= 65) {return bunny;} /*bunny encounter - 65% chance*/
		else if (x > 65 && x <= 90) {return ferret;} /*ferret encounter - 25% chance*/
		else {return midget;} /*midget encounter - 10% chance*/
	}

	public static Player fight_menu(Player player) {
		Creature creature;
		creature = det_creature(player.get_level());
		System.out.println("You've encountered a " + creature.get_name() + "!\n");
		Global.Pause();
	
		start:
		while (true) {
			Global.Clear();
			System.out.println("What would you like to do?");
			System.out.println("1. Attack");
			System.out.println("2. Cast a spell");
			System.out.println("3. Use item");
			System.out.println("4. Run away\n");
			System.out.println("Your health: " + player.get_current_health() + "/" + player.get_max_health());
			System.out.println("Your mana: " + player.get_current_mana() + "/" + player.get_max_mana() + "\n");
			System.out.println(creature.get_name() + "'s health: " + creature.get_health());
			
			char choice = new Scanner(System.in).next().charAt(0);
			switch (choice) {
				case '1':
				case 'A':
				case 'a':
					creature.take_damage(player.get_atk(), player.get_accuracy(), player.get_crit());
					if (creature.is_dead()) {
						System.out.println("You've killed the " + creature.get_name() + "!");
						player.gain_exp(creature.get_exp());
						player.add_gold(creature.get_money());
						System.out.println("You've looted " + creature.get_money() + " gold!");
						if (creature.get_has_loot()) {player.inc_inventory(creature.get_loot());}
						Global.Pause();
						return player;
					}
					player.take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit());
					if (player.is_dead()) {
						player.dies();
						return player;
					}
					Global.Pause();
					continue start;
				case '2':
				case 'C':
				case 'c':
				case 'S':
				case 's':
					if (player.get_player_class() == "Mage") {
						creature = player.cast_spell(creature);
						if (player.get_did_cast_spell()) {
							if (creature.is_dead()) {
								System.out.println("You've killed the " + creature.get_name() + "!");
								player.gain_exp(creature.get_exp());
								player.add_gold(creature.get_money());
								System.out.println("You've looted " + creature.get_money() + " gold!");
								if (creature.get_has_loot()) {player.inc_inventory(creature.get_loot());}
								Global.Pause();
								return player;
							}
							player.take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit());
							if (player.is_dead()) {
								player.dies();
								return player;
							}
							Global.Pause();
						}
					}
					else {
						System.out.print("You don't have any spells to cast. ");
						System.out.println("You're not a Mage, " + player.get_name() + "!");
						Global.Pause();
					}
					continue start;
				case '3':
				case 'U':
				case 'u':
					if (player.use_item()) {
						player.take_damage(creature.get_atk(), creature.get_accuracy(), creature.get_crit());
						if (player.is_dead()) {
							player.dies();
							return player;
						}
						Global.Pause();
					}
					continue start;
				case '4':
				case 'R':
				case 'r':
					System.out.print("You flee like a coward! Were you that scared ");
					System.out.println("of a " + creature.get_name() + "?");
					Global.Pause();
					return player;
				default:
					System.out.print("Invalid entry. ");
					choice = new Scanner(System.in).next().charAt(0);
			}
		}
	}

	public static void entering_wild() {
		Global.Clear();
		System.out.print("You have entered the wild! It's a large and scary meadow, ");
		System.out.println("filled with gross things and cute creatures.\n");
		Global.Pause();
	}
	
	public static Player wild_menu(Player player) {
		Map map = new Map();
		int position = map.get_town();
		int creature_chance = 35;
			
		entering_wild();
			
		start:	
		while (true) {
			//handles death situations (returns to town)
			if (player.is_dead()) {
				player.restore_health_to_max();
				player.restore_mana_to_max();
				return player;
			}
			Global.Clear();
			System.out.println("Map:");
			map.draw_map();
	
			System.out.println("What would you like to do?");
			System.out.println("(N) Move north");
			System.out.println("(W) Move west");
			System.out.println("(E) Move east");
			System.out.println("(S) Move south");
			System.out.println("1. Return to town");
			System.out.println("2. Player menu");
	
			char choice = new Scanner(System.in).next().charAt(0);
			switch (choice) {
				case 'N':
				case 'n':
					if (map.can_move_north(position)) {
						position = map.move_player(position, 'N');
						if ((int)(Math.random() * 101) < creature_chance) {player = fight_menu(player);}
						else {
							System.out.print("You move north; so far, the coast seems clear. ");
							System.out.println("Who's really scared of bunnies, anyway?");
							Global.Pause();
						}
						if (position == map.get_town()) {
							map.enter_town(position);
							return player;
						}
						continue start;
					}
					else {
						System.out.print("You can't move north. Please try another direction: ");
						choice = new Scanner(System.in).next().charAt(0);
					}
				case 'W':
				case 'w':
					if (map.can_move_west(position)) {
						position = map.move_player(position, 'W');
						if ((int)(Math.random() * 101) < creature_chance) {player = fight_menu(player);}
						else {
							System.out.print("You move west; so far, nothing has eaten you. ");
							System.out.println("Flowers can't eat you, right?");
							Global.Pause();
						}
						if (position == map.get_town()) {
							map.enter_town(position);
							return player;
						}
						continue start;
					}
					else {
						System.out.print("You can't move west. Please try another direction: ");
						choice = new Scanner(System.in).next().charAt(0);
					}
					break;
				case 'E':
				case 'e':
					if (map.can_move_east(position)) {
						position = map.move_player(position, 'E');
						if ((int)(Math.random() * 101) < creature_chance) {player = fight_menu(player);}
						else {
							System.out.print("You move east; so far, everything is fine. ");
							System.out.println("Grass just makes you itch, after all.");
							Global.Pause();
						}
						if (position == map.get_town()) {
							map.enter_town(position);
							return player;
						}
						continue start;
					}
					else {
						System.out.println("You can't move east. Please try another direction: ");
						choice = new Scanner(System.in).next().charAt(0);
					}
					break;
				case 'S':
				case 's':
					if (map.can_move_south(position)) {
						position = map.move_player(position, 'S');
						if ((int)(Math.random() * 101) < creature_chance) {player = fight_menu(player);}
						else {
							System.out.print("You move south; so far, all you see are bees ");
							System.out.println("buzzing around the flowers. Oh man, BEES!?");
							Global.Pause();
						}
						if (position == map.get_town()) {
							map.enter_town(position);
							return player;
						}
						continue start;
					}
					else {
						System.out.println("You can't move south. Please try another direction: ");
						choice = new Scanner(System.in).next().charAt(0);
					}
					break;
				case '1':
					return player;
				case '2':
					player = player_menu(player);
					continue start;
				default:
					System.out.print("Invalid entry. ");
					choice = new Scanner(System.in).next().charAt(0);
			}
		}
	}
	
	public static void town_menu(Player player) {
		Blacksmith town_blacksmith = new Blacksmith(player.get_level());
		Inn town_inn = new Inn(player.get_level());

		start:
		while (true) {
			Global.Clear();
			System.out.println("You have entered town.\n");
			System.out.println("What would you like to do?");
			System.out.println("1. Go to the blacksmith");
			System.out.println("2. Go to the inn");
			System.out.println("3. Leave the town (enter the wild)");
			System.out.println("4. Player menu");
	
			char choice = new Scanner(System.in).next().charAt(0);
			switch (choice) {
				case '1':
					player = town_blacksmith.menu(player);
					continue start;
				case '2':
					player = town_inn.menu(player);
					continue start;
				case '3':
					player = wild_menu(player);
					continue start;
				case '4':
					player = player_menu(player);
					continue start;
				default:
					System.out.print("Invalid input. ");
					choice = new Scanner(System.in).next().charAt(0);
			}
		}
	}

	//SAVE-LOAD FUNCTIONS
	public static Player save_or_load(Player player) {
		start:
		while (true) {
			Global.Clear();
			System.out.println("What would you like to do?");
			System.out.println("1. Save player");
			System.out.println("2. Load player");
			System.out.println("3. Exit menu");
			char choice = new Scanner(System.in).next().charAt(0);
			switch (choice) {
				case '1':
				case 'S':
				case 's':
//					save_game(player);
					continue start;
				case '2':
				case 'L':
				case 'l':
//					player = load_game(player);
					continue start;
				case '3':
				case 'X':
				case 'x':
				case 'E':
				case 'e':
					return player;
				default:
					System.out.print("Invalid entry. ");
					choice = new Scanner(System.in).next().charAt(0);
			}
		}
	}
	public static boolean saves_exist() {
		File file = new File("save_games.txt");
		if (file.exists()) {return true;}
		else {return false;}
	}
	public static Player load_or_create_player_menu(Player player) {
		if (saves_exist()) {
			System.out.println("What would you like to do?");
			System.out.println("1. Load player");
			System.out.println("2. New player");
			char choice = new Scanner(System.in).next().charAt(0);
			boolean repeat = true;
			while (repeat) {
				switch (choice) {
					case '1':
					case 'L':
					case 'l':
//						player = load_game(player);
						repeat = false;
					case '2':
					case 'N':
					case 'n':
						player.construct();
						repeat = false;
					default:
						System.out.print("Invalid entry. ");
						choice = new Scanner(System.in).next().charAt(0);
				}
			}
		}
		else {player.construct();}
		return player;
	}
	public static void save_game(Player player) {
		//save name of game to a separate file
		File file = new File("save_games.txt");
		boolean save_exists = false;
		char line[30];
		for (int i = 0; file.good(); i++) {
			file.getline(line, 30, '\n');
			if (line == filename) {save_exists = true;}
		}
		file.close();
		Global.Clear();
		System.out.print("To create a save game, enter a name: ");
		String filename = new Scanner(System.in).nextLine();
		start:
		while (repeat) {
			cout << "A game already exists with this name. Overwrite (Y/N)? ";
			char decision = new Scanner(System.in).next().charAt(0);
			bool repeat = true;
			if (save_exists) {
				if (decision == 'Y' || decision == 'y') {repeat = false;}
				else if (decision == 'N' || decision == 'n') {continue start;}
				else {
					cout << "Invalid entry.";
					cin >> decision;
				}
			}
		}
		else {
			file.open("save_games.txt", fstream::out |  fstream::app);
			file << filename << endl;
			file.close();
		}
	
		cout << "\nSaving game...." << endl;
	
		//attributes and stats
		file.open(filename + "_int.txt", fstream::out);
		file << player.get_base_str() << endl;
		file << player.get_base_agi() << endl;
		file << player.get_base_int() << endl;
		file << player.get_base_sta() << endl;
		file << player.get_points() << endl;
		file << player.get_exp() << endl;
		file << player.get_gold() << endl;
		file << player.get_armor_rating() << endl;
		file << player.get_level() << endl;
		file << player.get_atk() << endl;
		file << player.get_def() << endl;
		file << player.get_max_health() << endl;
		file << player.get_max_mana() << endl;
		file << player.get_items_in_inventory() << endl;
		file.close();
		file.open(filename + "_char.txt", fstream::out);
		file << player.get_name().c_str() << endl;
		file << player.get_player_class().c_str() << endl;
		file.close();
	
		//inventory - only if there is any
		if (player.has_inventory()) {
			Item *item = player.get_inventory();
			for (int i = 0; i < player.get_items_in_inventory(); i++) {
				file.open(filename + "_char.txt", fstream::out | fstream::app);
				file << item[i].get_type() << endl;
				file << item[i].get_subtype() << endl;
				file.close();
				file.open(filename + "_int.txt", fstream::out | fstream::app);
				file << item[i].get_level() << endl;
				file << item[i].get_quantity() << endl;
				file.close();
			}
			delete[] item;
			item = 0;
		}
	
		//equipment
		Item *equipment = player.get_equipment();
		for (int i = 0; i < 3; i++) {
			file.open(filename + "_char.txt", fstream::out | fstream::app);
			file << equipment[i].get_type() << endl;
			file << equipment[i].get_subtype() << endl;
			file.close();
			file.open(filename + "_int.txt", fstream::out | fstream::app);
			file << equipment[i].get_level() << endl;
			file.close();
		}
	
		cout << filename << " has been saved." << endl;
		system("PAUSE");
	}

	public static Player player_menu(Player player) {
		start:
		while (true) {
			Global.Clear();

			System.out.println("Player menu:\n");
			System.out.println("What would you like to do?");
			System.out.println("1. View attributes");
			System.out.println("2. View stats");
			System.out.println("3. View inventory");
			System.out.println("4. Use item");
			System.out.println("5. Cast healing spell");
			System.out.println("6. Equipment");
			System.out.println("7. Load / Save Player");
			System.out.println("8. Exit menu");
			System.out.println("9. Quit game");
	
			char choice = new Scanner(System.in).next().charAt(0);
			switch (choice) {
				case '1':
					player.view_attributes();
					Global.Pause();
					continue start;
				case '2':
					player.view_stats();
					Global.Pause();
					continue start;
				case '3':
					player.view_inventory();
					Global.Pause();
					continue start;
				case '4':
					player.use_item();
					continue start;
				case '5':
				case 'H':
				case 'h':
					player.cast_healing_spell();
					continue start;
				case '6':
					player.equipment_menu();
					continue start;
				case '7':
//					save_or_load(player);
					continue start;
				case '8':
				case 'X':
				case 'x':
					return player;
				case '9':
				case 'Q':
				case 'q':
					Global.Clear();
					System.out.print("Are you sure you sure you want to quit? Don't be that guy, man. ");
					System.out.print("Are you really that much of a pathetic, lowlife loser (Y/N)? ");
					char decision = new Scanner(System.in).next().charAt(0);
					while (true) {
						if (decision == 'Y' || decision == 'y') {System.exit(0);}
						else if (decision == 'N' || decision == 'n') {
							System.out.println("That's the spirit, champ!");
							Global.Pause();
							continue start;
						}
						else {
							System.out.print("Invalid entry. ");
							decision = new Scanner(System.in).next().charAt(0);
						}
					}
				default:
					System.out.print("Invalid entry. ");
					choice = new Scanner(System.in).next().charAt(0);
			}
		}
	}
	
	public static void main(String args[]) {
		System.out.println("Welcome to RPG Land!\n");
		
		player = load_or_create_player_menu(player);
		town_menu(player);
	}
}