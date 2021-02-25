#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Arc
{
	string identifiant;
	int iDepart, iFin;
	int longueur;
	
	int nbPassent;
};

struct Voiture
{
	vector<int> chemin;
	int longueurChemin;
};


struct Probleme
{
	int dureeSimulation;
	int nbIntersections;
	int nbRues;
	int nbVoitures;
	int bonus;
	string fichierEntree;

	vector<Voiture> voitures;
	vector<Arc> arcs;
	map<string, int> nomToId;

	Probleme() {}

	Probleme(string fichierEntree)
		: fichierEntree(fichierEntree)
	{
		ifstream entree(fichierEntree, ios::in);
		
		entree >> dureeSimulation >> nbIntersections >> nbRues >> 
			nbVoitures >> bonus;
		for (int i(0); i < nbRues; ++i)
		{
			Arc arc;
			arc.nbPassent = 0;
			entree >> arc.iDepart >> arc.iFin >> arc.identifiant >> arc.longueur;
			arcs.push_back(arc);
			nomToId[arc.identifiant] = i;
		}
		
		vector<int> nbPassages(nbIntersections, 0);
		for (int i(0); i < nbVoitures; ++i)
		{
			Voiture curVoiture;
			int nbRues;
			entree >> nbRues;
			curVoiture.chemin.resize(nbRues);
			curVoiture.longueurChemin = 0;
			for (int j(0); j < nbRues; ++j)
			{
				string nom;
				entree >> nom;
				int id = nomToId[nom];
				curVoiture.chemin[j] = id;
				curVoiture.longueurChemin += arcs[id].longueur;
				arcs[id].nbPassent++;
				nbPassages[arcs[id].iFin]++;
			}
			voitures.push_back(curVoiture);
		}
		
		int nbArcsInutiles(0);
		for(int iArc = 0;iArc < (int)arcs.size();iArc++)
			if(arcs[iArc].nbPassent == 0)
				nbArcsInutiles++;
		
		vector<int> nouvelId(nbRues,-1);
		vector<Arc> nouveauxArcs;
		int curId(0);
		for (int i(0); i < nbRues; ++i)
			if (arcs[i].nbPassent != 0)
			{
				nouvelId[i] = curId++;
				nouveauxArcs.push_back(arcs[i]);
			}
		arcs = move(nouveauxArcs);
		nbRues = (int)arcs.size();

		for (int i(0); i < (int)voitures.size(); ++i)
			for (auto &id : voitures[i].chemin)
			{
				assert(nouvelId[id] != -1);
				id = nouvelId[id];
			}
		for (auto v : voitures)
			for (auto id : v.chemin)
			{
				assert(id >= 0);
				assert(id < (int)arcs.size());
			}

		vector<int> degreEntrant(nbIntersections), degreSortant(nbIntersections);
		for (auto arc : arcs)
		{
			degreEntrant[arc.iFin]++;
			degreSortant[arc.iDepart]++;
		}

		int degEntrantMax(0), degSortantMax(0);
		for (auto d : degreEntrant)
			degEntrantMax = max(degEntrantMax, d);
		for (auto d : degreSortant)
			degSortantMax = max(degSortantMax, d);
		cerr << "Deg sortant max : " << degSortantMax << endl;
		cerr << "Deg entrant max : " << degEntrantMax << endl;

		ofstream sortieVoitures(fichierEntree + ".infoVoitures", ios::out);
		
		cerr << "Nombre d'arcs inutiles : " << nbArcsInutiles << endl;
		sortieVoitures << voitures.size() << endl;
		for (auto voiture : voitures)
			sortieVoitures << voiture.longueurChemin << endl;
			
		
		ofstream sortieSommets(fichierEntree + ".infoSommets", ios::out);
		sortieSommets << nbIntersections << endl;
		for (int iInter = 0;iInter < nbIntersections;iInter++)
			sortieSommets << nbPassages[iInter] << endl;
	}
};

Probleme probleme;

struct Solution
{
	vector< vector< pair<int, int > > > solution;
	
	void afficheSolution();
};

int idRueOuverte(const vector<pair<int, int>> cycle, int temps)
{
	int curPos(0);
	while (curPos < (int)cycle.size() and cycle[curPos].second <= temps)
		temps -= cycle[curPos++].second;
	assert(curPos < (int)cycle.size());
	return cycle[curPos].first;
}

int scoreSolution(const Solution &sol)
{
	int scoreTot(0);
	
	vector<queue<int>> queues(probleme.nbRues);
	vector<pair<int, int>> curPos(probleme.nbVoitures);
	vector<int> dureeCycle(probleme.nbIntersections);
	for (int i(0); i < probleme.nbIntersections; ++i)
		for (auto v : sol.solution[i])
			dureeCycle[i] += v.second;
	const auto &arcs = probleme.arcs;
	const auto &voitures = probleme.voitures;

	for (int idVoiture(0); idVoiture < probleme.nbVoitures; ++idVoiture)
	{
		curPos[idVoiture] = make_pair(0, 0);
		queues[voitures[idVoiture].chemin[0]].push(idVoiture);
	}

	for (int t(0); t <= probleme.dureeSimulation; ++t)
	{
		vector<int> toPop;
		for (int idVoiture(0); idVoiture < probleme.nbVoitures; ++idVoiture)
		{
			auto &[idChemin , lenRestante] = curPos[idVoiture];
			if (idChemin == (int)voitures[idVoiture].chemin.size())
				continue;
			int idArc = voitures[idVoiture].chemin[idChemin];
			if (lenRestante == 1)
				queues[idArc].push(idVoiture);
			if (lenRestante > 0)
				lenRestante--;
			if (!lenRestante)
			{
				Arc curArc = arcs[ voitures[idVoiture].chemin[idChemin]];
				//Arc curArc = arcs[idArc];
				if (queues[ idArc].front() != idVoiture)
					continue;
				assert( dureeCycle[curArc.iFin]);
				if (idRueOuverte(sol.solution[curArc.iFin], t % dureeCycle[curArc.iFin]) != idArc)
					continue;
				toPop.push_back(idArc);
				idChemin++;
				if (idChemin == (int)voitures[idVoiture].chemin.size())
					scoreTot += probleme.bonus + probleme.dureeSimulation - t;
				else
				{
					Arc nxtArc = arcs[voitures[idVoiture].chemin[idChemin]];
					lenRestante = nxtArc.longueur;
				}
			}
		}
		for (auto v : toPop)
			queues[v].pop();
	}
	return scoreTot;
}

void Solution::afficheSolution()
	{
		ofstream sortieSolution(probleme.fichierEntree + ".meilleureSolution", ios::out);
		int nbNonNuls(0);
		for (auto v : solution)
			nbNonNuls += !v.empty();
		sortieSolution << nbNonNuls << endl;
		cerr << "valeur de la solution : " << scoreSolution(*this);
		for (int id(0); id < probleme.nbIntersections; ++id)
		{
			if (solution[id].empty()) continue;
			sortieSolution << id << endl;

			sortieSolution << solution[id].size() << endl;
			for (auto [idChemin, duree] : solution[id])
			{
				//assert(idChemin < probleme.arcs.size());
				assert(0 <= idChemin);
				sortieSolution << probleme.arcs[idChemin].identifiant << ' ' << duree << endl;		
			}
		}
	}
