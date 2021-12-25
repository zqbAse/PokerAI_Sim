#pragma once
#include "Card.h"
#include <exception>
class Player
{
public:
	unsigned char id;		//玩家id
	int initial_chips, n_chips;	//initial_chips初始筹码，n_chips当前投入pot的筹码个数
	bool active;			//是否fold
	unsigned char clusters[4];	//clusters[0]表示preflop该玩家手牌属于哪一类，clusters[1]表示flop该玩家手牌属于哪一类，clusters[2]表示turn该玩家手牌属于哪一类
	Card cards;		//玩家的hole card
	Player() {}
	Player(int id, int initial_chips) {
		//cout << "pot address:" << pot.pot << endl;
		this->id = id;
		this->initial_chips = initial_chips;
		this->n_chips = initial_chips;
		this->active = true;
	}
	void reset() {
		this->n_chips = this->initial_chips;
		this->active = true;
	}
	void add_chips(int chips) {
		this->n_chips += chips;
	}
	void call(int call_bet) {
		add_to_pot(call_bet);
	}
	void raise_to(int chips) {
		add_to_pot(chips);
	}
	void add_to_pot(int chips) {
		n_chips -= chips;
	}
	inline int n_bet_chips() {
		return initial_chips - n_chips;
	}

	void add_private_card(int c1) {
		cards.eval_card = c1;
	}
};
