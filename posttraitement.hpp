#pragma once
#include <bits/stdc++.h>
#include "pretraitement.hpp"

int idRueOuverte(const vector<pair<int, int>> cycle, int temps)
{
	int curPos(0);
	while (curPos < (int)cycle.size() and cycle[curPos].second <= temps)
		temps -= cycle[curPos].second;
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

	for (int t(0); t < probleme.dureeSimulation; ++t)
	{
		for (int idVoiture(0); idVoiture < probleme.nbVoitures; ++idVoiture)
		{
			if (curPos[idVoiture].first == (int)voitures[idVoiture].chemin.size())
				continue;
			if (curPos[idVoiture].second == 1)
				queues[voitures[idVoiture].chemin[curPos[idVoiture].first]].push(idVoiture);
			if (curPos[idVoiture].second > 0)
				curPos[idVoiture].second--;
			if (curPos[idVoiture].second == 0)
			{
				Arc curArc = arcs[voitures[idVoiture].chemin[curPos[idVoiture].first]];
				
				if (idRueOuverte(sol.solution[curArc.iFin], t % dureeCycle[curArc.iFin]) != voitures[idVoiture].chemin[curPos[idVoiture].first])
					continue;
				if (queues[ voitures[idVoiture].chemin[curPos[idVoiture].first] ].front() != idVoiture)
					continue;
				queues[voitures[idVoiture].chemin[curPos[idVoiture].first]].pop();
				curPos[idVoiture].first++;
				if (curPos[idVoiture].first == (int)voitures[idVoiture].chemin.size())
					scoreTot += probleme.bonus + probleme.dureeSimulation - t-1;
				else
				{
					Arc nxtArc = arcs[voitures[idVoiture].chemin[curPos[idVoiture].first]];
					curPos[idVoiture].second = nxtArc.longueur;
				}
			}
		}
	}
	return scoreTot;
}
