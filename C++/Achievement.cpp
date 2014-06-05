#include"Achievement.h"
using namespace std;

//INITIALIZATION
void Achievement::set_points() {
	if (type == "Quest" && rank < MAX_LEVEL + 1) {points = 10;}
	else if (type == "Quest" && rank == MAX_LEVEL + 1) {points = 10 * MAX_LEVEL;}
	else {cout << "Invalid achievement type (set_points)" << endl;}
}
void Achievement::init_achievement(string achieve_type, int level) {
	type = achieve_type;
	rank = level;
	
	//QUEST ACHIEVEMENTS
	if (type == "Quest") {
		//set achievement name
		if (rank == 1) {name = "Stumping senility";}
		else if (rank == 2) {name = "Forbearing flatulence";}
		else if (rank == 3) {name = "Coveting the claw";}
		else if (rank == 4) {name = "Fighting off fleas";}
		else if (rank == 5) {name = "Bobbing for apples";}
		else if (rank == MAX_LEVEL + 1) {name = "Rocking the casbah";}
		else {cout << "Invalid rank provided for setting achievement name" << endl;}
		
		//set achievement description
		if (rank < MAX_LEVEL + 1) {
			Quest boss_quest(rank);
			description = "Complete " + boss_quest.get_name();
		}
		else if (rank == MAX_LEVEL + 1) {description = "Complete all 'WANTED' quests";}
		else {cout << "Invalid rank provided for setting achievement description" << endl;}
	}
	//error
	else {cout << "Invalid achievement type (init_achievement)" << endl;}
	
	set_points();
}