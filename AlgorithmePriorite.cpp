#include <iostream>

#include "AlgorithmePriorite.h"
#include "DonneesCarrefour.h"
#include "Graphe.h"
#include "Vecteur.h"

using std::cout;         using std::endl;

// Recherche des chemins
Graphe<Phase> calcGraphe(const Carrefour&);
void rechercheRecursive(const Graphe<Phase>&, const Chemin&, const Vecteur<DemandePriorite>&, Vecteur<Chemin>&);
bool finDeBranche(const Graphe<Phase>&, const Chemin&, const Vecteur<DemandePriorite>&);

Vecteur<Chemin> rechercheChemins(const Carrefour& carrefour){
    Vecteur<Chemin> cheminsTrouves;

    // Calcule du graphe et initialisation du chemin
    Graphe<Phase> graphe = calcGraphe(carrefour);
    Chemin cheminBase(carrefour);

    // Recherche tous les chemins possibles récursivement
    rechercheRecursive(graphe, cheminBase, carrefour.demandesPriorite(), cheminsTrouves);

    return cheminsTrouves;
}

Graphe<Phase> calcGraphe(const Carrefour& carrefour){
    // Identifie les sommets du graphe
    Vecteur<Phase> sommets;
    for (size_t i=0; i!=carrefour.numPhases(); ++i){
        Phase phase = carrefour.phase(i);
        if (carrefour.phase(i).solicitee || carrefour.phase(i).code != 0){
            sommets.push_back(phase);
        }
    }

    // Vérifie les transitions possibles et assemble la matrice
    Matrice<bool> matriceTransitions(sommets.size(), sommets.size(), false);

    for (Vecteur<Phase>::const_iterator iterActuelle=sommets.begin(); iterActuelle!=sommets.end(); ++iterActuelle){
        Vecteur<Phase>::const_iterator iterSuivante = sommets.cyclicNext(iterActuelle);

        bool continuer;
        do {
            // Empêche des transitions entre deux phases exclusives de même code
            if ( !(iterActuelle->exclusive && iterSuivante->exclusive && iterActuelle->code == iterSuivante->code) ){
                size_t indexActuelle = iterActuelle - sommets.begin();
                size_t indexSuivante = iterSuivante - sommets.begin();
                matriceTransitions.element(indexActuelle, indexSuivante) = true;
            }
            continuer = iterSuivante->escamotable;
            iterSuivante = sommets.cyclicNext(iterSuivante);
        } while (continuer && iterSuivante != iterActuelle);
    }

    // Crée l'objet graphe
    return Graphe<Phase>(sommets, matriceTransitions);
}

void rechercheRecursive(const Graphe<Phase>& graphe, const Chemin& chemin,
                        const Vecteur<DemandePriorite>& demandesPriorite, Vecteur<Chemin>& cheminsTrouves){

    // Enregistre le chemin s'il est acceptable
    if (chemin.valide(demandesPriorite) )
        cheminsTrouves.push_back(chemin);

    // Teste si on est arrivé à la fin de la branche
    if (!finDeBranche(graphe, chemin, demandesPriorite)){
        Vecteur<Phase> enfants = graphe.enfants(chemin.phase(-1));

        // Répète pour chaque enfant vers lequel la transition est valide
        for (Vecteur<Phase>::const_iterator enfant=enfants.begin(); enfant!=enfants.end(); ++enfant){
            if (chemin.transitionPossible(*enfant, demandesPriorite) ){
                Chemin cheminDerive = chemin;
                cheminDerive.push_back(*enfant);

                rechercheRecursive(graphe, cheminDerive, demandesPriorite, cheminsTrouves);
            }
        }
    }
}

// On est arrivé à la fin de la branche si la somme des minis est au moins égale au plus grand délai d'approche et
// si le nombre de phases dans le chemin est au moins égale au nombre de sommets du graphe
bool finDeBranche(const Graphe<Phase>& graphe, const Chemin& chemin, const Vecteur<DemandePriorite>& demandesPriorite){
    // Calcule plus grand delai d'approche
    int maxDelai = 0;
    for (Vecteur<DemandePriorite>::const_iterator demande=demandesPriorite.begin();
                                                  demande!=demandesPriorite.end(); ++demande){
        if (demande->delaiApproche > maxDelai)
            maxDelai = demande->delaiApproche;
    }

    return (chemin.sommeMin() >= maxDelai) && (chemin.size() >= graphe.sommets().size() );

    return false;
}
