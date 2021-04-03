#include <math.h>
#include <iostream>
#include <string>
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
	for (int d = 2; d < totiente; d+= 1)
		if (d*e % totiente == 1)
			return d;
	return 0;
}

int powmod2 (int b, int e, int modulo){
	if(e==0)
		return 1;
 	int acum = b;
 	for (int i=1; i < e ; i++){
        acum = acum * b % modulo;
 	}
	std::cerr<<"pw2: "<<b<<" ^ "<<e<<" % "<<modulo<<" =  "<<acum<<'\n';
 	return acum;
}

int powmod5 (int b, int e, int modulo){
	if(e == 0)
		return 1;
	std::cerr<<"init: "<<b<<" ^ "<<e<<" % "<<modulo<<'\n';
	int acum = 1;
	int rest = e%5;
	if (e >= 5){
		int save = powmod2(b,5,modulo);
		for (int i = 5; i <= e; i+= 5){
			std::cerr<<"acum-:"<<acum;
			acum = acum * save % modulo;
			std::cerr<<"base: "<<b<<" e: "<<e<<" i: "<<i<<" acum: "<<acum<<'\n';
	}
	}
	std::cerr<<"acum+:"<<acum<<' ';
	acum = acum * powmod2(b,rest,modulo) % modulo;
	std::cerr<<"base: "<<b<<" e: "<<e<<" rest: "<<rest<<" acum: "<<acum<<'\n';
	std::cerr<<"end:"<<acum<<"\n\n";
	return acum;
}

using namespace std;
int powmod(int b, int e, int modulo, int * phisao,int size){
	if (e==0)
		return 1;
	int nexp = 0;
	int um = 0;
	int expoentes[15];
	int iexp = 0;
	if (e % 2 == 1){
		nexp = e - 1;
		um  = 1; // + uma multiplicacao
	}
	else nexp = e;
	for (int i = 0; phisao[i] <= nexp ; i++){
		if (nexp % phisao[i] == 0){
			nexp = nexp/phisao[i];
			expoentes[iexp++] = phisao[i];
			i--;
		}
	}
	int acum = b;
	for(int i=0; i< iexp; i++){
		//acum = powmod2(acum, expoentes[i], modulo);
		acum = powmod5(acum, expoentes[i], modulo);
		std::cerr<<"exp: "<<expoentes[i]<<" acum: "<<acum<<"\n\n";
	}
	//acum = powmod2(acum, nexp, modulo);
//	acum = powmod5(acum, nexp, modulo);
	if (um){
		std::cerr<<"um: "<<acum<<" * "<<b<<" % "<<modulo<<" = ";
		acum = acum * b % modulo;
		std::cerr<<acum<<'\n';

	}
	return acum;
}


using namespace std;
int main(){
	int size = sqrt(pow(10,9));
//	cout<<"size: "<<size<<endl;
	int * phisao = (int *) malloc(sizeof(int) * size);
	crivo(size,phisao);
	int n, e;
	int c;
	scanf("%d %d %d",&n,&e, &c);
	int p, q;
	fatorar (n, &p, &q, phisao, size);

	int totiente = (p-1)*(q-1);
	int d = gera_chave_d(e,totiente);

//	cout << std::fmod (std::pow(c,d), n) <<endl;
	
	cout<<powmod (c,d,n,phisao,size)<<endl;
//	cout<<powmod2(c,d,n)<<endl;
//	cout<<powmod5(c,d,n)<<endl;
//	for (int i = 0; i< size; i++)
//		cout << phisao[i] << ' ';
	return 0;
}
