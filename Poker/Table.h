#pragma once
#include "Player.h"
#include "Card.h"
#include "Deck.h"
class PokerTable {
public:
	Player players[2];
	Card community_cards[5];
	int community_cards_number;
	int playerlen,total_pot;
	Deck deck;
	PokerTable() {}
	PokerTable(int _playerlen, Player _players[]) {
		this->playerlen = _playerlen;
		community_cards_number = 0;
		total_pot = 0;
		for (int i = 0; i < playerlen; i++)
			players[i] = _players[i];
	}
	void reset() {
		community_cards_number = 0;
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
		for (int i = 0; i < 3; i++)
			community_cards[i].eval_card = deck.deal_one_card();
	}
	void deal_private_cards() {
		for (int i = 0; i < playerlen; i++)
			players[i].add_private_card(deck.deal_one_card());
	}
	void deal_flop() {
		community_cards_number += 2;
	}
	void deal_turn() {
		community_cards_number++;
	}

	void _assign_blinds(int small_blind, int big_blind) {
		players[0].add_to_pot(small_blind);
		players[1].add_to_pot(big_blind);
		total_pot = small_blind + big_blind;
	}
};