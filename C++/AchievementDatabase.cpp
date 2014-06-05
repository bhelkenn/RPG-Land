#include"AchievementDatabase.h"
using namespace std;

//CONSTRUCTORS
AchievementDatabase::AchievementDatabase() {
	set_total_count();
	init_all_achievements();
	set_total_points();
}

//INITIALIZATION
void AchievementDatabase::set_total_count() {
	total_count = 0;
	
	//quest achievements
	for (int i = 0; i < MAX_LEVEL; i++) {total_count++;}
	total_count++; //add 1 more for all ranks complete
}
void AchievementDatabase::set_total_points() {
	total_points = 0;
	
	for (int i = 0; i < total_count; i++) {total_points += all_achievements[i].get_points();}
}
void AchievementDatabase::init_all_achievements() {
	all_achievements = new Achievement[total_count];
	
	//quest achievements
	for (int i = 0; i < MAX_LEVEL; i++) {
		all_achievements[i].init_achievement("Quest", i + 1);
	}
	all_achievements[MAX_LEVEL].init_achievement("Quest", MAX_LEVEL + 1);
	
	earned_count = 0;
}

Achievement AchievementDatabase::target_achievement(string achieve_type, int achieve_rank) {
	for (int i = 0; i < total_count; i++) {
		if (all_achievements[i].get_type() == achieve_type && all_achievements[i].get_rank() == achieve_rank) {
			return all_achievements[i];
		}
	}
	cout << "Achievement doesn't exist (AchievementDatabase::target_achievement())" << endl;
	Achievement null;
	return null;
}
void AchievementDatabase::earn_achievement(Achievement &achieve) {
	for (int i = 0; i < total_count; i++) {
		if (all_achievements[i].get_type() == achieve.get_type() && all_achievements[i].get_rank() == achieve.get_rank()) {
			all_achievements[i].earn_achievement();
			earned_count++;
		}
	}
}
bool AchievementDatabase::check_if_earned(Achievement &achieve) {
	for (int i = 0; i < total_count; i++) {
		if (all_achievements[i].get_type() == achieve.get_type() && all_achievements[i].get_rank() == achieve.get_rank()) {
			return all_achievements[i].get_obtained();
		}
	}
	return true;
}

//VIEW FUNCTIONS
void AchievementDatabase::view_unearned_achievements() {
	Clear();
	//check to see if all achievements are already earned
	bool earned_all = true;
	for (int i = 0; i < total_count; i++) {
		if (all_achievements[i].get_obtained() == false) {earned_all = false;}
	}
	if (earned_all) {
		cout << "You've earned all available achievements!" << endl;
		Pause();
		return;
	}
	
	//display only unearned achievements
	cout << "*** QUEST ACHIEVEMENTS ***\n" << endl;
	for (int i = 0; i < total_count; i++) {
		if (all_achievements[i].get_obtained() == false && all_achievements[i].get_type() == "Quest") {
			cout << all_achievements[i].get_name() << " - Rank " << all_achievements[i].get_rank() << ": " << all_achievements[i].get_points() << " points" << endl;
			cout << all_achievements[i].get_description() << endl << endl;
		}
	}
	cout << "*** OTHER ACHIEVEMENTS ***\n" << endl;
	for (int i = 0; i < total_count; i++) {
		if (all_achievements[i].get_obtained() == false && all_achievements[i].get_type() != "Quest") {
			cout << all_achievements[i].get_name() << " - Rank " << all_achievements[i].get_rank() << ": " << all_achievements[i].get_points() << " points" << endl;
			cout << all_achievements[i].get_description() << endl << endl;
		}
	}
	Pause();
}
void AchievementDatabase::view_earned_achievements() {
	Clear();
	//checks to see if there are no earned achievements
	bool no_earned = true;
	for (int i = 0; i < total_count; i++) {
		if (all_achievements[i].get_obtained() == true) {no_earned = false;}
	}
	if (no_earned) {
		cout << "You haven't earned any achievements to display." << endl;
		Pause();
		return;
	}
	
	//display only earned achievements
	cout << "*** QUEST ACHIEVEMENTS ***\n" << endl;
	for (int i = 0; i < total_count; i++) {
		if (all_achievements[i].get_obtained() == true && all_achievements[i].get_type() == "Quest") {
			cout << all_achievements[i].get_name() << " - Rank " << all_achievements[i].get_rank() << ": " << all_achievements[i].get_points() << " points" << endl;
			cout << all_achievements[i].get_description() << endl << endl;
		}
	}
	cout << "*** OTHER ACHIEVEMENTS ***\n" << endl;
	for (int i = 0; i < total_count; i++) {
		if (all_achievements[i].get_obtained() == true && all_achievements[i].get_type() != "Quest") {
			cout << all_achievements[i].get_name() << " - Rank " << all_achievements[i].get_rank() << ": " << all_achievements[i].get_points() << " points" << endl;
			cout << all_achievements[i].get_description() << endl << endl;
		}
	}
	Pause();
}

//GET FUNCTIONS
Achievement* AchievementDatabase::get_earned_achievements() {
	Achievement *earned = new Achievement[earned_count];
	for (int i = 0; i < total_count; i++) {
		int j = 0;
		if (all_achievements[i].get_obtained() == true) {
			earned[j] = all_achievements[i];
			j++;
		}
	}
	return earned;
}

//LOAD FUNCTIONS
void AchievementDatabase::load_achievements(Achievement* &earned, int earned_achievements) {
	earned_count = earned_achievements;
	for (int i = 0; i < earned_count; i++) {
		for (int j = 0; j < total_count; j++) {
			if (earned[i].get_type() == all_achievements[j].get_type() && earned[i].get_rank() == all_achievements[j].get_rank()) {all_achievements[j].earn_achievement();}
		}
	}
}