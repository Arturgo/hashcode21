#include <bits/stdc++.h>
#include "pretraitement.hpp"
using namespace std;

Solution calculerSolution(void){
	const Probleme &p = probleme;
	vector<vector<pair<int,int>>> reponse(p.nbIntersections);
	vector<vector<int> > nbPass(p.nbIntersections);
	for (auto voit : p.voitures)
		for (auto el : voit.chemin)
			nbPass[p.arcs[el].iFin].push_back(el);

	for(int i=0;i<p.nbIntersections;i++){
		sort(nbPass[i].begin(),nbPass[i].end());
		int nb=1, av=-1;
		for(int el:nbPass[i]){
			if(el==av)
				nb++;
			else if(av!=-1){
				assert(av < (int)p.arcs.size());
				if (nb >= 1)
					reponse[i].push_back(make_pair(av, nb));
				nb=1;
			}
			av=el;
		}
		if(nb >= 1 and av != -1)
		{
			assert(av < (int)p.arcs.size());
			reponse[i].push_back(make_pair(av, nb));
		}
	}
	for (auto voit : p.voitures)
		for (auto el : voit.chemin)
			assert(!reponse[p.arcs[el].iFin].empty());
	Solution ret;
	ret.solution = move(reponse);
	for (auto v : ret.solution)
		for (auto [id, ignore] : v)
		{
			assert(id >= 0);
			assert(id < probleme.arcs.size());
		}
	return ret;
}
