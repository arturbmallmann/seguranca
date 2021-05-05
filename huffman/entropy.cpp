/*
 * entropy.cpp
 *
 *  Created on: 30 de abr. de 2021
 *      Author: artur
 */

#include "entropy.h"
#include <cmath>
#include <iostream>

using namespace std;

double calc_entropy(char * input,int size){
	map<char,int> char_qnt;
	for (int i=0;i<size;i++){
		char c = input[i];
		auto cresult = char_qnt.find(c);
		if(cresult==char_qnt.end())
			char_qnt.insert(pair<char,int>(c,1));
		else
			char_qnt[c]++;
	}
	double h=0.0, prob = 0.0;
	double b_log = log10(char_qnt.size());
//	cout << "caracteres: " << char_qnt.size()<<" tam total: "<<size<<endl;
	for (auto pair_kv: char_qnt){
//		cout << "Char: "<<pair_kv.first<<" qnt: "<<pair_kv.second<<endl;
		prob = (double)pair_kv.second/size;
		h -=  prob * (log10(prob)/b_log) ;
	}
	return h;
}
