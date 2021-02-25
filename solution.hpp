#include <bits/stdc++.h>
#include "pretraitement.hpp"
using namespace std;

vector<vector<pair<int,int>>> calculerSolution(Probleme p){
    vector<vector<pair<int,int>>> reponse(p.nbIntersections);
    vector<vector<int> > nbPass(p.nbIntersections);
    for(int i=0;i<p.nbVoitures;i++){
        for(int el : p.voitures[i].chemin )
            nbPass[p.arcs[el].iFin].push_back(p.arcs[el].iDepart);
    }
    for(int i=0;i<p.nbIntersections;i++){
        sort(nbPass[i].begin(),nbPass[i].end());
        int nb=1, av=-1;
        for(int el:nbPass[i]){
            if(el==av){
                nb++;
            }
            else if(av!=-1){
                reponse[i].push_back(make_pair(av, nb));
                nb=1;
            }
            av=el;
        }
        if(nb)
            reponse[i].push_back(make_pair(av, nb));
    }
    return reponse;
}