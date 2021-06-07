#pragma once
#include <map>
#include <algorithm>
#include "Card.h"
#include "../Util/Randint.h"
using namespace std;
const int card_rule_len = 6;
const int cardlen3 = 1000;
const int cardlen2 = 100;
const int cardlen1 = 10;
class Deck {
public:
	unsigned char* cards = new unsigned char[card_rule_len];
	unsigned char cur_index;
	Randint randi;
	Deck() {
		//randi.prngState = 2;
		reset();
	}
	void reset() {
		cur_index = 0;
		for (int i = 0; i < card_rule_len; i++)
			cards[i] = i;
		//srand((unsigned)time(NULL));
		for (int i = 0; i < card_rule_len; i++)
			swap(cards[i], cards[randi._rand() % card_rule_len]);
		for (int i = 0; i < card_rule_len; i++)
			swap(cards[i], cards[randi._rand() % card_rule_len]);
	}
	/*void println() {
		cout << "deck index:" << cur_index << endl;
	}*/
	unsigned char deal_one_card() {
		return cards[cur_index++];
	}
};