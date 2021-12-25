#include <assert.h>
#include "Table.h"
#include <vector>
#include<algorithm>
using namespace std;

class Engine {
public:
	Engine() { }

	unsigned char Maxstrength(int hand, int community) {		//找最大hand strength
		unsigned char strength = 100;
		if ((hand % 3) == (community % 3))
			return (hand % 3) + 3;
		else
			return (hand % 3) > (community % 3) ? (hand % 3) : (community % 3);
	}
	void compute_winner(LeducPokerTable& table) {//计算玩家输赢

		int strength1 = Maxstrength(table.players[0].cards.eval_card, table.community_cards.eval_card);
		int strength2 = Maxstrength(table.players[1].cards.eval_card, table.community_cards.eval_card);
		assert(table.total() == table.total_pot);
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