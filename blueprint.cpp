#include <iostream>
#include "Tree/Exploitability.h"
#include "Tree/Bulid_Tree.h"
#include "Tree/Visualize_Tree.h"
#include <random>
#include <map>
using namespace std;

extern const int card_rule_len;
extern const int cardlen3;
extern const int cardlen2;
extern const int cardlen1;

extern map<int, int> mapcluster;
double mccfrp(strategy_node* cnode[], Pokerstate state, int pi, int c, mt19937_64 &_rng_gen);
double mccfr(strategy_node* cnode[], Pokerstate& state, int pi, mt19937_64 &_rng_gen);

double mccfrp(strategy_node* cnode[],Pokerstate state,int pi,int c, mt19937_64 &_rng_gen) {		 //pruning mccfr
	int ph = state.player_i_index;
	if (state.is_ending()) {
		cnode[0]->action_len = 0;
		cnode[1]->action_len = 0;
		return state.payout(pi);
	}
	else if (ph == pi) {
		double sigma[4];
		double vo = 0;

		calculate_strategy(cnode[ph]->regret, cnode[ph]->action_len, sigma);
		int len = cnode[0]->action_len;
		bool explored[4] = { false };
		int voa[4] = { 0 };
		for (int i = 0; i < len; i++) {
			if (cnode[ph]->regret[i] > c) {
				Pokerstate st2 = state;
				bool is_chance = st2.apply_action(cnode[ph]->actionstr[i]);
				strategy_node* cnode2[2];
				cnode2[0] = cnode[0]->findnode(cnode[ph]->actionstr[i]);
				cnode2[1] = cnode[1]->findnode(cnode[ph]->actionstr[i]);
				if (is_chance) {
					cnode2[0] = (cnode2[0]->actions + st2.table.players[0].clusters[st2.betting_stage]);
					cnode2[1] = (cnode2[1]->actions + st2.table.players[1].clusters[st2.betting_stage]);
				}
				voa[i] = mccfrp(cnode2, st2, pi, _rng_gen);
				explored[i] = true;
				vo += sigma[i] * voa[i];
			}
			else
				explored[i] = false;
		}
		for (int i = 0; i < len; i++)
			if (explored[i])
				cnode[ph]->regret[i] += voa[i] - vo;
		return vo;
	}
	else {

		double sigma[4];
		calculate_strategy(cnode[ph]->regret, cnode[ph]->action_len, sigma);
		double randomc = std::generate_canonical<double, std::numeric_limits<double>::digits>(_rng_gen);
		double ans = sigma[0];
		int acti = 0;
		while (ans < randomc && acti < cnode[ph]->action_len - 1)
			ans += sigma[++acti];
		Pokerstate st2 = state;
		bool is_chance = st2.apply_action(cnode[ph]->actionstr[acti]);
		strategy_node* cnode2[2];
		cnode2[0] = cnode[0]->findnode(cnode[ph]->actionstr[acti]);
		cnode2[1] = cnode[1]->findnode(cnode[ph]->actionstr[acti]);
		if (is_chance) {
			cnode2[0] = (cnode2[0]->actions + st2.table.players[0].clusters[st2.betting_stage]);
			cnode2[1] = (cnode2[1]->actions + st2.table.players[1].clusters[st2.betting_stage]);
		}
		double tmp = mccfrp(cnode2, st2, pi, _rng_gen); //takea_rollout(1, cnode[ph]->actionstr[acti], cnode, state, pi);
		return tmp;
	}
}
double mccfr(strategy_node* cnode[], Pokerstate& state, int pi, mt19937_64 &_rng_gen) { // mccfr
	int ph = state.player_i_index;
	if (state.is_ending()){
		cnode[0]->action_len = 0;
		cnode[1]->action_len = 0;
		return state.payout(pi);
	}
	else if (ph == pi) {
		double sigma[4];
		double vo = 0;
		calculate_strategy(cnode[ph]->regret, cnode[ph]->action_len, sigma);
		int len = cnode[0]->action_len;
		int voa[4] = { 0 };
		for (int i = 0; i < len; i++) {
			Pokerstate st2 = state;
			bool is_chance = st2.apply_action(cnode[ph]->actionstr[i]);
			strategy_node* cnode2[2];
			cnode2[0] = cnode[0]->findnode(cnode[ph]->actionstr[i]);
			cnode2[1] = cnode[1]->findnode(cnode[ph]->actionstr[i]);
			if (is_chance) {
				cnode2[0] = (cnode2[0]->actions + st2.table.players[0].clusters[st2.betting_stage]);
				cnode2[1] = (cnode2[1]->actions + st2.table.players[1].clusters[st2.betting_stage]);
			}
			voa[i] = mccfr(cnode2, st2, pi, _rng_gen); //takea_rollout(1, cnode[ph]->actionstr[i], cnode, state, pi);
			vo += sigma[i] * voa[i];
		}
		for (int i = 0; i < len; i++) 
			cnode[ph]->regret[i]+= voa[i] - vo;
		return vo;
	}
	else {
		double sigma[4];
		calculate_strategy(cnode[ph]->regret, cnode[ph]->action_len, sigma);
		double randomc = std::generate_canonical<double, std::numeric_limits<double>::digits>(_rng_gen);
		double ans = sigma[0];
		int acti = 0;
		while (ans < randomc && acti < cnode[ph]->action_len - 1)
			ans += sigma[++acti];
		Pokerstate st2 = state;
		bool is_chance = st2.apply_action(cnode[ph]->actionstr[acti]);
		strategy_node* cnode2[2];
		cnode2[0] = cnode[0]->findnode(cnode[ph]->actionstr[acti]);
		cnode2[1] = cnode[1]->findnode(cnode[ph]->actionstr[acti]);
		if (is_chance) {
			cnode2[0] = (cnode2[0]->actions + st2.table.players[0].clusters[st2.betting_stage]);
			cnode2[1] = (cnode2[1]->actions + st2.table.players[1].clusters[st2.betting_stage]);
		}
		double tmp = mccfr(cnode2, st2, pi, _rng_gen); //takea_rollout(1, cnode[ph]->actionstr[acti], cnode, state, pi);
		return tmp;
	}
}
void dfs_discount(strategy_node* cnode, double d) {
	if (cnode->chanced)
		for (int i = 0; i < cnode->action_len; i++)
			dfs_discount((cnode->actions + i), d);
	else
		for (int i = 0; i < cnode->action_len; i++) {
			cnode->regret[i] *= d;
			dfs_discount((cnode->actions + i), d);
		}
}
const int strategy_interval = 100, n_iterations = 1500000, lcfr_threshold = 1000000, discount_interval = 50;
const int prune_threshold = 200000, c = -200000, n_players = 2, print_iteration = 10, dump_iteration = 100000, update_threshold = 100000;
/*
cd /home/zhouqibin/projects/PokerAI/obj/x64/Debug/
g++ /home/zhouqibin/projects/PokerAI/blueprint.cpp -o /home/zhouqibin/projects/PokerAI/obj/x64/Debug/blueprint.o -std=c++11
nohup ./blueprint.o > blueprint.log 2>&1 & 
tail -fn 50 blueprint.log
ps ax | grep 'gdb'|xargs kill
ps ax | grep 'python main.py'
kill pid
*/

int main() {			//program exit
	mapcluster.clear();
	Player players[] = { Player(0, 400),Player(1, 400) };
	PokerTable table(2,players);
	Engine* engine = new Engine();
	strategy_node* pref[2];
	Pokerstate state(table, engine, &mapcluster);
	state.reset_game_single();
	strategy_node* root = new strategy_node();

	bulid_preflop(root, state);


	Randint randi; 
	for (int t = 1; t <= n_iterations; t++) {
		if (t % 10000 == 0) {
			randi.reset();
			cout << "iter:" << t <<endl;
		}
		mt19937_64 _rng_gen(randi._rand());
		state.reset_game_single();

		for (int i = 0; i < n_players; i++) {
			state.reset_game();
			pref[0] = (root->actions + state.table.players[0].clusters[0]);
			pref[1] = (root->actions + state.table.players[1].clusters[0]);
			mccfr(pref, state, i, _rng_gen);
		}

		if (t < lcfr_threshold && t % discount_interval == 0) {
			double d = ((double)t / discount_interval) / (((double)t / discount_interval) + 1);
			dfs_discount(root, d);
		}
	}
	Sim_State simstate((!0) * 50 + 50);
	cout << exploit(root, 0, simstate) << endl;
	visualization(root, "blueprint_sim.stgy");
	for (int i = 0; i < 2; i++) {
		Sim_State simstate((!i) * 50 + 50);
		cout << exploit(root, i, simstate) << endl;
	}
}
