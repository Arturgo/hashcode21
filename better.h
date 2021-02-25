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
		int estOk = true;
		
		int somme = 0;
		for(Arc arc : parents[iInter]) {
			if(arc.nbPassent > 1)
				estOk = false;
			somme += arc.nbPassent;
		}
		
		if(parents[iInter].size() < 10) {
			for(Arc arc : parents[iInter]) {
				for(int iFois = 0;iFois < min(1, (60 * arc.nbPassent) / somme);iFois++) {
					plan[iInter].push_back(arc.id);
				}
			}
		}
		else {
			if(!estOk)
				plan[iInter] = vector<int>(parents[iInter].size(), -1);
			else
				plan[iInter] = vector<int>(p.dureeSimulation, -1);
		}
	}
	
	//schedule le reste
	
	vector<list<pair<int, int>>> etat(p.arcs.size());
	
	for(int iVoiture = 0;iVoiture < p.nbVoitures;iVoiture++) {
		etat[p.voitures[iVoiture].chemin[0]].push_back({iVoiture, 0});
	}
	
	vector<vector<pair<int, int>>> aAjouter(p.dureeSimulation + 100 * 1000);
	vector<set<int>> scheduled(p.nbIntersections);
	
	for(int iTemps = 0;iTemps < p.dureeSimulation;iTemps++) {	
		for(pair<int, int> ajout : aAjouter[iTemps]) {
			etat[p.voitures[ajout.first].chemin[ajout.second]].push_back(ajout);
		}
		
		for(int iInter = 0;iInter < p.nbIntersections;iInter++) {
			if(plan[iInter].size() == 0)
				continue;
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
		int derTemps = 0;
		int derVal = -1;
		
		for(int elem : plan[iInter]) {
			if(elem != -1) {
				if(derVal != -1) {
					sol[iInter].push_back({derVal, derTemps});
					derTemps = 0;
				}
				derVal = elem;
			}
			derTemps++;
		}
		if(derVal != -1)
			sol[iInter].push_back({derVal, derTemps});
	}
	
	cerr << "fini" << endl;
	
	return {sol};
}
