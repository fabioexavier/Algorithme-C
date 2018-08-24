#include <algorithm>

#include "AlgorithmePriorite.h"
#include "DonneesCarrefour.h"
#include "Graphe.h"
#include "Vecteur.h"

using std::cout;
using std::endl;
using std::max_element;

Vecteur<Chemin> rechercheChemins(const Carrefour& carrefour){
    Vecteur<Chemin> cheminsTrouves;

    // Calcule du graphe et initialisation du chemin
    Graphe<Phase> graphe = calcGraphe(carrefour);
    Chemin cheminBase(carrefour);

    // Recherche tous les chemins possibles r�cursivement
    rechercheRecursive(graphe, cheminBase, cheminsTrouves);

    return cheminsTrouves;
}

Graphe<Phase> calcGraphe(const Carrefour& carrefour){
    // Identifie les sommets du graphe
    Vecteur<Phase> sommets;
    for (size_t i=0; i!=carrefour.numPhases(); ++i){
        Phase phase = carrefour.phase(i);
        if (phase.solicitee || phase.code != 0){
            sommets.push_back(phase);
        }
    }

    // V�rifie les transitions possibles et assemble la matrice
    Matrice<bool> matriceTransitions(sommets.size(), sommets.size(), false);

    for (Vecteur<Phase>::const_iterator iActuelle=sommets.begin(); iActuelle!=sommets.end(); ++iActuelle){
        Vecteur<Phase>::const_iterator iSuivante = sommets.cyclicNext(iActuelle);

        bool continuer;
        do {
            // Emp�che des transitions entre deux phases exclusives de m�me code
            if ( !(iActuelle->exclusive && iSuivante->exclusive && iActuelle->code == iSuivante->code) ){
                size_t indexActuelle = iActuelle - sommets.begin();
                size_t indexSuivante = iSuivante - sommets.begin();
                matriceTransitions.element(indexActuelle, indexSuivante) = true;
            }
            continuer = iSuivante->escamotable;
            iSuivante = sommets.cyclicNext(iSuivante);
        } while (continuer && iSuivante != iActuelle);
    }

    // Cr�e l'objet graphe
    return Graphe<Phase>(sommets, matriceTransitions);
}

void rechercheRecursive(const Graphe<Phase>& graphe, const Chemin& chemin, Vecteur<Chemin>& cheminsTrouves){
    // Enregistre le chemin s'il est acceptable
    if (chemin.valide() )
        cheminsTrouves.push_back(chemin);

    // Teste si on est arriv� � la fin de la branche
    if (!finDeBranche(graphe, chemin) ){
        Vecteur<Phase> enfants = graphe.enfants(chemin.phase(-1) );

        // R�p�te pour chaque enfant vers lequel la transition est valide
        for (Vecteur<Phase>::const_iterator iEnfant=enfants.begin(); iEnfant!=enfants.end(); ++iEnfant){
            if (chemin.transitionPossible(*iEnfant) ){
                Chemin cheminDerive = chemin;
                cheminDerive.push_back(*iEnfant);

                rechercheRecursive(graphe, cheminDerive, cheminsTrouves);
            }
        }
    }
}

bool finDeBranche(const Graphe<Phase>& graphe, const Chemin& chemin){
    // On est arriv� � la fin de la branche si la somme des minis est au moins �gale au plus grand d�lai d'approche et
    // si le nombre de phases dans le chemin est au moins �gal au nombre de sommets du graphe

    Vecteur<DemandePriorite> demandesPriorite = chemin.carrefour().demandesPriorite();

    // Calcule plus grand delai d'approche
    Vecteur<DemandePriorite>::const_iterator iMax = max_element(demandesPriorite.begin(), demandesPriorite.end(), DemandePriorite::compareDelai);

//    int maxDelai = 0;
//    for (Vecteur<DemandePriorite>::const_iterator iDemande=demandesPriorite.begin();
//                                                  iDemande!=demandesPriorite.end(); ++iDemande){
//        if (iDemande->delaiApproche > maxDelai)
//            maxDelai = iDemande->delaiApproche;
//    }

    return (chemin.sommeMin() >= iMax->delaiApproche) && (chemin.size() >= graphe.sommets().size() );
}

