#include <bits/stdc++.h>
#include "pretraitement.hpp"
#include "better.h"
using namespace std;


int main(int ac, char **av) {
	if (ac != 2)
	{
		cout << "nom du fichier attendu" << endl;
		return 0;
	}
	string fichierEntree = "";
	for (int i(0); av[1][i]; ++i)
		fichierEntree += av[1][i];
	probleme = Probleme(fichierEntree);
	calculerSolution().afficheSolution();
	return 0;
}
