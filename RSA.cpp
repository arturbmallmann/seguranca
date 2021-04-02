#include <math.h>
#include <iostream>

void crivo (int n,int *phisao) {
    phisao[0]=2;
    int i,j,tp=1;
    int primo;
    for( i = 3 ; tp < n ; i+= 2 ){
        primo = 1;
//      printf("floor de %d é: %d",i,(int)floor(sqrt(i)));
        for( j = 0 ; phisao[j] <= (int)floor(sqrt(i)) && j < tp; j++ ){
            if( i % phisao[j] == 0 ){
                primo = 0;
//              printf(". %d não é primo",i);
                break;
            }
//              printf(". %d é primo",i);
        }
//      printf("\n");
        if(primo)
            phisao[tp++]=i;
    }

}
using namespace std;
void fatorar(int n, int * p, int * q,int * phisao, int size){
//	for (*q=3; *q < ceil(sqrt(n));*q = *q + 1){
	for (int i=0; i<size;i++){
		*q = phisao[i];
//		cout<<"q "<<*q<<endl;
		if (n % *q == 0){
			*p = n / *q;
		//	*q = q;
			break;
		}
	}
}

int gera_chave_d (int e, int totiente){
	for (int d = 0; d < totiente; d+= e)
		if (d*e % totiente == 1)
			return d;
	return 0;
}

using namespace std;
int main(){
	int size = sqrt(pow(10,9));
//	cout<<"size: "<<size<<endl;
	int * phisao = (int *) malloc(sizeof(int) * size);
	crivo(size,phisao);
	int n, e;
	long double c;
	scanf("%d %d %LF",&n,&e, &c);
	int p, q;
	fatorar (n, &p, &q, phisao, size);
	cerr<<"n: "<<n<<" p: "<<p<<" q: "<<q<<endl;
	int totiente = (p-1)*(q-1);
	cerr<<"totiene: "<<totiente<<endl;
	long double d = (long double)gera_chave_d(e,totiente);
	cerr<<"c: "<<c<<" d: "<<d<<endl;
	cerr<<"c ^ d: "<<powl(c,d)<<endl;
	cout << fmodl (powl(c,d), (long double) n) <<endl;
//	for (int i = 0; i< size; i++)
//		cout << phisao[i] << ' ';
	return 0;
}
