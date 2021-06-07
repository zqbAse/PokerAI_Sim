#include <assert.h>
#include "Table.h"
#include <vector>
#include<algorithm>
using namespace std;

class Engine {
public:
	//PokerTable *table;
	Engine() { }
		

	void sortp(unsigned char evalcards[], int len) {
		for (int i = 0; i < len - 1; i++) {
			for (int j = 0; j < len - 1 - i; j++) {
				if (evalcards[j] > evalcards[j + 1]) {
					unsigned temp = evalcards[j];
					evalcards[j] = evalcards[j + 1];
					evalcards[j + 1] = temp;
				}
			}
		}
	}
	unsigned char Maxstrength(Card hand, Card community[]) {
		unsigned char cards[4] = { hand.eval_card ,community[0].eval_card ,community[1].eval_card ,community[2].eval_card };
		int len = 4;
		sortp(cards, len);
		unsigned char strength = 100;
		for (int i = 0; i < len; i++) {
			int tem = i ? 0: 1;
			int cur = cards[tem];
			for ( ++tem ; tem < len; tem++) {
				if (i != tem && cur + 1 == cards[tem])
					cur = cards[tem];
				else
					break;
			}
			if (tem == len)
				return cards[len - 1] + 5;
		}
		return cards[len - 1];
	}
	unsigned char Maxstrength(int hand, int community[]) {
		unsigned char cards[4] = { hand ,community[0] ,community[1] ,community[2] };
		int len = 4;
		sortp(cards, len);
		unsigned char strength = 100;
		for (int i = 0; i < len; i++) {
			int tem = i ? 0 : 1;
			int cur = cards[tem];
			for (++tem; tem < len; tem++) {
				if (i != tem){
					if (cur + 1 == cards[tem])
						cur = cards[tem];
					else
						break;
				}
			}
			if (tem == len)
				return cur + 5;
		}
		return cards[len - 1];
	}
	void compute_winner(PokerTable& table) {
		if (table.community_cards_number != 3) {
			cout << "community_cards_number error" << table.community_cards_number << endl;
			throw exception();
		}
		int strength1 = Maxstrength(table.players[0].cards, table.community_cards);
		int strength2 = Maxstrength(table.players[1].cards, table.community_cards);
		if (strength1 > strength2)
			table.players[0].add_chips(table.total());
		else if (strength1 < strength2)
			table.players[1].add_chips(table.total());
		else {
			table.players[0].n_chips = table.players[0].initial_chips;
			table.players[1].n_chips = table.players[1].initial_chips;
		}
	}

};
