
#include <iostream>
#include "Tree/Exploitability.h"
#include "Tree/Bulid_Tree.h"
#include "Tree/Visualize_Tree.h"
#include <random>
using namespace std;

/// <summary>
/// mccfr�㷨
/// </summary>
/// <param name="cnode">�������е�ǰ�ڵ㣬������ǰ����Ϣ��</param>
/// <param name="state">���ݶ������еõ���ǰ��Ϸ��״̬</param>
/// <param name="pi">����pi��ҵĶ������Լ�����pi���ÿ�����������ݶ��ֲ��Բ������ֵĶ���</param>
/// <param name="_rng_gen">���������������Ҫ���ڲ�������������ö��ֵĶ���</param>
/// <returns>counterfactual value</returns>
double mccfr(strategy_node* cnode[], LeducPokerstate& state, int pi, double w, mt19937_64& _rng_gen) { // mccfr
    int ph = state.player_i_index;
    if (state.is_ending()) {// �ж��Ƿ���Ϸ����
        return state.payout(pi);//���㵱ǰpi��ҵ�����
    }
    else if (ph == pi) {
        double sigma[4];
        double vo = 0;			//expected counterfactual value
        calculate_strategy(cnode[ph]->regret, cnode[ph]->action_len, sigma);//���ݵ�ǰ�ڵ�ĺ��ֵ�����ҵ�ǰ�Ĳ���sigma[a]
        int len = cnode[0]->action_len;		//��ǰ��Ϣ�����¿���ִ�еĶ�������
        double voa[4] = { 0 };
        for (int i = 0; i < len; i++) {
            LeducPokerstate st2 = state;
            bool is_chance = st2.apply_action(cnode[ph]->actionstr[i]);//ִ�е�i��legal action�õ���Ϣ��Ϸ״̬��Ϣst2
            strategy_node* cnode2[2];
            //player0ִ�ж�����͵������µ���Ϣ�������ڵ�cnode2[0]
            cnode2[0] = cnode[0]->actions + i;
            //player1ִ�ж�����͵������µ���Ϣ�������ڵ�cnode2[1]
            cnode2[1] = cnode[1]->actions + i;
            //�ж�ִ���궯���Ƿ񵽴�chance node
            if (is_chance) {
                //��������chance node�����ݵ�ǰ��round����������ƣ������ҵ�hole card�͹����Ƶõ�������Ϣ��ʹ��mapcluster���Եõ���ǰ����������һ��
                //st2.table.players[0].clusters[st2.betting_stage]��ʾplayer 0�ڵ�ǰst2.betting_stage ��round������һ�࣬�������pluribus��200����һ�����һ��
                cnode2[0] = (cnode2[0]->actions + st2.table.players[0].clusters[st2.betting_stage]);
                cnode2[1] = (cnode2[1]->actions + st2.table.players[1].clusters[st2.betting_stage]);
            }
            voa[i] = mccfr(cnode2, st2, pi, w, _rng_gen) * w; //��⵱ǰ����u[a]��counterfactual regret value
            vo += sigma[i] * voa[i];			//��ǰ��Ϣ����expected counterfactual value
        }
        for (int i = 0; i < len; i++) {
            if (cnode[ph]->regret[i] > -20000000)
                cnode[ph]->regret[i] += voa[i] - vo;	//���µ�ǰ��Ϣ����ÿ�������ĺ��ֵ
            else
                cnode[ph]->regret[i] = -20000000;
        }
        return vo / w;
    }
    else {
        double sigma[4];
        calculate_strategy(cnode[ph]->regret, cnode[ph]->action_len, sigma); //���ݵ�ǰ�ڵ�ĺ��ֵ�����ҵ�ǰ�Ĳ���sigma[a]
        int len = cnode[ph]->action_len;
        double voa[6] = { 0 };
        double vo = 0;
        strategy_node* cnode2[2];
        for (int i = 0; i < len; i++) {
            if (sigma[i] > 0) {
                LeducPokerstate st2 = state;
                bool is_chance = st2.apply_action(cnode[ph]->actionstr[i]);//ִ�в��������õ���Ϣ��Ϸ״̬��Ϣst2
                //player0ִ�ж�����͵������µ���Ϣ�������ڵ�cnode2[0]
                cnode2[0] = cnode[0]->actions + i;
                //player1ִ�ж�����͵������µ���Ϣ�������ڵ�cnode2[1]
                cnode2[1] = cnode[1]->actions + i;
                if (is_chance) {
                    cnode2[0] = (cnode2[0]->actions + st2.table.players[0].clusters[st2.betting_stage]);
                    cnode2[1] = (cnode2[1]->actions + st2.table.players[1].clusters[st2.betting_stage]);
                }
                voa[i] = mccfr(cnode2, st2, pi, w * sigma[i], _rng_gen); //���㵱ǰ����������counterfactual value
                vo += sigma[i] * voa[i];
            }
        }

        //double randomc = std::generate_canonical<double, std::numeric_limits<double>::digits>(_rng_gen);//���������
        //double ans = sigma[0];
        //int acti = 0;
        //while (ans < randomc && acti < cnode[ph]->action_len - 1)//��������actiΪ��ǰlegal action�в�������acti������
        //    ans += sigma[++acti];
        //Pokerstate st2 = state;
        //bool is_chance = st2.apply_action(cnode[ph]->actionstr[acti]);//ִ�в��������õ���Ϣ��Ϸ״̬��Ϣst2
        //strategy_node* cnode2[2];
        ////player0ִ�ж�����͵������µ���Ϣ�������ڵ�cnode2[0]
        //cnode2[0] = cnode[0]->findnode(cnode[ph]->actionstr[acti]);
        ////player1ִ�ж�����͵������µ���Ϣ�������ڵ�cnode2[1]
        //cnode2[1] = cnode[1]->findnode(cnode[ph]->actionstr[acti]);
        //if (is_chance) {
        //    //��������chance node�����ݵ�ǰ��round����������ƣ������ҵ�hole card�͹����Ƶõ�������Ϣ��ʹ��mapcluster���Եõ���ǰ����������һ��
        //    //st2.table.players[0].clusters[st2.betting_stage]��ʾplayer 0�ڵ�ǰst2.betting_stage ��round������һ�࣬�������pluribus��200����һ�����һ��
        //    cnode2[0] = (cnode2[0]->actions + st2.table.players[0].clusters[st2.betting_stage]);
        //    cnode2[1] = (cnode2[1]->actions + st2.table.players[1].clusters[st2.betting_stage]);
        //}
        //double tmp = mccfr(cnode2, st2, pi, w * sigma[acti], _rng_gen); //���㵱ǰ����������counterfactual value
        return vo;
    }
}
/// <summary>
/// Linear MCCFR discount function���Բ�������ÿ���ڵ���ÿ���������ֵdiscount d
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
/// n_iterations: ��������
/// lcfr_threshold: ����ǰlcfr_threshold����ʹ��Linear MCCFR��discount regret value
/// discount_interval: ÿ���discount_interval�������ĵ��������������Ų�����discount regret value
/// prune_threshold: lazy mccfr���ڵ���prune_threshold��������95%����ȥʹ��prune mccfr���²��ԣ������Ժ��ֵС��c = -2000000�Ľڵ���б�������
/// n_players: ��Ҹ���
/// dump_iteration: ÿ����dump_iteration�������һ�β������Ĳ���
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
/// ѵ���߼���
/// һ�����ȳ�ʼ����Ϸ״̬������shuffle deck�� ��ʼ����ҳ���chips=initial��pot = 0����Ϣ
/// Ȼ���ÿ����ҷ�hole card����ǰ����1�Ź����ƣ�������Ϸֻ�е�����Ӧ��round�Żῴ�õ�������
/// ����������ǰ�����ÿ����ҵ�flop�ֻ���turn��ʱ������������һ�࣬����ʵ���ϵ���һ�ֲſ�ʼ����Ȼ���������������һ�ࡣ��ǰ����ù�����������һ�ൽ����round��ֱ��ʹ�á�
/// �������ݹ����õĲ��������ò�������ÿ���ڵ��Ѿ�������ǰ��Ϣ���¿��Բ�ȡ�Ķ�����
/// ��ôֻҪ����������������Ϸ״̬��ֱ����Ϸ������Ȼ�������Ϸ������棬�ٸ���·���϶����ĺ��ֵ��
/// </summary>
/// <returns></returns>

int main() {			//program exit
    ///��ʼ����Ϸ״̬��Ϣstate
    //��Ϸ������������� 0�������400���룬1�������400����
    Player players[] = { Player(0, 10000),Player(1, 10000) };
    LeducPokerTable table(2, players);
    Engine* engine = new Engine();	//��Ϸ���棬Ϊ�˼�����Ϸ�����Ӯ
    LeducPokerstate state(table, engine);
    state.reset_game_single();

    //pref[0]��ʾplayer0��������Ϣ��
    //pref[1]��ʾplayer1��������Ϣ��
    strategy_node* pref[2];

    //�������ڵ�
    strategy_node* root = new strategy_node();

    //�������Ų�����
    bulid_leduc(root, state, 6);

    srand(time(NULL));
    Randint randi; //���������������Ҫ�����������
    for (int t = 1; t <= n_iterations; t++) {
        if (t % 10000 == 0) {	//ÿ���10000�����������������
            randi.reset(rand());
            cout << "iter:" << t << endl;
        }
        mt19937_64 _rng_gen(randi._rand());	//����c����srand(time(0))������������Ӳ����������
        state.reset_game_single();			//������Ϸ״̬���������·�players hole card,community cards�����¼���ÿ�������ÿһ�ֽ�Ϲ�����������һ��

        for (int i = 0; i < n_players; i++) {
            state.reset_game();				//������Ϸ״̬������Ҫ���·�players hole card,community cards�����¼���ÿ�������ÿһ�ֽ�Ϲ�����������һ��
            //��ǰΪchance node������ÿ��player��hole card��ʹ��mapcluster���Եõ���ǰ����������һ��,����pluribus��һ����1326����𣬵�ǰ������������һ��
            //state.table.players[0].clusters[0]��ʾplayer 0��prflop(��0��round)����������һ�࣬�������pluribus�е�һ����1326�������һ��
            pref[0] = (root->actions + state.table.players[0].clusters[0]);
            pref[1] = (root->actions + state.table.players[1].clusters[0]);
            mccfr(pref, state, i, 1.0, _rng_gen);
        }
        if (t > update_threshold && t % 100 == 0)
            update_strategy(root);
        //һ�����ʹ��Linear MCCFR to discount regret value
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
            //��������ö�
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