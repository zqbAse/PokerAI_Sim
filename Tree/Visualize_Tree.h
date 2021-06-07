#pragma once
#include "Node.h"
#include <fstream>

void write2dot(strategy_node* tree, FILE* fw, char his[], int cur)
{
	if (tree->action_len == 0)
		return;
	char ori[115];
	for (int i = 0; i <= cur; i++)
		ori[i] = his[i];
	int ans;
	if (tree->chanced) {
		for (int i = 0; i < tree->action_len; i++) {
			ans = cur;
			if ((tree->actions + i)->action_len > 0) {
				int tp = i + 1;
				if (tp >= 100)
					his[ans++] = tp / 100 + '0', his[ans++] = (tp / 10) % 10 + '0', his[ans++] = tp % 10 + '0';
				else if (tp >= 10)
					his[ans++] = tp / 10 + '0', his[ans++] = tp % 10 + '0';
				else
					his[ans++] = tp + '0';
				his[ans] = '\0';
				fprintf(fw, "%s [label = \"%d\\n%d\" color = red, style = filled];\n", his, tree->actions[i].chancempde_cards, tree->actions[i].bestresponse);
				fprintf(fw, "%s -> %s [label = \"%d\" ];\n", ori, his, tp);

				write2dot((tree->actions + i), fw, his, ans);
			}
		}
	}
	else {
		double sigma[4];
		calculate_strategy(tree->regret, tree->action_len, sigma);
		for (int i = 0; i < tree->action_len; i++) {
			his[cur] = tree->actionstr[i];
			his[cur + 1] = '\0';
			if (0 == tree->actions[i].playerid)
				fprintf(fw, "%s [label = \"%d\" color = black, style = filled];\n", his, tree->actions[i].bestresponse);
			else
				fprintf(fw, "%s [label = \"%d\" color = blue, style = filled];\n", his, tree->actions[i].bestresponse);
			fprintf(fw, "%s -> %s [label = \"%c|%.2lf\" ];\n", ori, his, his[cur], sigma[i] * 100);
			write2dot((tree->actions + i), fw, his, cur + 1);
		}
	}
}
void visualization(strategy_node* tree, const char* filename)
{
	FILE* fw;
	char his[115] = "T";
	if (NULL == (fw = fopen(filename, "w")))
	{
		printf("open file error");
		throw exception();
	}
	fprintf(fw, "digraph\n{\nnode [shape = Mrecord, style = filled, color = black, fontcolor = white];\n");
	fprintf(fw, "%s [color = red, style = filled];\n", his);
	write2dot(tree, fw, his, 1);
	fprintf(fw, "}");
	fclose(fw);
}