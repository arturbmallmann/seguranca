#include <math.h>
#include <iostream>
#include <string>

int ciclos;
#define ERR
inline void crivo (int n,int *phisao) {
    phisao[0]=2;
    int i,j,tp=1;
    int primo;
    for( i = 3 ; tp < n ; i+= 2 ){
        primo = 1;
//      printf("floor de %d é: %d",i,(int)floor(sqrt(i)));
        for( j = 0 ; phisao[j] <= (int)floor(sqrt(i)) && j < tp; j++ ){
#ifdef ERR
            ciclos++;
#endif
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
void fatorar(int n, long long int * p, long long int * q,int * phisao, int size){
//	for (*q=3; *q < ceil(sqrt(n));*q = *q + 1){
	for (int i=0; i<size;i++){
		*q = phisao[i];
//		cout<<"q "<<*q<<endl;
		if (n % *q == 0){
			*p = n / *q;
		//	*q = q;
			ciclos += i;
			break;
		}
	}
}
// using namespace std;
// void fatorar2(int n, long long int * p, long long int * q,int * phisao, int size){
// 	for (*q=3; *q < ceil(sqrt(n));*q = *q + 1){
// 	*p = n;
// 	for (int i=0; i<size;i++){
// 		cout<<"q "<<*q<<endl;
// 		if (*p % phisao[i] == 0){
// 			*p = *p / phisao[i];
// 			cerr<<"multiplos de "<<n<<": "<<phisao[i]<<endl;
// 		}
// 	}
// }
int gera_chave_d (long long int e, long long int totiente, int n){
//     cerr<<"e: "<<e<<" totiente: "<<totiente<<endl;

    for(int k=0;true; k++){
        int d = (k * totiente +1 ) / e ;
        if (d*e % totiente ==1){
#ifdef ERR
            cerr <<"D: "<<d<<" ciclos: "<<k<<endl;
            ciclos += k;
#endif
            return d;
        }
//		cout<<"D: "<<d<<endl;
    }
	
	return -1;
}

int powmod2 (long long int b, long long int e,long long int modulo){
	if(e==0)
		return 1;
 	long long int acum = b;
 	for (int i=1; i < e ; i++){
        acum = acum % modulo * b % modulo;
 	}
	ciclos += e;
//	std::cerr<<"pw2: "<<b<<" ^ "<<e<<" % "<<modulo<<" =  "<<acum<<'\n';
 	return acum;
}
inline int powmod5 (long long int b, long long int e, long long int modulo){
//	if(e == 0)
//		return 1;
//	std::cerr<<"init: "<<b<<" ^ "<<e<<" % "<<modulo<<'\n';
	long long int acum = 1;
	long long int rest = e%5;
	if (e >= 5){
		long long int save = powmod2(b,5,modulo);
		for (long long int i = 5; i <= e; i+= 5){
//			std::cerr<<"acum-:"<<acum;
			acum = acum % modulo * save % modulo;
//			std::cerr<<"base: "<<b<<" e: "<<e<<" i: "<<i<<" acum: "<<acum<<'\n';
#ifdef ERR
			ciclos ++;
#endif
        }
	}
//	std::cerr<<"acum+:"<<acum<<' ';
	acum = acum % modulo * powmod2(b,rest,modulo) % modulo;
//	std::cerr<<"base: "<<b<<" e: "<<e<<" rest: "<<rest<<" acum: "<<acum<<'\n';
//	std::cerr<<"end:"<<acum<<"\n\n";
	return acum;
}
inline int powmod200 (long long int b, long long int e, long long int modulo){
//	if(e == 0)
//		return 1;
//	std::cerr<<"init: "<<b<<" ^ "<<e<<" % "<<modulo<<'\n';
	long long int acum = 1;
	long long int rest = e%200;
	if (e >= 200){
		long long int save = powmod5(b,200,modulo);
		for (long long int i = 200; i <= e; i+= 200){
//			std::cerr<<"acum-:"<<acum;
			acum = acum % modulo * save % modulo;
//			std::cerr<<"base: "<<b<<" e: "<<e<<" i: "<<i<<" acum: "<<acum<<'\n';
#ifdef ERR
			ciclos ++;
#endif
        }
	}
//	std::cerr<<"acum+:"<<acum<<' ';
	acum = acum % modulo * powmod5(b,rest,modulo) % modulo;
//	std::cerr<<"base: "<<b<<" e: "<<e<<" rest: "<<rest<<" acum: "<<acum<<'\n';
//	std::cerr<<"end:"<<acum<<"\n\n";
	return acum;
}

using namespace std;
inline int powmod(long long int b, long long int e, long long int modulo, int * phisao,int size){
	//if (e==0)
	//	return 1;
	long long int nexp = 0;
	int um = 0;
	int expoentes[100];
	int iexp = 0;
	if (e % 2 == 1){
		nexp = e - 1;
		um  = 1; // + uma multiplicacao
	}
	else nexp = e;
// 	cerr<<"size "<<size<<endl;
	for (int i = 0; phisao[i] <= nexp && i < size; i++){
#ifdef ERR
		ciclos++;
#endif
		if (nexp % phisao[i] == 0){
			nexp = nexp/phisao[i];
			expoentes[iexp++] = phisao[i];
			i--;
		}
	}
	int acum = b;
	for(int i=0; i< iexp; i++){
		//acum = powmod2(acum, expoentes[i], modulo);
		acum = powmod200(acum, expoentes[i], modulo);
        #ifdef ERR
        std::cerr<<"exp: "<<expoentes[i]<<" acum: "<<acum<<"\n\n";
        #endif
	}
	ciclos += iexp;
	//acum = powmod2(acum, nexp, modulo);
	acum = powmod200(acum, nexp, modulo);
	if (um){
		acum = acum % modulo * b % modulo;
        #ifdef ERR
            std::cerr<<"um: "<<acum<<" * "<<b<<" % "<<modulo<<" = ";
            std::cerr<<acum<<'\n';
        #endif
	}
	return acum;
}


using namespace std;
int main(){
//	int size = sqrt(pow(10,9));
//	int size = 10000;
//	cout<<"size: "<<size<<endl;
	int n, e;
	int c;
	scanf("%d %d %d",&n,&e, &c);
	int size = sqrt(n);

	int * phisao = (int *) malloc(sizeof(int) * size);
	crivo(size,phisao);
	long long int p, q;
	fatorar (n, &p, &q, phisao, size);
	long long int totiente = (p-1)*(q-1);
	long long int d = gera_chave_d(e,totiente,n);
#ifdef ERR
	cerr<<"d: "<<d<<endl;
	cerr<<"fatoração: "<<p<<" e "<<q<<endl;
// 	long long int a,b;
// 	cerr<<"totiente:\n";
// 	fatorar2((int)totiente,&a,&b,phisao,size);
// 	cerr<<"d:\n";
// 	fatorar2((int)d, &a,&b,phisao,size);
// 	cerr<<"d-1:\n";
// 	fatorar2((int)d-1, &a,&b,phisao,size);
// 	cerr<<"d+1:\n";
// 	fatorar2((int)d+1, &a,&b,phisao,size);
// 	cerr<<"e:\n";
// 	fatorar2(e,&a,&b,phisao,size);
//	fatorar2(e-1,&a,&b,phisao,size);
//	fatorar2(e+1,&a,&b,phisao,size);
//	cout << std::fmod (std::pow(c,d), n) <<endl;
#endif
    
//	cout<<powmod2(c,d,n)<<endl;
//	cout<<powmod5(c,d,n)<<endl;
//	cout<<powmod200(c,d,n)<<endl;
	cout<<powmod (c,d,n,phisao,size)<<endl;
#ifdef ERR
    cerr<<"ciclos: "<<ciclos<<endl;
#endif
//    cout<<powmod (c,d,n,phisao,size)<<endl;
//	for (int i = 0; i< size; i++)
//		cout << phisao[i] << ' ';
	return 0;
}
