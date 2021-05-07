#include "huffman.hpp"
// C
#include <string.h>
// C++
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
//#include <hash_set>
#include <vector>
#include <map>
#include "entropy.h"

using namespace std;
void flush_bit (bitstrm * bstrm){
	write_file<char>((fstream *)bstrm->strm, &bstrm->cache);
//	printf("Bit cache %x Flushed!\n",(int) bstrm->cache);
	bstrm->cache=0;
	bstrm->control=0;
}

void write_bit (bitstrm * bstrm,char bit){
	bit = bit & 0x1;
	bstrm->cache = bstrm->cache>>1 & 0x7f; //zero no mais significativo
	bstrm->cache += (char)(bit<<7);
	bstrm->control++;
	if(bstrm->control == 8){
		flush_bit(bstrm);
	}
}

//dic_item * readTree(vector<no_arvore> ar_itens, no_arvore no){
//	readTree(no.left);
//	return no.value;
//}
char * change_suffix(char * name,char * sufix){
	int name_size = strlen(name);
	int dot;
	for (dot = name_size; name[dot]!='.' && dot!=0;dot--){cout<<dot<<endl;}
	if (dot==0){
		dot = name_size;
	}
	int sufix_size = strlen(sufix);
	char * outfile_name = (char*) malloc((name_size - dot + sufix_size +1) * sizeof(char));
	strncpy(outfile_name,name,dot);
	strcpy((char*)&outfile_name[dot],sufix);
	return outfile_name;
}

void codify_tree (int raiz, int cod, int count, vector<no_arvore> vet_arvore, dic_item * items){
	no_arvore no = vet_arvore[raiz];
	dic_item * item = &items[no.pointer];
//	cout<<"atual: "<<item->c<<'('<<raiz<<") filhos: "<<no.left<<" esq, "<<no.right<<" direita.\n";

	if(no.leaf == 1){
		item->cod = cod;
		item->cod_bits = (char)count;
//		cout<<"leaf\n";
//		cout<<"\ngiving id:"<<item->c<<" cod "<<item->cod<<" bits "<<(int)item->cod_bits<<endl;
		return;
	}
	count ++;
//	printf("%x(%d)",cod,count);
	cerr<<"n"<<raiz<<" -> "<<"n"<<no.left<<"[label=\"0\"];\n";
	cerr<<"n"<<raiz<<" -> "<<"n"<<no.right<<"[label=\"1\"];\n";
	codify_tree(no.left, (cod<<1),count,vet_arvore,items);
	codify_tree(no.right, (cod<<1) + 1,count,vet_arvore,items);
}

void compress(char * file_name){
//	fstream input_file = fstream(file_name);

	char sufix [5] = ".afh";
	char * outfile_name = change_suffix(file_name, sufix);
	fstream dic= fstream(outfile_name,ios::binary|ios::out);

	fstream input_file;
	input_file.open(file_name,ios::in);
	input_file.seekg(0,ios::end);
	int size = input_file.tellg();
	input_file.seekg(0,ios::beg);
	char * texto = (char *)malloc(sizeof(char)*size);

	string line;
	map <char,dic_item> mapa_entradas;
//	int i = 0;
//	while (getline(input_file,line)){ //não muito confiável
//		for(auto it: line){
	char it;
	for(int i =0;i <size; i++){
		read_file<char>(&input_file, &it);
//		cout<<it;
//		cout<<" => ";
		auto fresult = mapa_entradas.find(it);
		if(fresult==mapa_entradas.end()){
			dic_item value;
			value.c = (char)it;
			value.cod = 0;
			value.cod_bits = (char)0;
			value.qnt = (char)1;
			mapa_entradas.insert(pair<char,dic_item>(it,value));
		}else{
			mapa_entradas[it].qnt++;
		}
//		cout<<(int)mapa_entradas[it].qnt<<endl;
		texto[i]=it;
//		}
	}
	cout<<"entropia: "<<calc_entropy(texto, size)<<endl;
//	sort(mapping.begin(),mapping.end());//,[](const dic_item a, const dic_item b){
//		return a.qnt < b.qnt;
//	});
//	vector<dic_item> entries; //nós
	dic_item entries[1000];
	vector<no_arvore> reducao_arvore; //nós
	vector<no_arvore> arquivo_arvore;
	dic_header cabecalho;
	cabecalho.n_entradas=mapa_entradas.size();
	cabecalho.beg_tree=sizeof(dic_header)+ mapa_entradas.size() * sizeof(dic_item);
	cabecalho.original_size=size;
	cabecalho.zip_size=0;
//	write_file<dic_header>(&dic, &cabecalho);
	dic.seekp(sizeof(dic_header), ios::beg);
	cout<<"tellg:"<<dic.tellg()<<endl;
	int tree_count = 0;
	for(pair<char,dic_item> it:mapa_entradas){
		//ar_itens[ar_count++]=it.second;
		no_arvore no;
		no.left=(char)-1;//-1 e -1 é final
		no.right=(char)-1;
		no.leaf = 1;
		no.pointer=tree_count;//ponteiros para entradas
		no.qnt=(int)it.second.qnt;

		entries[tree_count++] = it.second;
		reducao_arvore.push_back(no);
		arquivo_arvore.push_back(no);//grava entradas "Entries"
	}
	int size_entries=tree_count;
	cout<<"Número de entradas: "<<tree_count<<"\n"<<endl;
	int camada=0;
	while(reducao_arvore.size()>1){
		sort(reducao_arvore.begin(),reducao_arvore.end(),[](const no_arvore a, const no_arvore b){
			return a.qnt < b.qnt;
		});

		no_arvore * novo = (no_arvore*)malloc(sizeof(no_arvore));
		novo->leaf=0;
//		reducao_arvore[0];
//		reducao_arvore[1];
//		dic_item left=entries[reducao_arvore[0].pointer];//modifica entradas
//		dic_item right=entries[reducao_arvore[1].pointer];

		novo->left=reducao_arvore[0].pointer; //ponteiro para a propria arvore
		novo->right=reducao_arvore[1].pointer;

		cout<<"Ciclos: "<<(++camada)<<" chars: "<<entries[reducao_arvore[0].pointer].c<<' '<<entries[reducao_arvore[1].pointer].c<<endl;

		novo->qnt=(char)reducao_arvore[0].qnt + reducao_arvore[1].qnt;
		novo->pointer=tree_count++;// ponteiro dentro da própria árvore
		reducao_arvore.erase(reducao_arvore.begin()+1);
		reducao_arvore.erase(reducao_arvore.begin());
		reducao_arvore.push_back(*novo);
		arquivo_arvore.push_back(*novo);//arquivo
//		itens[array_count++]=novo;
	}
	cerr<<"digraph regexp {\n";
	for(auto no:arquivo_arvore){
		char c = entries[no.pointer].c;
		int qnt = no.qnt;
		c = (c>='a' && c<='z') || (c>='A' && c<='Z') ?c: c=='\n'?'@':' ';
		char l = no.leaf == 1? 'l':'_';
		cerr<<'n'<<no.pointer<<" [label=\"("<<c<<") "<<qnt<<"\"];\n";
	}
	no_arvore raiz = reducao_arvore[0];
//	if (raiz.leaf)

//	cout << "filhos de raiz: "<<(int)raiz.qnt <<" pointer: "<<(int)raiz.pointer<< endl<<endl;

	codify_tree(raiz.pointer, 0, 0, arquivo_arvore, entries);
	cerr<<"}\n";
	for(int i=0;i<size_entries;i++){
		dic_item it = entries[i];
		printf("char %c cod %x size %d qnt %d\n",it.c,it.cod,it.cod_bits,it.qnt);
		write_file<dic_item>(&dic, &it);
	}
	cout<<"fim entradas:"<<dic.tellg()<<endl;
	for(no_arvore it: arquivo_arvore){
		cout<<"pointer "<<(int)it.pointer<<endl;
		write_file<no_arvore>(&dic, &it);
	}
	cout<<"fim arvore:"<<dic.tellg()<<endl;

	bitstrm bstm;
	bstm.strm=&dic;//arquivo para gravar

	cout<<"Comprimindo:\n";
//	input_file.seekg(0, ios::beg);
	for(int i =0;i < size; i++){
		char c=texto[i];
//		read_file<char>(&input_file,&c);
//		cout<<c;
		auto iter = mapa_entradas.find(c);
		int o=0;
		for ( ; o < mapa_entradas.size();o++)
			if (entries[o].c==c)
				break;
		dic_item item = entries[o];
//		cout<<(char)entries[o].c;
//		printf("%c %x(%d)[",item.c,item.cod,item.cod_bits);
		cout<<'[';
		if (item.cod_bits != 0){
			for (int z=32-item.cod_bits; z < 32 ;z++){
				write_bit(&bstm,((item.cod << z) & 0x80000000) == 0x80000000);
			//	cout<<'>'<<(7-z)<<"< ";
//				printf(",%x z=%d",((item.cod << z)& 0x80000000) == 0x80000000,z);
				cout<<(((item.cod << z)& 0x80000000) == 0x80000000);
				cabecalho.zip_size++;
			}
		}
		cout<<"]\n";
	}

	// empurra os bits que faltam
	flush_bit(&bstm);
	cout<<"\npos\ng: "<<dic.tellg()<<" p: "<<dic.tellp()<<endl;
	dic.seekg(0, ios::beg);
	cabecalho.n_tree=arquivo_arvore.size();
	cabecalho.root_tree=raiz.pointer;
	write_file<dic_header>(&dic, &cabecalho);//grava zip size etc..
//	cout<<"size of dic_item:"<<sizeof(dic_item)<<endl;
//	cout<<"size of no_arvore:"<<sizeof(no_arvore)<<endl;
//	cout<<"size of char:"<<sizeof(char)<<endl;
	cout<<"Bits original size: "<<size*8<<" zip size: "<<cabecalho.zip_size<<endl;
	dic.close();
}



int seek_tree(no_arvore * tree_vect, int * position,char * zipped,int raiz){ //position bit a bit
//	cout<<"ponteiro atual:"<<raiz<<endl<<flush;
	char leitura = zipped[*position / 8];
	no_arvore * no = & tree_vect[raiz];
//	cout<<"filhos: "<<no->left<<' '<<no->right<<endl<<flush;
//	cout<<"position: "<<*position<<'['<<*position / 8<<"] leitura: "<<leitura<<endl<<flush;
//	*position=*position+1;
	if (no->leaf)
		return no->pointer;
	else{
		*position=*position+1;
//		cout<<"pos"<<*position<<' '<<(int)(0x80&(leitura << (*position-1)%8 ))<<' ';
//		if( (0x80&(leitura << (*position-1)%8 ) )== 0x80  ){
		if( (0x1&(leitura >> (*position-1)%8 ) )== 0x1  ){
//			cout<<1<<endl;
			return seek_tree(tree_vect,position,zipped,(int)no->right);
		}else{
//			cout<<0<<endl;
			return seek_tree(tree_vect, position,zipped,(int)no->left);
		}
	}
}
using namespace std;
void decompress(char * file_name){
	char sufix [5] = ".out";
	char * outfile_name = change_suffix(file_name, sufix);
	ofstream output_file = ofstream(outfile_name,ios::out|ios::binary);
	ifstream input_file = ifstream(file_name,ios::in|ios::binary);
	input_file.seekg(0, ios::end);
	int size = input_file.tellg();
	input_file.seekg(0, ios::beg);
	dic_header cabecalho;
	read_file<dic_header>((fstream*)&input_file,&cabecalho);
	dic_item * entradas = (dic_item*) malloc(sizeof(dic_item) * cabecalho.n_entradas);
	no_arvore * arvore = (no_arvore*) malloc(sizeof(no_arvore) * cabecalho.n_tree);

	cout<<"cabecalho\nentradas: "<<cabecalho.n_entradas<<" raiz arvore:"<<cabecalho.root_tree<<endl<<flush;
	cout<<"tellg: "<<input_file.tellg()<<"from"<<size<<endl<<flush;
	for ( int i = 0 ; i < cabecalho.n_entradas; i++){
		read_file<dic_item>((fstream *)&input_file, &entradas[i]);
		cout<<entradas[i].c<<'['<<(int)entradas[i].c<<"] cod>"<<entradas[i].cod<<endl;
	}
	cout<<"end entradas:"<<input_file.tellg()<<"from"<<size<<endl<<flush;
	for ( int i = 0 ; i < cabecalho.n_tree; i++)
		read_file<no_arvore>((fstream *)&input_file, &arvore[i]);
	// a partir daqui o stream começa a parte "zipada"
	cout<<"End Arvore:"<<input_file.tellg()<<"from"<<size<<endl<<flush;
	cout<<"zip size:"<<cabecalho.zip_size<<endl<<flush;
	char * zipped = (char*) malloc(cabecalho.zip_size/8 + 1);// 8* pois
	for ( int i = 0 ; i < (cabecalho.zip_size/8) + 1;i++){
		read_file<char>((fstream *)&input_file, &zipped[i]);
		printf("%x ",(unsigned int)zipped[i]);
	}
	int position = 0;
	cout<<"\nreading at5:"<<input_file.tellg()<<"from"<<size<<endl<<flush;
	char aaa[10];
	while (position<cabecalho.zip_size){
		int c = seek_tree(arvore, &position, zipped, cabecalho.root_tree);
		cout<<entradas[c].c;
//		cin>>aaa;
		//write_file((fstream*)&output_file, &entradas[c].c);
	}
	output_file.close();
}

int main(int argc, char **argv) {
//	fstream bin_test = fstream("bin_test",ios::binary|ios::out);
//	bitstrm bstm;
//	bstm.strm=&bin_test;
//	for (int o = 0; o <2;o++){
//
//		for(int i = 0;i<4;i++)
//			write_bit(&bstm, 1);
//		for(int i = 0;i<4;i++)
//			write_bit(&bstm, 0);
//		for(int i = 0;i<8;i++)
//			write_bit(&bstm, 1);
//	}
//	for(int i = 0;i<8;i++)
//		write_bit(&bstm, 0);
//	for(int i = 0;i<8;i++)
//		write_bit(&bstm, 1);
//	for(int i = 0;i<8;i++)
//		write_bit(&bstm, 1);
//	bin_test.close();
	char * file_name = argv[argc-1];
	char * action = argv[argc-2];
//	int n = 2; cout << "teste bit:"<<(0x80 >>  (7-(n%8) ))<<endl; //mover bits pra direita conforme o número de n
	if (!strcmp(action,"-c")||!strcmp(action,"--compress")){
		cout<<"compress"<<endl;
		compress(file_name);
	}else if (!strcmp(action,"-d")||!strcmp(action,"--decompress")){
		cout<<"decompress..."<<endl;
		decompress(file_name);
	}else if (!strcmp(action,"-h")||!strcmp(action,"--help")){
		cout<<"Usage:\n\tCompress huffman -c original_file\n\tDecompress -d compressed_file\n";
	}else{
		compress(file_name);
	}
	cout << "size of bool: " << sizeof(bool) << endl;
}

