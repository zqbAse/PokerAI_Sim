#pragma once
#include <map>
#include <algorithm>
#include "Card.h"
#include "../Util/Randint.h"
using namespace std;
/// <summary>
/// card_rule_len: 手牌的个数，该程序手牌6个(0-5)，与画图中1-6手牌对应
/// cardlen3 = 1000， cardlen2 = 100， cardlen1 = 10
/// 这三个变量作用是一个玩家hole card为'2'，flop community cards为'13'，turn community card为'5'
/// 那么该玩家历史手牌信息为'2135'，为了根据手牌构建这个历史手牌信息 hole card * 1000 + flop community cards[0] * 100 + flop community cards[1] * 10 + turn community card
/// </summary>
const int card_rule_len = 6;
const int cardlen3 = 1000;
const int cardlen2 = 100;
const int cardlen1 = 10;
class Deck {
public:
	unsigned char* cards = new unsigned char[card_rule_len];//手牌的长度
	unsigned char cur_index;
	Randint randi;
	Deck() {
		//randi.prngState = 2;
		reset();
	}
	void reset() {		//打乱手牌然后发牌
		cur_index = 0;
		for (int i = 0; i < card_rule_len; i++)
			cards[i] = i;
		struct timeval ti;
		gettimeofday(&ti, NULL);
		srand((unsigned)(ti.tv_sec + ti.tv_usec));
		for (int i = 0; i < card_rule_len; i++)
			swap(cards[i], cards[rand() % card_rule_len]);
		for (int i = 0; i < card_rule_len; i++)
			swap(cards[i], cards[rand() % card_rule_len]);
	}
	/*void println() {
		cout << "deck index:" << cur_index << endl;
	}*/
	unsigned char deal_one_card() {
		return cards[cur_index++];
	}
};