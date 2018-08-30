#include <algorithm>
#include <vector>

#include "AlgorithmePriorite.h"
#include "AuxVector.h"
#include "DonneesCarrefour.h"
#include "Graphe.h"

using std::cout;
using std::endl;
using std::max_element;
using std::vector;

void rechercheChemins(const Carrefour& carrefour, vector<Chemin>& cheminsTrouves){
    cheminsTrouves.clear();

    // Calcule du graphe et initialisation du chemin
    Graphe<Phase> graphe = calcGraphe(carrefour);
    Chemin cheminBase(carrefour);

    // Recherche tous les chemins possibles récursivement
    rechercheRecursive(graphe, cheminBase, cheminsTrouves);
}

Graphe<Phase> calcGraphe(const Carrefour& carrefour){
    // Identifie les sommets du graphe
    vector<Phase> sommets;
    for (size_t i=0; i!=carrefour.numPhases(); ++i){
        Phase phase = carrefour.phase(i);
        if (phase.solicitee || phase.code != 0){
            sommets.push_back(phase);
        }
    }

    // Vérifie les transitions possibles et assemble la matrice
    Matrice<int> matriceTransitions(sommets.size(), sommets.size(), 0);

    for (vector<Phase>::const_iterator iActuelle=sommets.begin(); iActuelle!=sommets.end(); ++iActuelle){
        vector<Phase>::const_iterator iSuivante = cyclicNext(iActuelle, sommets);

        bool continuer;
        do {
            // Empêche des transitions entre deux phases exclusives de même code
            if ( !(iActuelle->exclusive && iSuivante->exclusive && iActuelle->code == iSuivante->code) ){
                size_t indexActuelle = iActuelle - sommets.begin();
                size_t indexSuivante = iSuivante - sommets.begin();
                matriceTransitions.element(indexActuelle, indexSuivante) = 1;
            }
            continuer = iSuivante->escamotable;
            iSuivante = cyclicNext(iSuivante, sommets);
        } while (continuer && iSuivante != iActuelle);
    }

    // Crée l'objet graphe
    cout << sommets << endl;
    cout << matriceTransitions << endl;
    return Graphe<Phase>(sommets, matriceTransitions);
}

void rechercheRecursive(const Graphe<Phase>& graphe, const Chemin& chemin, vector<Chemin>& cheminsTrouves){
    // Enregistre le chemin s'il est acceptable
    if (chemin.valide() )
        cheminsTrouves.push_back(chemin);

    // Teste si on est arrivé à la fin de la branche
    if (!finDeBranche(graphe, chemin) ){
        vector<Phase> enfants = graphe.enfants(chemin.phase(-1) );

        // Répète pour chaque enfant vers lequel la transition est valide
        for (vector<Phase>::const_iterator iEnfant=enfants.begin(); iEnfant!=enfants.end(); ++iEnfant){
            if (chemin.transitionPossible(*iEnfant) ){
                Chemin cheminDerive = chemin;
                cheminDerive.push_back(*iEnfant);

                rechercheRecursive(graphe, cheminDerive, cheminsTrouves);
            }
        }
    }
}

bool finDeBranche(const Graphe<Phase>& graphe, const Chemin& chemin){
    // On est arrivé à la fin de la branche si la somme des minis est au moins égale au plus grand délai d'approche et
    // si le nombre de phases dans le chemin est au moins égal au nombre de sommets du graphe

    vector<DemandePriorite> demandesPriorite = chemin.carrefour().demandesPriorite();

    // Calcule plus grand delai d'approche
    vector<DemandePriorite>::const_iterator iMax =
        max_element(demandesPriorite.begin(), demandesPriorite.end(), DemandePriorite::compareDelai);

    return (chemin.sommeMin() >= iMax->delaiApproche) && (chemin.size() >= graphe.sommets().size() );
}

