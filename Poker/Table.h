#pragma once
#include "Player.h"
#include "Card.h"
#include "Deck.h"
class LeducPokerTable {
public:
	Player players[2];
	Card community_cards;
	int playerlen, total_pot;
	Deck deck;
	LeducPokerTable() {}
	LeducPokerTable(int _playerlen, Player _players[]) {
		this->playerlen = _playerlen;
		total_pot = 0;
		for (int i = 0; i < playerlen; i++)
			players[i] = _players[i];
	}
	void reset() {
		total_pot = 0;
		//deck.reset();
		for (int i = 0; i < playerlen; i++)
			players[i].reset();
	}
	int total() {
		int ans = 0;
		for (int i = 0; i < playerlen; i++)
			ans += players[i].n_bet_chips();
		return ans;
	}
	inline void Add_pot(int addchips) {
		total_pot += addchips;
	}
	void deal_community_cards() {
		community_cards.eval_card = deck.deal_one_card();
	}
	void deal_private_cards() {
		for (int i = 0; i < playerlen; i++)
			players[i].add_private_card(deck.deal_one_card());
	}

	void _assign_blinds(int small_blind, int big_blind) {
		players[0].add_to_pot(small_blind);
		players[1].add_to_pot(big_blind);
		total_pot = small_blind + big_blind;
	}
};