#include <algorithm>
#include <iostream>

#include "AlgorithmePriorite.h"
#include "DonneesCarrefour.h"
#include "Graphe.h"
#include "Vecteur.h"

using std::cout;
using std::endl;
using std::find;

Graphe<Phase> calcGraphe(const Carrefour&);
void rechercheRecursive(const Graphe<Phase>&, const Chemin&, const Vecteur<DemandePriorite>&, Vecteur<Chemin>&);
bool finDeBranche(const Graphe<Phase>&, const Chemin&, const Vecteur<DemandePriorite>&);
bool transitionPossible(const Chemin&, const Phase&, const Vecteur<DemandePriorite>&);

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
        if (phase.solicitee || phase.code != 0){
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
            if (transitionPossible(chemin, *enfant, demandesPriorite) ){
                Chemin cheminDerive = chemin;
                cheminDerive.push_back(*enfant);

                rechercheRecursive(graphe, cheminDerive, demandesPriorite, cheminsTrouves);
            }
        }
    }
}

bool finDeBranche(const Graphe<Phase>& graphe, const Chemin& chemin, const Vecteur<DemandePriorite>& demandesPriorite){
    // On est arrivé à la fin de la branche si la somme des minis est au moins égale au plus grand délai d'approche et
    // si le nombre de phases dans le chemin est au moins égal au nombre de sommets du graphe

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

bool transitionPossible(const Chemin& chemin, const Phase& phase, const Vecteur<DemandePriorite>& demandesPriorite){
    bool ret = true;
    if (phase.escamotable){
        // Phase ESC
        if (phase.code == 0){
            // Si le phase est deja dans le chemin
            if (find(chemin.begin(), chemin.end(), phase) != chemin.end())
                ret = false;
        }
        // Phase PEE ou PENE
        else{
            int code = phase.code;

            // Nombre max admissible de phases avec ce code dans le chemin
            // Admet une phase en plus si la première phase est PEE et a le même code
            int maxPhases = chemin.phase(0).exclusive && (chemin.phase(0).code == code) ? 1 : 0;
            // +1 pour chaque demande avec le même code
            for (Vecteur<DemandePriorite>::const_iterator demande=demandesPriorite.begin();
                                                          demande!=demandesPriorite.end(); ++demande){
                if (demande->code == code)
                    ++maxPhases;
            }
            // +1 pour chaque phase PENE distincte, solicitée, avec le même code et dans le chemin
            for (Vecteur<Phase>::size_type i=0; i!=chemin.carrefour().numPhases(); ++i){
                if (!chemin.carrefour().phase(i).exclusive &&
                    chemin.carrefour().phase(i).solicitee &&
                    chemin.carrefour().phase(i).code == phase.code &&
                    find(chemin.begin(), chemin.end(), chemin.carrefour().phase(i)) != chemin.end() )
                    ++maxPhases;
            }

            // Empêche la transition si le nombre max de phases est déjà atteint
            if (chemin.comptageCode(code) == maxPhases)
                ret = false;
        }
    }
    return ret;
}

