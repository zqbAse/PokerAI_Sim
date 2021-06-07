#pragma once
#include "Card.h"
#include <exception>
class Player
{
public:
	unsigned char id;
	int initial_chips, n_chips;
	bool active;
	unsigned char* clusters = new unsigned char[4];
	Card cards;
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

