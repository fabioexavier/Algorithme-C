#include <algorithm>
#include <chrono>
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

    // Début mésure de temps
    auto start = std::chrono::high_resolution_clock::now();

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
//        cout << *iChemin << endl << resultat << endl << endl;
    }

    // Trouve le meilleur chemin
    Vecteur<ResultatLP>::const_iterator iterMin = min_element(resultatsFaisables.begin(), resultatsFaisables.end(), ResultatLP::compare);
    size_t indexMin = iterMin - resultatsFaisables.begin();

    Vecteur<Chemin>::const_iterator iMeilleurChemin = iCheminsFaisables[indexMin];

    // Fin mésure de temps
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;

    cout << carrefour.demandesPriorite() << endl << endl
         << *iMeilleurChemin << endl << endl
         << *iterMin << endl << endl
         << "Chemins Analyses: " << cheminsPossibles.size() << endl << endl
         << "Temps: " << elapsed.count() << " s" << endl << endl;

    return 0;
}
