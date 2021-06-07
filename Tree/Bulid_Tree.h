#pragma once
#include "Node.h"
#include "../Poker/State.h"


extern std::map<int, int>::iterator it;
map<int, int> mapcluster;
void bulid_subtree(strategy_node* node, Pokerstate state, int cards[7], int community_cards[]);

void bulid_turn(strategy_node* chancenode, Pokerstate state, int cards[4], int community_cards[]) {
	chancenode->playerid = state.player_i_index;
	int cardlen = card_rule_len - 3;
	chancenode->chanced = true;
	chancenode->init_chance_node(cardlen);
	int curi = 0;
	for (int i = 0; i < cardlen; i++) {
		int tem = (community_cards[0] + 1) * cardlen3 + (community_cards[1] + 1) * cardlen2 + (community_cards[2] + 1) * cardlen1 + (cards[i] + 1);
		it = mapcluster.find(tem);
		if (it != mapcluster.end()) {
			cout << "had have tem:" << tem << ",pre:" << mapcluster[tem] << endl;
			throw exception();
		}
		mapcluster[tem] = curi;
		int tempcards[3], cnt = 0;
		for (int t1 = 0; t1 < cardlen; t1++)
			if (cards[t1] != cards[i])
				tempcards[cnt++] = cards[t1];
		assert(cnt == card_rule_len - 4);
		chancenode->actions[curi].chancenode_id = cards[i];
		chancenode->actions[curi].chancempde_cards = (community_cards[0] + 1) * 1000 + (community_cards[1] + 1) * 100 + (community_cards[2] + 1) * 10 + (cards[i] + 1);
		bulid_subtree(chancenode->actions + curi, state, tempcards, community_cards);
		curi++;
	}
}
void bulid_flop(strategy_node* chancenode, Pokerstate state, int cards[7], int community_cards[]) {
	chancenode->playerid = state.player_i_index;
	int cardlen = card_rule_len - 1;
	chancenode->chanced = true;
	chancenode->init_chance_node(cardlen * (cardlen - 1) / 2);
	int curi = 0;
	for (int i = 0; i < cardlen; i++) {
		for (int j = i + 1; j < cardlen; j++) {
			int tem = (community_cards[0] + 1) * cardlen3 + (cards[i] + 1) * cardlen2 + (cards[j] + 1) * cardlen1;
			it = mapcluster.find(tem);
			if (it != mapcluster.end()) {
				cout << "had have tem:" << tem << endl;
				throw exception();
			}
			mapcluster[tem] = curi;
			int tempcards[4], cnt = 0;
			for (int t1 = 0; t1 < cardlen; t1++)
				if (cards[t1] != cards[i] && cards[t1] != cards[j])
					tempcards[cnt++] = cards[t1];
			assert(cnt == card_rule_len - 3);
			chancenode->actions[curi].chancenode_id = cards[i] * 9 + cards[j];
			chancenode->actions[curi].chancempde_cards = (community_cards[0] + 1) * 100 + (cards[i] + 1) * 10 + (cards[j] + 1);
			community_cards[1] = cards[i];
			community_cards[2] = cards[j];
			bulid_subtree(chancenode->actions + curi, state, tempcards, community_cards);
			curi++;

		}
	}
}
void bulid_preflop(strategy_node* root, Pokerstate state) {
	int cardlen = card_rule_len;
	root->chanced = true;
	root->init_chance_node(cardlen);
	int community_cards[6];
	int curi = 0;
	for (int i = 0; i < cardlen; i++) {
		int tem = (i + 1) * cardlen3;
		it = mapcluster.find(tem);
		if (it != mapcluster.end()) {
			cout << "had have tem:" << tem << endl;
			throw exception();
		}
		mapcluster[tem] = curi;
		int tempcards[8], cnt = 0;
		for (int t1 = 0; t1 < cardlen; t1++)
			if (t1 != i)
				tempcards[cnt++] = t1;
		assert(cnt == card_rule_len - 1);
		root->actions[curi].chancenode_id = i;
		root->actions[curi].chancempde_cards = (i + 1);
		community_cards[0] = i;
		bulid_subtree(root->actions + curi, state, tempcards, community_cards);
		curi++;
	}
}
void bulid_subtree(strategy_node* node, Pokerstate state, int cards[7], int community_cards[]) {
	node->playerid = state.player_i_index;
	if (state.is_ending())
		return;
	char* legal_acts = new char[4];
	state.legal_actions(legal_acts);
	node->init_child(legal_acts);
	for (int i = 0; i < node->action_len; i++) {
		Pokerstate newstate = state;
		bool is_chance = newstate.apply_action(node->actionstr[i]);
		if (is_chance) {
			if (newstate.betting_stage == 1) {
				bulid_flop(node->actions + i, newstate, cards, community_cards);
			}
			else if (newstate.betting_stage == 2) {
				bulid_turn(node->actions + i, newstate, cards, community_cards);
			}
			else {
				cout << "chance node error" << endl;
				throw exception();
			}
		}
		else
			bulid_subtree(node->actions + i, newstate, cards, community_cards);
	}
}