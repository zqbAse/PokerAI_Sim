#pragma once
#include <iostream>
#include <string.h>
using namespace std;

struct strategy_node {		//info set node
	int playerid;	//playerid: 0表示0号玩家，1表示1号玩家
	int bestresponse; //求玩家的bestreponse值	
	bool chanced;		//当前节点是否为chance noed
	int chancempde_cards;	
	int action_len = 0;		//当前信息集下legal action的长度或者chance node节点的child node个数
	char* actionstr;		//player 动作节点的动作集合，其中'd'(flod),'l'(call),'n'(allin)
	double* regret;			//player 动作节点的动作对应的后悔值集合
	//int* ave_strategy;
	strategy_node* actions;	//当前信息集下做动作或者，chance node发牌后下一个信息集的节点集合
	strategy_node() :action_len(0), chanced(false) {}
	strategy_node* findnode(char action) {		//寻找执行动作action后的下一个信息集的节点
		int i = 0;
		for (; i < action_len; i++) {
			if (*(actionstr + i) == action)
				break;
		}
		return (actions + i);
	}
	void init_child(char* action_str) {		
		if (action_len == 0) {
			actionstr = action_str;
			int aclen = strlen(action_str);
			actions = new strategy_node[aclen]();
			regret = new double[aclen] {0};
			action_len = aclen;
		}
	}
	void init_chance_node(int initlen) {
		if (action_len == 0) {
			actions = new strategy_node[initlen]();
			//regret = new int[initlen] {0};
			action_len = initlen;
		}
	}
};
/*void calculate_strategy(int* regret, int len, double sigma[]) {
	double sum = 0;
	for (int i = 0; i < len; i++) {
		if (regret[i] > 0)
			sum += regret[i];
	}
	if (sum > 0)
		for (int i = 0; i < len; i++)
			if (regret[i] > 0)
				sigma[i] = regret[i] / sum;
			else
				sigma[i] = 0;
	else
		for (int i = 0; i < len; i++)
			sigma[i] = 1.0 / len;
}*/
void calculate_strategy(double* regret, int len, double sigma[]) {		//根据动作集的后悔值计算sigma[a]
	double sum = 0;
	for (int i = 0; i < len; i++) {
		if (regret[i] > 0)
			sum += regret[i];
	}
	if (sum > 0)
		for (int i = 0; i < len; i++)
			if (regret[i] > 0)
				sigma[i] = regret[i] / sum;
			else
				sigma[i] = 0;
	else
		for (int i = 0; i < len; i++)
			sigma[i] = 1.0 / len;
}