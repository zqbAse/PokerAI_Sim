#pragma once
#include "Node.h"
#include "../Poker/State.h"

/// <summary>
/// 构建玩家动作节点的action node and chance node
/// </summary>
/// <param name="node">current node</param>
/// <param name="state">current state</param>
/// <param name="len">len is constant 6, which mean 6 cards</param>
void bulid_leduc_subtree(strategy_node* node, LeducPokerstate state, int len) {
	if (state.is_terminal())
		return;
	char* legal_acts = new char[4];
	state.legal_actions(legal_acts);
	node->init_child(legal_acts);
	for (int i = 0; i < node->action_len; i++) {
		LeducPokerstate newstate = state;
		bool is_chance = newstate.apply_action(node->actionstr[i]);
		node->actions[i].chancempde_cards = node->chancempde_cards;
		if (is_chance) {
			node->actions[i].chanced = true;
			node->actions[i].init_chance_node(len);
			if (newstate.betting_stage == 1) {
				for (int j = 0; j < len; j++)
					if (j != node->actions[i].chancempde_cards)
						bulid_leduc_subtree(node->actions[i].actions + j, newstate, len);
			}
			else {
				cout << "chance node error" << endl;
				throw exception();
			}
		}
		else {
			bulid_leduc_subtree(node->actions + i, newstate, len);
		}
	}
}
/// <summary>
/// 构建玩家动作节点的chance node
/// </summary>
/// <param name="node">current node</param>
/// <param name="state">current state</param>
/// <param name="len">len is constant 6, which mean 6 cards</param>
void bulid_leduc(strategy_node* root, LeducPokerstate state, int len) {
	root->chanced = true;
	root->init_chance_node(len);
	for (int i = 0; i < len; i++) {
		root->actions[i].chancempde_cards = i;
		bulid_leduc_subtree(root->actions + i, state, len);
	}
}