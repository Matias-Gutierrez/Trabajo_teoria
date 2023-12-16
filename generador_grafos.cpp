#include<bits/stdc++.h>
using namespace std;
int tabla[1000][1000] {0};
int largo=50;
int main(int argc, char const *argv[]){
	std::random_device rd;
    std::mt19937 gen(rd());

	int j;
	int i;
	int variable;
	int limite_inferior=1;
	int limite_superior=50;
	std::uniform_int_distribution<> distribucion(limite_inferior, limite_superior);
	for (i = 0; i < largo ; ++i){
		for (j = 0; j < largo; ++j){
			if(tabla[j][i]==0 and i!=j){
				variable = distribucion(gen) ;
				tabla[i][j] = variable;
			}
			if(tabla[j][i]!=0){
				tabla[i][j] = tabla[j][i];
			}
			cout << tabla[i][j];
			if(j < largo-1){
				cout<<';';
			}
			
		}
		cout <<endl;
	}

}