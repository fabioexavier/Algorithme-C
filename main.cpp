#include <algorithm>
#include <iostream>
#include <vector>

#include "AlgorithmePriorite.h"
#include "DonneesCarrefour.h"

using std::cout;
using std::endl;
using std::min_element;
using std::vector;

int main(){
    // Charge un exemple de carrefour
    Carrefour carrefour;
    carrefour.loadExemple(1);

    // Calcule les chemins possibles
    vector<Chemin> cheminsPossibles;
    rechercheChemins(carrefour, cheminsPossibles);

    // Analyse chacun des chemins avec la LP
    vector< vector<Chemin>::const_iterator > iCheminsFaisables;
    vector<ResultatLP> resultatsFaisables;

    for (vector<Chemin>::const_iterator iChemin=cheminsPossibles.begin(); iChemin!=cheminsPossibles.end(); ++iChemin){
        ResultatLP resultat = analyseLP(*iChemin);
        if (resultat){
            iCheminsFaisables.push_back(iChemin);
            resultatsFaisables.push_back(resultat);
        }
        cout << *iChemin << endl << resultat << endl << endl;
    }

    // Trouve le meilleur chemin
    vector<ResultatLP>::const_iterator iterMin = min_element(resultatsFaisables.begin(), resultatsFaisables.end(), ResultatLP::compare);
    size_t indexMin = iterMin - resultatsFaisables.begin();

    vector<Chemin>::const_iterator iMeilleurChemin = iCheminsFaisables[indexMin];

    cout << endl << "Meilleur Chemin: " << endl << endl
         << *iMeilleurChemin << endl << endl
         << *iterMin << endl << endl
         << "Chemins Analyses: " << cheminsPossibles.size() << endl << endl
         << "Temps: " << endl << endl;

    return 0;
}
