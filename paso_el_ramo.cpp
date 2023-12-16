#include<bits/stdc++.h>
using namespace std;

int main(int argc, char const *argv[])
{
	int cant_nota = 4;
	float promedio = 0;
	int nota;
	for (int i = 0; i < cant_nota; ++i){
		cout << "Ingrese la nota "<<i+1<<endl;
		cin >> nota;
		promedio += nota*0.025;
	}
	cout << "El promedio es "<<promedio<<endl;

	return 0;
}