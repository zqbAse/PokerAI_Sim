//#pragma once
//
//#include "Node.h"
//#include <fstream>
//
///// <summary>
///// 根据文件数据结构构建博弈树
///// </summary>
//ofstream fout;
//void dfs_write(strategy_node* cnode) {		//save strategy
//	unsigned char len = cnode->action_len;
//	fout.write((char*)&len, sizeof(len));
//	fout.write((char*)&cnode->chanced, sizeof(bool));
//	if (!cnode->chanced && len > 0) {
//		fout.write((const char*)cnode->actionstr, sizeof(unsigned char) * len);
//		fout.write((const char*)cnode->regret, sizeof(double) * len);
//	}
//	else if (cnode->chanced)
//		fout.write((char*)&cnode->chancempde_cards, sizeof(int));
//	for (int i = 0; i < len; i++) {
//		dfs_write((cnode->actions + i));
//	}
//}
//void dump(strategy_node* root) {
//	fout.open("blueprint_policy_sim.dat", ios::out | ios::binary);
//	dfs_write(root);
//	fout.close();
//}
///// <summary>
///// 保存博弈树的策略到文件
///// </summary>
//ifstream fin;
//void bulid_bluestrategy(strategy_node* cnode) {
//	unsigned char len;
//	if (fin.eof())
//		return;
//	fin.read((char*)&len, sizeof(unsigned char));
//	fin.read((char*)&cnode->chanced, sizeof(bool));
//	if (len <= 0) {
//		cnode->action_len = 0;
//		return;
//	}
//	cnode->action_len = (int)len;
//	cnode->actions = new strategy_node[cnode->action_len]();
//	if (!cnode->chanced) {
//		cnode->actionstr = new char[cnode->action_len];
//		cnode->regret = new double[cnode->action_len];
//		fin.read((char*)cnode->actionstr, sizeof(char) * cnode->action_len);
//		fin.read((char*)cnode->regret, sizeof(double) * cnode->action_len);
//	}
//	else
//		fin.read((char*)&cnode->chancempde_cards, sizeof(int));
//
//	for (int i = 0; i < len; i++) {
//		bulid_bluestrategy((cnode->actions + i));
//	}
//}
//void load(strategy_node* root) {
//	fin.open("blueprint_policy_sim.dat", ios::in | ios::binary);
//	bulid_bluestrategy(root);
//	fin.close();
//}