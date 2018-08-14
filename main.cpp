#include <iostream>

#include "AlgorithmePriorite.h"
#include "DonneesCarrefour.h"
#include "Vecteur.h"

using std::cout;
using std::endl;

int main(){
    // Phases
    const Phase arrPhases[] = { Phase(0, 14, 41, 60, false, 2, false, true),
                                Phase(1, 4, 30, 30, true, 1, true, false),
                                Phase(2, 11, 15, 40, false, 0, false, true),
                                Phase(3, 4, 30, 30, true, 1, true, false) };
    const size_t numPhases = sizeof(arrPhases)/sizeof(arrPhases[0]);
    const Vecteur<Phase> vecPhases(arrPhases, numPhases);

    // Interphases
    const unsigned int dureeInterphases[numPhases][numPhases] = { {0, 7, 7, 5},
                                                                  {8, 0, 7, 0},
                                                                  {8, 8, 0, 8},
                                                                  {5, 0, 7, 0} };
    Matrice<Interphase> matriceInterphases(numPhases, numPhases);
    for (size_t i=0; i!=numPhases; ++i)
        for (size_t j=0; j!=numPhases; ++j)
            matriceInterphases.element(i,j) = Interphase(i, j, dureeInterphases[i][j]);

    // Demandes Priorite
    const DemandePriorite arrDemandes[] = { DemandePriorite(30,1),
                                            DemandePriorite(75,2) };
    const Vecteur<DemandePriorite> vecDemandes(arrDemandes, sizeof(arrDemandes)/sizeof(arrDemandes[0]) );

    // Charge l'exemple de carrefour
    Carrefour carrefour(vecPhases, matriceInterphases, vecDemandes, &vecPhases[1], 5);

    // Calcule les chemins possibles
    Vecteur<Chemin> cheminsPossibles = rechercheChemins(carrefour);

    // Analyse chacun des chemins avec la LP
    Vecteur<Chemin> cheminsFaisables;
    Vecteur<ResultatLP> resultatsFaisables;

    for (Vecteur<Chemin>::const_iterator iChemin=cheminsPossibles.begin(); iChemin!=cheminsPossibles.end(); ++iChemin){
        ResultatLP resultat = analyseLP(*iChemin, carrefour.demandesPriorite());
        if (resultat){
            cheminsFaisables.push_back(*iChemin);
            resultatsFaisables.push_back(resultat);

            cout << *iChemin << endl << resultat << endl << endl;
        }
    }

    return 0;
}
