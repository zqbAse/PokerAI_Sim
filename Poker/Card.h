#pragma once
#include<iostream>
#include<exception>
#include<string>
using namespace std;
const static char suit[] = "scdh";
const static char rankc[] = "23456789TJQKA";
class Card
{
public:
	unsigned char eval_card;	//0-5��ʾ����id
	Card(unsigned char ecard = 0) {
		this->eval_card = ecard;
	}
};

