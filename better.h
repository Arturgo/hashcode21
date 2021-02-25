#include <bits/stdc++.h>
#include "pretraitement.hpp"
using namespace std;

Solution calculerSolution(void) {
	const Probleme &p = probleme;
	vector<vector<Arc>> parents(p.nbIntersections);
	
	for(Arc arc : p.arcs) {
		parents[arc.iFin].push_back(arc);
	}
	
	vector<vector<int>> plan(p.nbIntersections);
	
	for(int iInter = 0;iInter < p.nbIntersections;iInter++) {
		plan[iInter] = vector<int>(parents[iInter].size(), -1);
	}
	
	//schedule le reste
	
	vector<list<pair<int, int>>> etat;
	
	for(int iVoiture = 0;iVoiture < p.nbVoitures;iVoiture++) {
		etat[p.voitures[iVoiture].chemin[0]].push_back({iVoiture, 0});
	}
	
	vector<vector<pair<int, int>>> aAjouter(p.dureeSimulation);
	vector<set<int>> scheduled(p.nbIntersections);
	
	for(int iTemps = 0;iTemps < p.dureeSimulation;iTemps++) {		
		for(pair<int, int> ajout : aAjouter[iTemps]) {
			etat[p.voitures[ajout.first].chemin[ajout.second]].push_back(ajout);
		}
		
		for(int iInter = 0;iInter < p.nbIntersections;iInter++) {
			int iArc = plan[iInter][iTemps % plan[iInter].size()];
			
			if(iArc == -1) {
				for(Arc arc : parents[iInter]) {
					int id = arc.id;
					if(!etat[id].empty() && !scheduled[iInter].count(id)) {
						iArc = id;
						scheduled[iInter].insert(id);
						break;
					}
				}
			}
			
			plan[iInter][iTemps % plan[iInter].size()] = iArc;
			
			if(iArc != -1) {
				if(!etat[iArc].empty()) {
					pair<int, int> voiture = etat[iArc].front(); etat[iArc].pop_front();
					
					if(voiture.second + 1 != (int)p.voitures[voiture.first].chemin.size()) {
						aAjouter[iTemps + 
							p.arcs[p.voitures[voiture.first].chemin[voiture.second + 1]].longueur
						].push_back({voiture.first, voiture.second + 1});
					}
				}
			}
		}
	}
	
	vector<vector<pair<int, int>>> sol(p.nbIntersections);
	
	for(int iInter = 0;iInter < p.nbIntersections;iInter++) {
		for(int elem : plan[iInter])
			sol[iInter].push_back({elem, 1});
	}
	
	return {sol};
}
