#pragma once
#include <iostream>
#include <string.h>
using namespace std;

struct strategy_node {		//info set node
	int playerid, bestresponse;
	bool chanced;
	int chancenode_id;
	int chancempde_cards;
	int action_len = 0;
	char* actionstr;
	double* regret;
	//int* ave_strategy;
	strategy_node* actions;
	strategy_node() :action_len(0), chancenode_id(0), chanced(false) {}
	strategy_node* findnode(char action) {
		int i = 0;
		for (; i < action_len; i++) {
			if (*(actionstr + i) == action)
				break;
		}
		return (actions + i);
	}
	void init_child(char* action_str) {
		if (action_len == 0) {
			actionstr = action_str;
			int aclen = strlen(action_str);
			actions = new strategy_node[aclen]();
			regret = new double[aclen] {0};
			action_len = aclen;
		}
	}
	void init_chance_node(int initlen) {
		if (action_len == 0) {
			actions = new strategy_node[initlen]();
			//regret = new int[initlen] {0};
			action_len = initlen;
		}
	}
};
class Sim_State {
public:
	int initial_chips;
	int last_raise, pot, curplayer, foldp;
	int rivercluster;
	int players_betchips, betting_stage;
	Sim_State(int big_small_blind) {
		reset_game(big_small_blind);
	}
	void reset_game(int big_small_blind) {
		initial_chips = 400;
		betting_stage = -1;
		foldp = -1;
		last_raise = 50;//every round last raise
		curplayer = 0; //current action player id
		pot = 150;//current raise time
		rivercluster = -1;
		players_betchips = big_small_blind;
	}
	void reset_betting_round_state() {
		last_raise = 0;
		curplayer = 0;
	}
	void move_to_next_player() {
		curplayer ^= 1;
	}
	void take_action(char actionstr, int playeri) {
		int paychips = 0;
		if (actionstr == 'l') {
			paychips = last_raise;
			last_raise = 0;
		}
		else if (actionstr == 'd')
			foldp = curplayer;
		else if (actionstr == 'n') {
			paychips = last_raise;
			last_raise = initial_chips - (pot + last_raise) / 2;
			paychips += last_raise;
		}
		else {
			cout << "action:" << actionstr << ",not exist the action" << endl;
			throw exception();
		}
		pot += paychips;
		if (curplayer != playeri)
			players_betchips += paychips;
		if (pot > 2 * initial_chips || players_betchips > initial_chips) {
			cout << "pot:" << pot << endl;
			cout << "players_betchips:" << players_betchips << endl;
			throw exception();
		}
		move_to_next_player();
	}
};
/*void calculate_strategy(int* regret, int len, double sigma[]) {
	double sum = 0;
	for (int i = 0; i < len; i++) {
		if (regret[i] > 0)
			sum += regret[i];
	}
	if (sum > 0)
		for (int i = 0; i < len; i++)
			if (regret[i] > 0)
				sigma[i] = regret[i] / sum;
			else
				sigma[i] = 0;
	else
		for (int i = 0; i < len; i++)
			sigma[i] = 1.0 / len;
}*/
void calculate_strategy(double* regret, int len, double sigma[]) {
	double sum = 0;
	for (int i = 0; i < len; i++) {
		if (regret[i] > 0)
			sum += regret[i];
	}
	if (sum > 0)
		for (int i = 0; i < len; i++)
			if (regret[i] > 0)
				sigma[i] = regret[i] / sum;
			else
				sigma[i] = 0;
	else
		for (int i = 0; i < len; i++)
			sigma[i] = 1.0 / len;
}