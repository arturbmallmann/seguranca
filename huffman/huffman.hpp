#ifndef HUFMFMAN_HPP_
#define HUFMFMAN_HPP_

#include <fstream>
struct {
	int size;
	int tree_pointers;
	int root_tree;
}typedef dic_header;

struct {
	char c;
	char size;
	char cod;
	char qnt;
}typedef dic_item;

struct {
	char left; //relative pointer to arvore
	char right;
	char qnt;
	char pointer; //pointer to dic item
}typedef no_arvore;
//
//class huffman_tree{
//
//	public:
//		huffman_tree(){
//
//		}
//
//};

template<typename T>
void read_file(std::fstream *strm, T *dest,int n=1){
	strm->read(reinterpret_cast<char *>(dest), sizeof(T)*n);
}
template<typename T>
inline void write_file(std::fstream *strm, T *src,int n=1){
	strm->write( reinterpret_cast<char *>(src), sizeof(T) * n);
}


#endif
