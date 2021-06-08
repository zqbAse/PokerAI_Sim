#pragma once
#include <algorithm>
#include "Table.h"
#include "Engine.h"
using namespace std;
/// <summary>
/// card_rule_len: 手牌的个数，该程序手牌6个(0-5)，与画图中1-6手牌对应
/// cardlen3 = 1000， cardlen2 = 100， cardlen1 = 10
/// 这三个变量作用是一个玩家hole card为'2'，flop community cards为'13'，turn community card为'5'
/// 那么该玩家历史手牌信息为'2135'，为了根据手牌构建这个历史手牌信息 hole card * 1000 + flop community cards[0] * 100 + flop community cards[1] * 10 + turn community card
/// </summary>
extern const int card_rule_len;
extern const int cardlen3;
extern const int cardlen2;
extern const int cardlen1;
std::map<int, int>::iterator it;
class Pokerstate {
public:
	//计算游戏输赢
	Engine* engine;		

	/// mapcluster: 是在chance node时候根据公共牌映射当前手牌属于哪一个类别.
	/// 举例说明：假设player1 hole card为'2',在prelop结束后发flop community cards,这个 community cards可能是'13','14','15','16','34','35','36','45'....（从小到大排序）当中任何一种
	/// 假设发出的手牌为'13'那么加上玩家的hole card得到的历史手牌为'213'那么mapcluster[213] = 0, mapcluster[215] = 2, mapcluster[234] = 4
	map<int, int>* mapcluster;

	//has_allin：是否有人allin，first_action_of_current_round判断是否有人做动作，主要区别第一个call是否是check
	bool has_allin, first_action_of_current_round;
	PokerTable table;

	//betting_stage：当前所处的game round,其中0：preflop，1：flop，2：turn
	//player_i_index：当前需要做动作的玩家id
	short betting_stage, player_i_index;
	//last_raise：上个player的raise筹码个数；n_raises:当前round有几个raise；small_blind：小盲个数id；big_blind：大盲个数；last_bigbet:上个玩家投入筹码数 
	int last_raise, n_raises, small_blind, big_blind, last_bigbet;

	Pokerstate(PokerTable _table, Engine* _engine, map<int, int>* _mapcluster,int _small_blind = 50, int _big_blind = 100) {
		mapcluster = _mapcluster;
		table = _table;
		engine = _engine;
		small_blind = _small_blind;
		big_blind = _big_blind;
		
	}
	//重置游戏状态，包括重新发players hole card,community cards和重新计算每个玩家在每一轮结合公共牌属于哪一类
	void reset_game_single() {
		reset_game();
		table.deck.reset();
		table.deal_community_cards();
		table.deal_private_cards();
		if ((*mapcluster).empty())
			return;
		for (int i = 0; i < table.playerlen; i++) {
			int tem = (table.players[i].cards.eval_card + 1) * cardlen3;
			it = (*mapcluster).find(tem);
			if (it == (*mapcluster).end()) {
				cout << "find error tem:" << tem << endl;
				throw exception();
			}
			unsigned char cl = (*mapcluster)[tem];
			table.players[i].clusters[0] = cl;
		}
		for (int i = 0; i < table.playerlen; i++) {
			int b1, b2;
			if (table.community_cards[0].eval_card < table.community_cards[1].eval_card) {
				b1 = table.community_cards[0].eval_card + 1;
				b2 = table.community_cards[1].eval_card + 1;
			}
			else {
				b1 = table.community_cards[1].eval_card + 1;
				b2 = table.community_cards[0].eval_card + 1;
			}
			int tem = (table.players[i].cards.eval_card + 1) * cardlen3 + b1 * cardlen2 + b2 * cardlen1;
			it = (*mapcluster).find(tem);
			if (it == (*mapcluster).end()) {
				cout << "find error tem:" << tem << endl;
				throw exception();
			}
			unsigned char cl = (*mapcluster)[tem];
			table.players[i].clusters[1] = cl;
		}
		for (int i = 0; i < table.playerlen; i++) {
			int b1, b2;
			if (table.community_cards[0].eval_card < table.community_cards[1].eval_card) {
				b1 = table.community_cards[0].eval_card + 1;
				b2 = table.community_cards[1].eval_card + 1;
			}
			else {
				b1 = table.community_cards[1].eval_card + 1;
				b2 = table.community_cards[0].eval_card + 1;
			}
			int tem = (table.players[i].cards.eval_card + 1) * cardlen3 + b1 * cardlen2 + b2 * cardlen1 + (table.community_cards[2].eval_card + 1);
			it = (*mapcluster).find(tem);
			if (it == (*mapcluster).end()) {
				cout << "find error tem:" << tem << endl;
				throw exception();
			}
			unsigned char cl = (*mapcluster)[tem];
			table.players[i].clusters[2] = cl;
		}
	}
	//重置游戏状态，不需要重新发players hole card,community cards和重新计算每个玩家在每一轮结合公共牌属于哪一类
	void reset_game() {
		table.reset(); //ayns
		betting_stage = 0;//preflop 0,flop 1, turn 2, river 3, shutdown 4, terminal 5
		reset_betting_round_state();
		has_allin = false;
		table._assign_blinds(small_blind, big_blind);
		last_bigbet = big_blind;
		//while (!history.empty()) history.pop_back();
	}
	void move_to_next_player() {
		player_i_index ^= 1;
	}
	void reset_betting_round_state() {
		last_raise = 0;//every round last raise
		player_i_index = 0; //current action player id
		n_raises = 0;//current raise time
		first_action_of_current_round = false; // flag check
	}
	int is_terminal() {
		if (betting_stage != 3 && betting_stage != 4)
			return false;
		else
			return true;
	}
	void compute_payout() {
		for (int i = 0; i < table.playerlen; i++) {
			if (table.players[i].active)
				table.players[i].add_chips(table.total());
		}
	}
	//计算收益
	int payout(int i) {
		return table.players[i].n_chips - table.players[i].initial_chips;
	}
	//判断游戏是否结束
	bool is_ending() {
		return betting_stage > 2;
	}
	bool apply_action(char actionstr) {		//执行动作actionstr
		if (actionstr == 'l') {
			int n_chips_to_call = last_bigbet - table.players[player_i_index].n_bet_chips();
			table.players[player_i_index].call(n_chips_to_call);//last max bet chips - self bet chips = need to call chips
			table.Add_pot(n_chips_to_call);
		}
		else if (actionstr == 'd') {
			betting_stage = 4;
			table.players[player_i_index].active = false;
		}
		else if (actionstr == 'n') {
			int n_chips = table.players[player_i_index].n_chips;
			table.players[player_i_index].raise_to(n_chips);
			table.Add_pot(n_chips);
			has_allin = true;
			last_bigbet = table.players[player_i_index].initial_chips;
			n_raises++;
		}
		else {
			cout << "action:" << actionstr << ",not exist the action" << endl;
			throw exception();
		}

		move_to_next_player();
		if (betting_stage == 4) {
			compute_payout();
		}
		else if (actionstr == 'l' && first_action_of_current_round) {
			if (has_allin)
				while (betting_stage != 3)
					increment_stage();
			else {
				increment_stage();
			}
			if (betting_stage == 3)
				engine->compute_winner(table);
			else
				return true;
			
		}
		first_action_of_current_round = true;
		return false;
	}
	//当前round结束，跳到next round
	void increment_stage() {
		if (betting_stage == 0) {
			betting_stage++;
			table.deal_flop();
			reset_betting_round_state();
		}
		else if (betting_stage == 1) {
			betting_stage++;
			table.deal_turn();
			reset_betting_round_state();
		}
		else if (betting_stage == 2) {
			betting_stage++;
			//engine->compute_winner(table);
		}
		else
		{
			cout << "not increment_stage" << endl;
			throw exception();
		}
	}
	//判断当前状态能够采取的动作
	void legal_actions(char* actions) {
		int chips = table.players[player_i_index].n_chips;
		int pot = table.total_pot;
		int n_chips_to_call = last_bigbet - table.players[player_i_index].n_bet_chips();
		//int halfpot = pot / 200 * 100;
		int onepot = pot / 100 * 100;
		int cur = 0;
		if (n_chips_to_call != 0)
			actions[cur++] = 'd'; //fold
		actions[cur++] = 'l'; //call
		if (has_allin == false) {
			if (chips > 0)
				actions[cur++] = 'n'; //allin
		}
		actions[cur] = '\0';

	}
};