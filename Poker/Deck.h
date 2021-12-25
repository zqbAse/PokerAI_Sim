#pragma once
#include <map>
#include <algorithm>
#include "Card.h"
#include "../Util/Randint.h"
using namespace std;
/// <summary>
/// card_rule_len: ���Ƶĸ������ó�������6��(0-5)���뻭ͼ��1-6���ƶ�Ӧ
/// cardlen3 = 1000�� cardlen2 = 100�� cardlen1 = 10
/// ����������������һ�����hole cardΪ'2'��flop community cardsΪ'13'��turn community cardΪ'5'
/// ��ô�������ʷ������ϢΪ'2135'��Ϊ�˸������ƹ��������ʷ������Ϣ hole card * 1000 + flop community cards[0] * 100 + flop community cards[1] * 10 + turn community card
/// </summary>
const int card_rule_len = 6;
const int cardlen3 = 1000;
const int cardlen2 = 100;
const int cardlen1 = 10;
class Deck {
public:
	unsigned char* cards = new unsigned char[card_rule_len];//���Ƶĳ���
	unsigned char cur_index;
	Randint randi;
	Deck() {
		//randi.prngState = 2;
		reset();
	}
	void reset() {		//��������Ȼ����
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