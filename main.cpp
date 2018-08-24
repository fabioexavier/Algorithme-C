#include <algorithm>
#include <iostream>

#include "AlgorithmePriorite.h"
#include "DonneesCarrefour.h"
#include "Vecteur.h"

using std::cout;
using std::endl;
using std::min_element;

int main(){
    // Charge un exemple de carrefour
    Carrefour carrefour;
    carrefour.loadExemple(1);

    // Calcule les chemins possibles
    Vecteur<Chemin> cheminsPossibles = rechercheChemins(carrefour);

    // Analyse chacun des chemins avec la LP
    Vecteur< Vecteur<Chemin>::const_iterator > iCheminsFaisables;
    Vecteur<ResultatLP> resultatsFaisables;

    for (Vecteur<Chemin>::const_iterator iChemin=cheminsPossibles.begin(); iChemin!=cheminsPossibles.end(); ++iChemin){
        ResultatLP resultat = analyseLP(*iChemin);
        if (resultat){
            iCheminsFaisables.push_back(iChemin);
            resultatsFaisables.push_back(resultat);
        }
        cout << *iChemin << endl << resultat << endl << endl;
    }

    // Trouve le meilleur chemin
    Vecteur<ResultatLP>::const_iterator iterMin = min_element(resultatsFaisables.begin(), resultatsFaisables.end(), ResultatLP::compare);
    size_t indexMin = iterMin - resultatsFaisables.begin();

    Vecteur<Chemin>::const_iterator iMeilleurChemin = iCheminsFaisables[indexMin];

    cout << endl << "Meilleur Chemin: " << endl << *iMeilleurChemin << endl << *iterMin << endl
         << "Chemins Analyses: " << cheminsPossibles.size() << endl << endl;

    return 0;
}
