#include <time.h>
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<math.h>
#include<string.h>
#include<conio.h>


using namespace std;

int lru[100][50];

int BinarioPraDecimal(long long n);
int atualizaLRU(int set, int assoc, int x);


int main(int argc, char *argv[]){
	
	if(argc!=5){
		cout<<"Numero de argumentos invalido";
		exit(0);
	}
	int tam, nsets, assoc, bsize,i,j, nblocos,r,alg, pos;
	float taxa;
	long int endereco;
	char bin[32], nomeArquivo[20];
	int no_set;
	int acesso=0, hit=0;
	int miss[3] = {0,0,0};
	nsets= atoi(argv[1]);
	bsize = atoi(argv[2]);
	assoc = atoi(argv[3]);
	strcpy(nomeArquivo, argv[4]);
	tam = nsets * assoc * bsize;
	srand(time(NULL));

	nblocos = tam / bsize;

	int cache[nsets][assoc];

	for(i=0;i<nsets;i++)
		for(j=0;j<assoc;j++)
			cache[i][j] = -1;


	int fifo[nsets];
		for(i=0;i<nsets;i++)
			fifo[i] = 0;

    for(i=0;i<nsets;i++)
        for(j=0;j<assoc;j++)
            lru[i][j] = j;
	
	int set;
	
	ifstream infile;
	infile.open(nomeArquivo,ios::in);
	if(!infile){
    	cout<<"Arquivo nao encontrado";
    	exit(0);
	}
	
	cout<<"Escolha uma opcao \n 1. FIFO  2.LRU  3. Random: ";
	cin>>alg;
	
	
	
	while(!infile.eof()){ 
		bool achou = false;
		bool espaco = false;
		bool comp = false;
    	infile>>bin;
        endereco = BinarioPraDecimal(atoi(bin));

		set = (endereco / bsize) % nsets;

  		acesso++;
  		for(i=0;i<assoc;i++){
   			if(cache[set][i] == endereco)
        		achou = true;
        		pos = i;
    	}
    	
    	if(achou){
    		if(alg == 2){
            	atualizaLRU(set,assoc,pos);
      		}
    		hit++;
		} else {
			for(i=0;i<assoc;i++){
    			if(cache[set][i] == -1){
					cache[set][i] = endereco;
					miss[0] = miss[0] + 1;
					comp = true;
					break;
				}
			}
			if(!comp){
				for(i=0;i<nsets;i++){
					for(j=0;j<assoc;j++){
						if(cache[i][j] == -1)
							espaco = true;
					}
				}
				if(espaco){
					miss[1] = miss[1] + 1;
				} else{
					miss[2] = miss[2] + 1;
				}
				
				if(alg==1){
            		i = fifo[set];
   					cache[set][i] = endereco;
   					fifo[set]++;
   					if(fifo[set] == assoc)
    					fifo[set] = 0;
            	}else if(alg==2){
                	i = lru[set][0];
                	cache[set][i] = endereco;
                	atualizaLRU(set,assoc,i);
           		}else{
                	r = rand() % assoc;
                	cache[set][r] = endereco;
           		}
    		}
  		}
    	
 	}
 	
	infile.close();
	taxa = ((float)((miss[0] + miss[1] + miss[2])) / (float)acesso);
	system("CLS");
	cout<<"Numero de acessos : "<<acesso;
	cout<<"\n";
	cout<<"Numero de hits : "<<hit;
	cout<<"\n";
	cout<<"Numero de misses compulsorios : "<<miss[0];
	cout<<"\n";
	cout<<"Numero de misses de conflito : "<<miss[1];
	cout<<"\n";
	cout<<"Numero de misses de capacidade : "<<miss[2];
	cout<<"\n";
	cout<<"Taxa de miss : "<<taxa * 100;
	
	return 0;
}



int BinarioPraDecimal(long long n){
    int decimal = 0, i = 0, x;
    while (n!=0){
        x = n%10;
        n /= 10;
        decimal += x*pow(2,i);
        i++;
    }
    return decimal;
}

int atualizaLRU(int set, int assoc, int x){
    int i,pos;
    for(i=0;i<assoc;i++)
        if(lru[set][i] == x)
            pos = i;
    for(i=pos;i<assoc-1;i++)
        lru[set][i] = lru[set][i+1];
    lru[set][assoc-1] = x;
}


