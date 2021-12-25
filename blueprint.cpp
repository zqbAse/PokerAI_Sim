
#include <iostream>
#include "Tree/Exploitability.h"
#include "Tree/Bulid_Tree.h"
#include "Tree/Visualize_Tree.h"
#include <random>
using namespace std;

/// <summary>
/// mccfr算法
/// </summary>
/// <param name="cnode">博弈树中当前节点，它代表当前的信息集</param>
/// <param name="state">根据动作序列得到当前游戏的状态</param>
/// <param name="pi">更新pi玩家的动作策略即便利pi玩家每个动作，根据对手策略采样对手的动作</param>
/// <param name="_rng_gen">随机数产生器，主要用于产生随机数来采用对手的动作</param>
/// <returns>counterfactual value</returns>
double mccfr(strategy_node* cnode[], LeducPokerstate& state, int pi, double w, mt19937_64& _rng_gen) { // mccfr
    int ph = state.player_i_index;
    if (state.is_ending()) {// 判断是否游戏结束
        return state.payout(pi);//计算当前pi玩家的收益
    }
    else if (ph == pi) {
        double sigma[4];
        double vo = 0;			//expected counterfactual value
        calculate_strategy(cnode[ph]->regret, cnode[ph]->action_len, sigma);//根据当前节点的后悔值获得玩家当前的策略sigma[a]
        int len = cnode[0]->action_len;		//当前信息集合下可以执行的动作长度
        double voa[4] = { 0 };
        for (int i = 0; i < len; i++) {
            LeducPokerstate st2 = state;
            bool is_chance = st2.apply_action(cnode[ph]->actionstr[i]);//执行第i个legal action得到信息游戏状态信息st2
            strategy_node* cnode2[2];
            //player0执行动作后就到达了新的信息集，即节点cnode2[0]
            cnode2[0] = cnode[0]->actions + i;
            //player1执行动作后就到达了新的信息集，即节点cnode2[1]
            cnode2[1] = cnode[1]->actions + i;
            //判断执行完动作是否到达chance node
            if (is_chance) {
                //如果到达的chance node，根据当前的round随机公共发牌，结合玩家的hole card和公共牌得到手牌信息再使用mapcluster可以得到当前手牌属于哪一类
                //st2.table.players[0].clusters[st2.betting_stage]表示player 0在当前st2.betting_stage 的round属于哪一类，这个类与pluribus中200类中一类概念一样
                cnode2[0] = (cnode2[0]->actions + st2.table.players[0].clusters[st2.betting_stage]);
                cnode2[1] = (cnode2[1]->actions + st2.table.players[1].clusters[st2.betting_stage]);
            }
            voa[i] = mccfr(cnode2, st2, pi, w, _rng_gen) * w; //求解当前动作u[a]的counterfactual regret value
            vo += sigma[i] * voa[i];			//求当前信息集下expected counterfactual value
        }
        for (int i = 0; i < len; i++) {
            if (cnode[ph]->regret[i] > -20000000)
                cnode[ph]->regret[i] += voa[i] - vo;	//更新当前信息集下每个动作的后悔值
            else
                cnode[ph]->regret[i] = -20000000;
        }
        return vo / w;
    }
    else {
        double sigma[4];
        calculate_strategy(cnode[ph]->regret, cnode[ph]->action_len, sigma); //根据当前节点的后悔值获得玩家当前的策略sigma[a]
        int len = cnode[ph]->action_len;
        double voa[6] = { 0 };
        double vo = 0;
        strategy_node* cnode2[2];
        for (int i = 0; i < len; i++) {
            if (sigma[i] > 0) {
                LeducPokerstate st2 = state;
                bool is_chance = st2.apply_action(cnode[ph]->actionstr[i]);//执行采样动作得到信息游戏状态信息st2
                //player0执行动作后就到达了新的信息集，即节点cnode2[0]
                cnode2[0] = cnode[0]->actions + i;
                //player1执行动作后就到达了新的信息集，即节点cnode2[1]
                cnode2[1] = cnode[1]->actions + i;
                if (is_chance) {
                    cnode2[0] = (cnode2[0]->actions + st2.table.players[0].clusters[st2.betting_stage]);
                    cnode2[1] = (cnode2[1]->actions + st2.table.players[1].clusters[st2.betting_stage]);
                }
                voa[i] = mccfr(cnode2, st2, pi, w * sigma[i], _rng_gen); //计算当前采样动作的counterfactual value
                vo += sigma[i] * voa[i];
            }
        }

        //double randomc = std::generate_canonical<double, std::numeric_limits<double>::digits>(_rng_gen);//产生随机数
        //double ans = sigma[0];
        //int acti = 0;
        //while (ans < randomc && acti < cnode[ph]->action_len - 1)//采样动作acti为当前legal action中采样到第acti个动作
        //    ans += sigma[++acti];
        //Pokerstate st2 = state;
        //bool is_chance = st2.apply_action(cnode[ph]->actionstr[acti]);//执行采样动作得到信息游戏状态信息st2
        //strategy_node* cnode2[2];
        ////player0执行动作后就到达了新的信息集，即节点cnode2[0]
        //cnode2[0] = cnode[0]->findnode(cnode[ph]->actionstr[acti]);
        ////player1执行动作后就到达了新的信息集，即节点cnode2[1]
        //cnode2[1] = cnode[1]->findnode(cnode[ph]->actionstr[acti]);
        //if (is_chance) {
        //    //如果到达的chance node，根据当前的round随机公共发牌，结合玩家的hole card和公共牌得到手牌信息再使用mapcluster可以得到当前手牌属于哪一类
        //    //st2.table.players[0].clusters[st2.betting_stage]表示player 0在当前st2.betting_stage 的round属于哪一类，这个类与pluribus中200类中一类概念一样
        //    cnode2[0] = (cnode2[0]->actions + st2.table.players[0].clusters[st2.betting_stage]);
        //    cnode2[1] = (cnode2[1]->actions + st2.table.players[1].clusters[st2.betting_stage]);
        //}
        //double tmp = mccfr(cnode2, st2, pi, w * sigma[acti], _rng_gen); //计算当前采样动作的counterfactual value
        return vo;
    }
}
/// <summary>
/// Linear MCCFR discount function，对博弈树中每个节点中每个动作后悔值discount d
/// </summary>
/// <param name="cnode">current discounted node</param>
/// <param name="d">discount factor d</param>
void dfs_discount(strategy_node* cnode, double d) {
    if (cnode->chanced)	//chance node don't contain regret value
        for (int i = 0; i < cnode->action_len; i++)
            dfs_discount((cnode->actions + i), d);
    else
        for (int i = 0; i < cnode->action_len; i++) {
            cnode->regret[i] *= d;
            cnode->ave_strategy[i] *= d;
            dfs_discount((cnode->actions + i), d);
        }
}
/// <summary>
/// update average strategy
/// </summary>
/// <param name="treenode">current node</param>
/// <param name="pi">update pi average strategy</param>
/// <param name="curplayer">curplayer is that current state need "curplayer" player to take action.</param>
/// <param name="_rng_gen">_rng_gen is a generator of random number</param>
void update_strategy(strategy_node* treenode) {
    if (treenode->action_len == 0)
        return;
    if (treenode->chanced) {
        for (int i = 0; i < treenode->action_len; i++)
            update_strategy(treenode->actions + i);
        return;
    }
    else{
        double sigma[15];
        calculate_strategy(treenode->regret, treenode->action_len, sigma);
        for (int i = 0; i < treenode->action_len; i++) {
            if (sigma[i] > 0)
                treenode->ave_strategy[i] += sigma[i];
            update_strategy(treenode->actions + i);
        }
    }

}
/// <summary>
/// n_iterations: 迭代次数
/// lcfr_threshold: 迭代前lcfr_threshold次数使用Linear MCCFR来discount regret value
/// discount_interval: 每间隔discount_interval这个区间的迭代次数来对整颗博弈树discount regret value
/// prune_threshold: lazy mccfr，在迭代prune_threshold次数后有95%概率去使用prune mccfr更新策略，它不对后悔值小于c = -2000000的节点进行便利更新
/// n_players: 玩家个数
/// dump_iteration: 每迭代dump_iteration间隔保存一次博弈树的策略
/// </summary>
const int n_iterations = 10000000, lcfr_threshold = 10000000, discount_interval = 100, update_threshold = 10000;
const int prune_threshold = 200000, c = -20000000, n_players = 2, dump_iteration = 100000;

/*
cd /home/zhouqibin/projects/PokerAI/obj/x64/Debug/
g++ /home/zhouqibin/projects/PokerAI/blueprint.cpp -o /home/zhouqibin/projects/PokerAI/obj/x64/Debug/blueprint.o -std=c++11
nohup ./blueprint.o > blueprint.log 2>&1 &
tail -fn 50 blueprint.log
ps ax | grep 'gdb'|xargs kill
ps ax | grep 'python main.py'
kill pid
*/

/// <summary>
/// 训练逻辑：
/// 一：首先初始化游戏状态，包括shuffle deck， 初始化玩家筹码chips=initial，pot = 0等信息
/// 然后给每个玩家发hole card和提前发完1张公共牌，但是游戏只有到达相应的round才会看得到公共牌
/// 这样就能提前计算好每个玩家到flop轮或者turn轮时候手牌属于哪一类，不用实际上到那一轮才开始发牌然后计算手牌属于哪一类。提前计算好公共牌属于哪一类到到达round后直接使用。
/// 二：根据构建好的博弈树，该博弈树在每个节点已经包含当前信息集下可以采取的动作。
/// 那么只要遍历动作，更新游戏状态，直到游戏结束，然后计算游戏结果收益，再更新路径上动作的后悔值。
/// </summary>
/// <returns></returns>

int main() {			//program exit
    ///初始化游戏状态信息state
    //游戏桌面有两个玩家 0号玩家有400筹码，1号玩家有400筹码
    Player players[] = { Player(0, 10000),Player(1, 10000) };
    LeducPokerTable table(2, players);
    Engine* engine = new Engine();	//游戏引擎，为了计算游戏结果输赢
    LeducPokerstate state(table, engine);
    state.reset_game_single();

    //pref[0]表示player0所处的信息集
    //pref[1]表示player1所处的信息集
    strategy_node* pref[2];

    //创建根节点
    strategy_node* root = new strategy_node();

    //构建整颗博弈树
    bulid_leduc(root, state, 6);

    srand(time(NULL));
    Randint randi; //随机数产生器，主要生成随机种子
    for (int t = 1; t <= n_iterations; t++) {
        if (t % 10000 == 0) {	//每间隔10000次重置随机数产生器
            randi.reset(rand());
            cout << "iter:" << t << endl;
        }
        mt19937_64 _rng_gen(randi._rand());	//类似c语言srand(time(0))，根据随机种子产生随机引擎
        state.reset_game_single();			//重置游戏状态，包括重新发players hole card,community cards和重新计算每个玩家在每一轮结合公共牌属于哪一类

        for (int i = 0; i < n_players; i++) {
            state.reset_game();				//重置游戏状态，不需要重新发players hole card,community cards和重新计算每个玩家在每一轮结合公共牌属于哪一类
            //当前为chance node，根据每个player的hole card，使用mapcluster可以得到当前手牌属于哪一类,类似pluribus第一轮有1326个类别，当前手牌属于其中一类
            //state.table.players[0].clusters[0]表示player 0在prflop(第0个round)手牌属于哪一类，这个类与pluribus中第一轮有1326个类概念一样
            pref[0] = (root->actions + state.table.players[0].clusters[0]);
            pref[1] = (root->actions + state.table.players[1].clusters[0]);
            mccfr(pref, state, i, 1.0, _rng_gen);
        }
        if (t > update_threshold && t % 100 == 0)
            update_strategy(root);
        //一定间隔使用Linear MCCFR to discount regret value
        if (t < lcfr_threshold && t % discount_interval == 0) {
            double d = ((double)t / discount_interval) / (((double)t / discount_interval) + 1);
            dfs_discount(root, d);
        }
        if (t % 2000000 == 0) {
            visualization(root, "blueprint_sim.stgy");// tree visualization
            strategy_node* nodes[6];
            double range[6];
            for (int i = 0; i < card_rule_len; i++)
                nodes[i] = root->actions + i, range[i] = 1;
            //计算可利用度
            for (int i = 0; i < 2; i++) {
                double tot = 0;
                double ex6[6];
                for (int j = 0; j < 6; j++) {
                    double cfvs[6];
                    range[j] = 0;
                    exploit(nodes, i, state, range, cfvs, j, card_rule_len);//best response
                    ex6[j] = 0;
                    for (int k = 0; k < card_rule_len; k++)
                        ex6[j] += cfvs[k];
                    tot += ex6[j];
                    range[j] = 1;
                }

                cout << tot / 6.0 << endl;
            }
        }
    }

}