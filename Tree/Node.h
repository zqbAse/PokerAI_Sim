#pragma once
#include <iostream>
#include <string.h>
using namespace std;

struct strategy_node {		//info set node
	int playerid;	//playerid: 0��ʾ0����ң�1��ʾ1�����
	int bestresponse; //����ҵ�bestreponseֵ	
	bool chanced;		//��ǰ�ڵ��Ƿ�Ϊchance noed
	int chancempde_cards;	
	int action_len = 0;		//��ǰ��Ϣ����legal action�ĳ��Ȼ���chance node�ڵ��child node����
	char* actionstr;		//player �����ڵ�Ķ������ϣ�����'d'(flod),'l'(call),'n'(allin)
	double* regret;			//player �����ڵ�Ķ�����Ӧ�ĺ��ֵ����
	//int* ave_strategy;
	strategy_node* actions;	//��ǰ��Ϣ�������������ߣ�chance node���ƺ���һ����Ϣ���Ľڵ㼯��
	strategy_node() :action_len(0), chanced(false) {}
	strategy_node* findnode(char action) {		//Ѱ��ִ�ж���action�����һ����Ϣ���Ľڵ�
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
void calculate_strategy(double* regret, int len, double sigma[]) {		//���ݶ������ĺ��ֵ����sigma[a]
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