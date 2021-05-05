#ifndef HUFMFMAN_HPP_
#define HUFMFMAN_HPP_

#include <fstream>
struct {
	int n_entradas;
	int beg_tree;
	int root_tree;
	int n_tree;
	int original_size;
	int zip_size;
}typedef dic_header;

struct {
	char c;
	int cod; //
	char cod_bits;
	int qnt;
}typedef dic_item;

struct {
	char leaf;
	int left; //relative pointer to arvore
	int right;
	int pointer; //ponteiro próprio ou na folha para dic_item
	int qnt;
}typedef no_arvore;

struct {
	std::fstream * strm;
	char cache=0;
	int control=0;
}typedef bitstrm;
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
