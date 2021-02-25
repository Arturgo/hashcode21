#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Arc
{
	string identifiant;
	int iDepart, iFin;
	int longueur;
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

	vector<Voiture> voitures;
	vector<Arc> arcs;
	map<string, int> nomToId;

	Probleme(string fichierEntree)
	{
		ifstream entree(fichierEntree, ios::in);
		
		entree >> dureeSimulation >> nbIntersections >> nbRues >> 
			nbVoitures >> bonus;
		for (int i(0); i < nbRues; ++i)
		{
			Arc arc;
			entree >> arc.iDepart >> arc.iFin >> arc.identifiant >> arc.longueur;
			arcs.push_back(arc);
			nomToId[arc.identifiant] = i;
		}

		vector<bool> areteVue(nbRues);

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
				areteVue[id] = true;
			}
			voitures.push_back(curVoiture);
		}
		vector<int> nouvelId(nbRues,-1);
		vector<Arc> nouveauxArcs;
		int curId(0);
		for (int i(0); i < nbRues; ++i)
			if (areteVue[i])
			{
				nouvelId[i] = curId++;
				nouveauxArcs.push_back(arcs[i]);
			}
		arcs = move(nouveauxArcs);

		for (int i(0); i < (int)voitures.size(); ++i)
			for (auto &id : voitures[i].chemin)
			{
				assert(nouvelId[id] != -1);
				id = nouvelId[id];
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
		int nbArcsInutiles(0);
		for (bool c : areteVue)
			nbArcsInutiles += !c;
		cerr << "Nombre d'arcs inutiles : " << nbArcsInutiles << endl;
		sortieVoitures << voitures.size() << endl;
		for (auto voiture : voitures)
			sortieVoitures << voiture.longueurChemin << endl;
	}
};
