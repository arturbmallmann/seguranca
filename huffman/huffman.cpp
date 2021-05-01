#include "huffman.hpp"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
//#include <hash_set>
#include <vector>
#include <map>

using namespace std;



//dic_item * readTree(vector<no_arvore> ar_itens, no_arvore no){
//	readTree(no.left);
//	return no.value;
//}
int main(int argc, char **argv) {
	char * file_name = argv[argc-1];
//	fstream input_file = fstream(file_name);

	fstream dic= fstream("dic.data",ios::binary |ios::in |ios::out);
	fstream input_file;
	input_file.open(file_name,ios::in);
	string line;
	map <char,dic_item> entradas;
	while (getline(input_file,line)){
		for(auto it: line){
			cout<<it;
			cout<<" => ";
			auto fresult = entradas.find(it);
			if(fresult==entradas.end()){
				dic_item value;
				value.c = (char)it;
				value.cod = (char)0;
				value.size = (char)0;
				value.qnt = (char)1;
				entradas.insert(pair<char,dic_item>(it,value));
			}else{
				entradas[it].qnt++;
			}
			cout<<(int)entradas[it].qnt<<endl;
		}
	}
//	sort(mapping.begin(),mapping.end());//,[](const dic_item a, const dic_item b){
//		return a.qnt < b.qnt;
//	});
	array<no_arvore, 1000> itens_arvore; //nós
	array<dic_item, 1000> entries; //nós
	vector<pair<no_arvore,int>> reducao_arvore; //nós
	int tree_count = 0;
	dic_header dh;
	dh.size=entradas.size();
	dh.tree_pointers=sizeof(dic_header)+ entradas.size() * sizeof(dic_item);
	write_file<dic_header>(&dic, &dh);
	int array_count = 0;
	for(auto it:entradas){
		entries[array_count++]=it.second;
		//ar_itens[ar_count++]=it.second;
		no_arvore no;
		no.left=-1;
		no.right=-1;
		no.value=tree_count++;//ponteiros para entradas
		no.qnt=(int)it.second.qnt;
		itens_arvore[array_count]=no;
		reducao_arvore.push_back(pair<no_arvore,int>(no,0));
		write_file<dic_item>(&dic, &it.second);//grava entradas "Entries"
	}

	//for(int i; i<tree_count;i++){
	//dic_item it =ar_itens[i];
//	dic.seekg(sizeof(dic_header),ios::beg);
//	for (no_arvore it:itens_arvore){

//		dic.seekg(sizeof(dic_header)+sizeof(dic_item)*it.value, ios::beg);
//		cout<<"position: "<<dic.tellg()<<endl;
//		dic_item di;
//		dic.read(reinterpret_cast<char *>(&di), sizeof(dic_item));
//		read_file<dic_item>(&dic, &di);
//		dic_item di = ar_itens[it.value];
//		cout<<di.c<<" pointer: "<<it.value<<" qnt="<<(int)it.qnt<<"\n";
	//	dic.write(reinterpret_cast<char *>(&it), sizeof(dic_item)); // esquece..gravar somente depois
//	}
	cout<<"Número de entradas: "<<array_count<<"\n"<<endl;
	int camada=0;
	while(reducao_arvore.size()>1){
		sort(reducao_arvore.begin(),reducao_arvore.end(),[](const pair<no_arvore,int> a, const pair<no_arvore,int> b){
					return a.first.qnt < b.first.qnt;
				});

		no_arvore * novo = (no_arvore*)malloc(sizeof(no_arvore));
		reducao_arvore[0];
		reducao_arvore[1];
		dic_item left=entries[reducao_arvore[0].first.value];
		dic_item right=entries[reducao_arvore[1].first.value];

		novo->left=reducao_arvore[0].first.value;
		entries[reducao_arvore[0].first.value].cod=left.cod<<1;
		novo->right=reducao_arvore[1].first.value;
		entries[reducao_arvore[1].first.value].cod=(right.cod<<1)+1;

		cout<<"Ciclos: "<<(++camada)<<" cod: "<<(int)entries[reducao_arvore[0].first.value].cod;
		cout<<" char: "<<entries[reducao_arvore[1].first.value].c<<endl;

		novo->qnt=reducao_arvore[0].first.qnt + reducao_arvore[1].first.qnt;
		novo->value=-1;//-1 == não nó folha
		reducao_arvore.erase(reducao_arvore.begin());
		reducao_arvore.erase(reducao_arvore.begin());
		reducao_arvore.push_back(pair<no_arvore,int>(*novo,camada));
//		itens[array_count++]=novo;
	}

	dic.seekg(dh.tree_pointers,ios::beg);
	cout<<"pos\ng: "<<dic.tellg()<<" p: "<<dic.tellp()<<endl;
	cout<<"size of dic_item:"<<sizeof(dic_item)<<endl;
	cout<<"size of char:"<<sizeof(char)<<endl;
	dic.close();
}
