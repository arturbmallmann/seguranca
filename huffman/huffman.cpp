#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
//#include <hash_set>
#include <vector>
#include <map>

using namespace std;


struct {
	int size;
	int tree_pointers;
}typedef dic_header;

struct {
	char c;
	char size;
	char cod;
	char qnt;
}typedef dic_item;

struct {
	int left; //relative pointer
	int right;
	int qnt;
	int value;
}typedef no_arvore;

class huffman_tree{
//	no raiz;
	public:
		huffman_tree(){

		}

};

template<typename T>
void read_file(fstream *strm, T *dest,int n=1){
	strm->read(reinterpret_cast<char *>(dest), sizeof(T)*n);
}
template<typename T>
void write_file(fstream *strm, T *src,int n=1){
	strm->write( reinterpret_cast<char *>(&src), sizeof(T) * n);
}

//dic_item * readTree(vector<no_arvore> ar_itens, no_arvore no){
//	readTree(no.left);
//	return no.value;
//}
int main(int argc, char **argv) {
	char * file_name = argv[argc-1];
//	fstream input_file = fstream(file_name);

	fstream dic= fstream("dic.data",ios::binary |ios::in |ios::out);
	fstream input_file;
	input_file.open(file_name);
	string line;
	map <char,dic_item> mapping;
	while (getline(input_file,line)){
		for(auto it: line){
			cout<<it;
			cout<<" => ";
			auto fresult = mapping.find(it);
			if(fresult==mapping.end()){
				dic_item value;
				value.c = (char)it;
				value.cod = (char)0;
				value.size = (char)0;
				value.qnt = (char)1;
				mapping.insert(pair<char,dic_item>(it,value));
			}else{
				mapping[it].qnt++;
			}
			cout<<(int)mapping[it].qnt<<endl;
		}
	}
//	sort(mapping.begin(),mapping.end());//,[](const dic_item a, const dic_item b){
//		return a.qnt < b.qnt;
//	});
	vector<no_arvore> ar_itens; //nós
	int ar_count = 0;
	dic_header dh;
	dh.size=mapping.size();
	dh.tree_pointers=sizeof(dic_header)+ mapping.size() * sizeof(dic_item);
	write_file<dic_header>(&dic, &dh);
	for(auto it:mapping){
		//ar_itens[ar_count++]=it.second;
		no_arvore no;
		no.left=-1;
		no.right=-1;
		no.value=ar_count++;
		no.qnt=it.second.qnt;
		ar_itens.push_back(no);
		write_file<dic_item>(&dic, &it.second);//grava no arquivo
	}
	cout<<"pos\ng: "<<dic.tellg()<<" p: "<<dic.tellp()<<endl;
	sort(ar_itens.begin(),ar_itens.begin()+mapping.size(),[](const no_arvore a, const no_arvore b){
				return a.qnt < b.qnt;
			});
	for(auto it:ar_itens){
		dic.write(reinterpret_cast<char *>(&it), sizeof(no_arvore));
	}
	//for(int i; i<ar_count;i++){
	//dic_item it =ar_itens[i];
	dic.seekg(sizeof(dic_header),ios::beg);
	for (no_arvore it:ar_itens){
		dic.seekg(sizeof(dic_header)+sizeof(dic_item)*it.value, ios::beg);
		cout<<"position: "<<dic.tellg()<<endl;
		dic_item di;
		//dic.read(reinterpret_cast<char *>(&di), sizeof(dic_item));
		read_file<dic_item>(&dic, &di, 1);
		cout<<di.c<<" pointer: "<<it.value<<" qnt="<<(int)it.qnt<<"\n";
	//	dic.write(reinterpret_cast<char *>(&it), sizeof(dic_item));
	}

	while(ar_itens.size()>1){
		sort(ar_itens.begin(),ar_itens.end(),[](const no_arvore a, const no_arvore b){
					return a.qnt < b.qnt;
				});
		no_arvore * novo = (no_arvore*)malloc(sizeof(no_arvore));
		novo->left=ar_itens[0].value;
		novo->right=ar_itens[1].value;
		novo->qnt=ar_itens[0].qnt + ar_itens[1].qnt;
		novo->value=-1;//-1 == não nó folha
		ar_itens.erase(ar_itens.begin());
		ar_itens.erase(ar_itens.begin());
		ar_itens.push_back(*novo);
	}

	dic.seekg(dh.tree_pointers,ios::beg);
	cout<<"pos\ng: "<<dic.tellg()<<" p: "<<dic.tellp()<<endl;
	cout<<"size of dic_item:"<<sizeof(dic_item)<<endl;
	cout<<"size of char:"<<sizeof(char)<<endl;
	dic.close();
}
