#include <bits/stdc++.h>
#include "pretraitement.hpp"
using namespace std;

vector<vector<pair<int, int>>> calculerSolution(Probleme p) {
	vector<Arc> parents(p.nbIntersections);
	
	for(Arc arc : p.arcs) {
		parents[arc.fin].push_back(arc);
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
	
	for(int iTemps = 0;iTemps < p.dureeSimulation;iTemps++) {		
		for(pair<int, int> ajout : aAjouter[iTemps]) {
			etat[p.voitures[p.first].chemin[ajout.second]].push_back(ajout);
		}
		
		for(int iInter = 0;iInter < p.nbIntersections;iInter++) {
			int iArc = plan[iInter][iTemps * plan[iInter].size()];
			
			if(iArc == -1) {
				for(int parent : parents[iInter]) {
					if(!etat[parent].empty()) {
						iArc = parent;
						break;
					}
				}
				
				
			}
			
			plan[iInter][iTemps * plan[iInter].size()] = iArc;
			
			if(iArc != -1) {
				if(!etat[iArc].empty()) {
					pair<int, int> voiture = etat[iArc].front(); etat[iArc].pop_front();
					
					if(voiture.second + 1 != p.voitures[voiture.first].chemin.size()) {
						aAjouter[iTemps + 
							p.arcs[p.voitures[voiture.first].chemin[voiture.second + 1]].longueur
						].push_back({voiture.first, voiture.second + 1});
					}
				}
			}
		}
		
		etat = nEtat;
	}
	
	vector<vector<pair<int, int>>> sol(p.nbIntersections);
	
	for(int iInter = 0;iInter < nbInters;iInter++) {
		for(int elem : plan[iInter]) {
			sol[iInter].push_back({elem, 0});
	}
	
	return sol;
}
