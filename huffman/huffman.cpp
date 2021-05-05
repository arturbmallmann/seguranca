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
	cout << "Bit cache "<<(int) bstrm->cache << " Flushed!" <<endl;
	bstrm->cache=0;
	bstrm->control=0;
}

void write_bit (bitstrm * bstrm,char bit){
	bit = bit & 0x1;
	if(bstrm->control == 8){
		flush_bit(bstrm);
	}
	bstrm->cache = bstrm->cache>>1 & 0x7f; //zero no mais significativo
	bstrm->cache += (char)(bit<<7);
	bstrm->control++;
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
	strncpy((char*)outfile_name+dot,sufix,sufix_size);
	return outfile_name;
}

void codify_tree (int raiz, char cod, int count, vector<no_arvore> vet_arvore, array<dic_item, 256> items){
	no_arvore no = vet_arvore[raiz];
	dic_item item = items[no.pointer];
	if(no.leaf){
		item.cod = cod;
		item.cod_bits = count;
	}
	count ++;
	codify_tree(no.left, cod<<1,count,vet_arvore,items);
	codify_tree(no.right, cod<<1 + 1,count,vet_arvore,items);
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
		cout<<it;
		cout<<" => ";
		auto fresult = mapa_entradas.find(it);
		if(fresult==mapa_entradas.end()){
			dic_item value;
			value.c = (char)it;
			value.cod = (char)0;
			value.cod_bits = (char)0;
			value.qnt = (char)1;
			mapa_entradas.insert(pair<char,dic_item>(it,value));
		}else{
			mapa_entradas[it].qnt++;
		}
		cout<<(int)mapa_entradas[it].qnt<<endl;
		texto[i]=it;
//		}
	}
	cout<<"entropia: "<<calc_entropy(texto, size)<<endl;
//	sort(mapping.begin(),mapping.end());//,[](const dic_item a, const dic_item b){
//		return a.qnt < b.qnt;
//	});
	array<dic_item, 256> entries; //nós
	vector<no_arvore> reducao_arvore; //nós
	vector<no_arvore> arquivo_arvore;
	int tree_count = 0;
	dic_header cabecalho;
	cabecalho.n_entradas=mapa_entradas.size();
	cabecalho.beg_tree=sizeof(dic_header)+ mapa_entradas.size() * sizeof(dic_item);
	cabecalho.original_size=size;
	write_file<dic_header>(&dic, &cabecalho);
	int array_count = 0;
	for(auto it:mapa_entradas){
		entries[array_count++]=it.second;
		//ar_itens[ar_count++]=it.second;
		no_arvore no;
		no.left=(char)-1;//-1 e -1 é final
		no.right=(char)-1;
		no.leaf = 1;
		no.pointer=tree_count++;//ponteiros para entradas
		no.qnt=(int)it.second.qnt;
		reducao_arvore.push_back(no);
		arquivo_arvore.push_back(no);//grava entradas "Entries"
	}

	cout<<"Número de entradas: "<<array_count<<"\n"<<endl;
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

		cout<<"Ciclos: "<<(++camada)<<" cod: "<<(int)entries[reducao_arvore[1].pointer].cod;
		cout<<" char: "<<entries[reducao_arvore[1].pointer].c<<endl;

		novo->qnt=(char)reducao_arvore[0].qnt + reducao_arvore[1].qnt;
		novo->pointer=tree_count++;// ponteiro dentro da própria árvore
		reducao_arvore.erase(reducao_arvore.begin());
		reducao_arvore.erase(reducao_arvore.begin());
		reducao_arvore.push_back(*novo);
		arquivo_arvore.push_back(*novo);//arquivo
//		itens[array_count++]=novo;
	}

	no_arvore raiz = reducao_arvore[0];
	cout << "filhos de raiz: "<<(int)raiz.qnt <<" pointer: "<<raiz.pointer<< endl;

	codify_tree(raiz.pointer, 0, 0, arquivo_arvore, entries);
	for(dic_item it: entries)
		write_file<dic_item>(&dic, &it);
	for(no_arvore it: arquivo_arvore)
		write_file<no_arvore>(&dic, &it);

	bitstrm bstm;
	bstm.strm=&dic;
//	char bits[10]="Bits f:";
//	write_file<char>(&dic,bits,strlen(bits));
//	cout<<bits<<dic.tellg()<<endl;


	cout<<"Cache:"<<endl;
	write_bit(&bstm,0);
	write_bit(&bstm,1);
	write_bit(&bstm,1);
	write_bit(&bstm,1);
	write_bit(&bstm,0);
	write_bit(&bstm,0);
	write_bit(&bstm,0);
	write_bit(&bstm,0);

	write_bit(&bstm,1);

	flush_bit(&bstm);// empurra os bits que faltam
	//dic.seekg(cabecalho.beg_tree,ios::beg);
	cout<<"pos\ng: "<<dic.tellg()<<" p: "<<dic.tellp()<<endl;
	cout<<"size of dic_item:"<<sizeof(dic_item)<<endl;
	cout<<"size of no_arvore:"<<sizeof(no_arvore)<<endl;
	cout<<"size of char:"<<sizeof(char)<<endl;
	dic.close();
}



int seek_tree(no_arvore * tree_vect, int * position,char * zipped,no_arvore * no){ //position bit a bit
	int leitura = zipped[*position/8];
	if (no->leaf)
		return no->pointer;
	else{
		*position++;
		if( 0x1 & leitura >> (7-(*position%8) ) )
			return seek_tree(tree_vect,position,zipped,&tree_vect[(int)no->right]);
		else
			return seek_tree(tree_vect, position,zipped,&tree_vect[(int)no->left]);
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
	dic_item * entradas = (dic_item*) malloc(sizeof(dic_item) * cabecalho.n_entradas);
	no_arvore * arvore = (no_arvore*) malloc(sizeof(no_arvore) * cabecalho.n_entradas);
	read_file<dic_header>((fstream*)&input_file,&cabecalho);
	//for ( int i = 0 ; i < cabecalho.n_entradas; i++)
		read_file<dic_item>((fstream *)&input_file, entradas,cabecalho.n_entradas);
	//while (input_file.tellg()==size)
		read_file<no_arvore>((fstream *)&input_file, arvore,cabecalho.n_tree);
	// a partir daqui o stream começa a parte "zipada"
	int zip_size = size - input_file.tellg();
	char * zipped = (char*) malloc(zip_size);// 8* pois
	read_file<char>((fstream *)&input_file, zipped, zip_size);
	int position = 0;
	while (input_file.tellg()==size){
		int c = seek_tree(arvore, &position, zipped, &arvore[cabecalho.root_tree]);
		write_file((fstream*)&output_file, &entradas[c]);
	}
	output_file.close();
}

int main(int argc, char **argv) {
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
	cout << (int)((char)0x7f>>1) <<endl;
	cout << "size of bool: " << sizeof(bool) << endl;
}

