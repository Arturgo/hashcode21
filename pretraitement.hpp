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
			}
			if (curVoiture.longueurChemin <= dureeSimulation)
			{
				voitures.push_back(curVoiture);
				for (int id : curVoiture.chemin)
					areteVue[id] = true;
			}
		}
		vector<int> nouvelId(nbRues);
		int curId(0);

		ofstream sortieVoitures(fichierEntree + ".infoVoitures", ios::out);
		int nbArcsInutiles(0);
		for (bool c : areteVue)
			nbArcsInutiles += !c;
		cerr << "Nombre d'arcs inutiles : " << nbArcsInutiles << endl;
		cerr << "Nombre de voitures inutiles : " << nbVoitures - voitures.size() << endl;
		sortieVoitures << voitures.size() << endl;
		for (auto voiture : voitures)
			sortieVoitures << voiture.longueurChemin << endl;
	}
};
