#include "SSTree.h"
#include <string>
#include <stdlib.h>
#include <time.h>

using namespace std;

int cont(string &s) {
	int c = 0;
	tam ss = s.size();
	for (tam i = 0; i < ss; ++i) {
		if (s[i] == ',')++c;
	}
	return c;
}

int main() 
{
	//srand(time(NULL));
	int cant = 515345, dim = 91;
	string::size_type sz;
	vector<vec_flot> datos(cant);
	string s;
	for (int i = 0; i < cant; ++i) {
		cin >> s;
		vec_flot aux(dim);
		for (int j = 0; j < dim; ++j) {
			string sub = s.substr(0,s.find(","));
			aux[j] = stof((sub), &sz);
			s.erase(0, sub.size() + 1);
		}
		datos[i] = aux;
	}
	SSTree* SS = new SSTree(3, 2);
	double t0, t1;
	t0 = clock();
	for (int i = 0; i < cant; ++i) {
		SS->insert(datos[i]);
	}
	t1 = clock();
	double time = (double(t1 - t0) / CLOCKS_PER_SEC);
	cout << "Execution Time: " << time << endl;
	system("pause");
	return 0;
}