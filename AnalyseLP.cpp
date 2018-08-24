#include <algorithm>
#include <iostream>
#include <stdexcept>

#include <stdio.h>

#include "AlgorithmePriorite.h"
#include "Vecteur.h"

#include "lp_lib.h"

using std::cout;
using std::domain_error;
using std::endl;
using std::max;
using std::runtime_error;

class RowLP{
public:
    static const size_t MAX_SIZE = ResultatLP::MAX_SIZE;

    RowLP() : _count(0) {}

    void add(int col, REAL value){
        if (_count == MAX_SIZE)
            throw runtime_error("RowLP deja plein");
        else
            _cols[_count] = col;
            _values[_count] = value;
            ++_count;
    }
    void clear() { _count = 0; }

    int* cols() { return _cols; }
    REAL* values() { return _values; }
    int count() { return _count; }

private:
    int _cols[MAX_SIZE];
    REAL _values[MAX_SIZE];
    int _count;
};

ResultatLP analyseLP(const Chemin& chemin){
    // INITIALISATION
    Vecteur<DemandePriorite> demandesPriorite = chemin.carrefour().demandesPriorite();

    // Variables X, U et R
    size_t colX = 1,                nX = chemin.size();
    size_t colU = colX + nX,        nU = chemin.size();
    size_t colR = colU + nU,        nR = demandesPriorite.size();

    // Partition des phases par rapport aux codes
    Vecteur<Vecteur<size_t> > P(chemin.numCodes() );

    for (int k = 0; k != chemin.numCodes(); ++k)
        for (size_t j = 0; j != chemin.size(); ++j)
            if (chemin.phase(j).code == k)
                P[k].push_back(j);

    // Variables H
    Vecteur<size_t> colH(demandesPriorite.size() ),     nH(demandesPriorite.size() );
    colH[0] = colR + nR;
    for (size_t i = 0; i != demandesPriorite.size(); ++i){
        int k = demandesPriorite[i].code;
        nH[i] = P[k].size();
        if (i != 0)
            colH[i] = colH[i-1] + nH[i-1];
    }

    // Nombre total de variables
    size_t nCols = nX + nU + nR + nH.somme();

    if (nCols > ResultatLP::MAX_SIZE)
        throw domain_error("Nombre de variables du LP dépasse le maximum");

    // Création du problème
    lprec *lp;
    lp = make_lp(0, nCols);

    RowLP row;


    // LIMITES DES VARIABLES

    // Variables X
    double LBPremierePhase = max(chemin.carrefour().tempsEcoule(), chemin.phase(0).dureeMinimale);
    set_bounds(lp, colX+0, LBPremierePhase, chemin.phase(0).dureeMaximale);
    for (size_t i = 1; i != nX; ++i)
        set_bounds(lp, colX+i, chemin.phase(i).dureeMinimale, chemin.phase(i).dureeMaximale);

    // Variables H
    for (size_t i = 0; i != demandesPriorite.size(); ++i){
        for (size_t j = 0; j != nH[i]; ++j)
            set_binary(lp, colH[i]+j, TRUE);
    }

    // FONCTION OBJECTIVE

    set_minim(lp);
    row.clear();
    // Coéficients des variables U de la fonction objective
    for (size_t i = 0; i != nU; ++i){
        if (chemin.phase(i).exclusive)
            row.add(colU+i, 2);
        else
            row.add(colU+i, 1);
    }
    // Coéficients des variables R de la fonction objective
    for (size_t i = 0; i != nR; ++i){
        row.add(colR+i, 100);
    }

    set_obj_fnex(lp, row.count(), row.values(), row.cols() );


    // CONTRAINTES

    set_add_rowmode(lp, TRUE);
    const int M = 1000; // Big M

    // Contraintes des variables U
    for (size_t i = 0; i != chemin.size(); ++i){
        // Equation 1 : Xi - Ui <= NOMi
        row.clear();
        row.add(colX+i, 1);
        row.add(colU+i, -1);
        add_constraintex(lp, row.count(), row.values(), row.cols(), LE, chemin.phase(i).dureeNominale);

        // Equation 2 : Xi + Ui >= NOMi
        row.clear();
        row.add(colX+i, 1);
        row.add(colU+i, 1);
        add_constraintex(lp, row.count(), row.values(), row.cols(), GE, chemin.phase(i).dureeNominale);
    }

    // Contraintes de délai d'approche
    for (size_t i = 0; i != demandesPriorite.size(); ++i){ // Pour chaque véhicule i
        int k = demandesPriorite[i].code;

        for (size_t j = 0; j != P[k].size(); ++j){ // Pour chaque possible position de phase de passage P[k][j]
            // Somme des durées des interphases
            int sommeInterphases = 0;
            for (size_t m = 0; m != P[k][j]; ++m)
                sommeInterphases += chemin.carrefour().interphase(chemin.phase(m), chemin.phase(m+1) ).duree;
            int rhs;

            // Equation 1
            row.clear();
            for (size_t m = 0; m != P[k][j]; ++m)
                row.add(colX+m, 1);
            row.add(colR+i, -1);
            row.add(colH[i]+j, M);
            rhs = demandesPriorite[i].delaiApproche + chemin.carrefour().tempsEcoule() - sommeInterphases + M;
            add_constraintex(lp, row.count(), row.values(), row.cols(), LE, rhs);

            // Equation 2
            row.clear();
            for (size_t m = 0; m != P[k][j]+1; ++m)
                row.add(colX+m, 1);
            row.add(colR+i, -1);
            row.add(colH[i]+j, -M);
            rhs = demandesPriorite[i].delaiApproche + chemin.carrefour().tempsEcoule()
                    - sommeInterphases - M + chemin.phase(P[k][j]).marge;
            add_constraintex(lp, row.count(), row.values(), row.cols(), GE, rhs);
        }
        // Equation finale
        row.clear();
        for (size_t j = 0; j != P[k].size(); ++j)
            row.add(colH[i]+j, 1);
        add_constraintex(lp, row.count(), row.values(), row.cols(), EQ, 1);
    }

    // Contraintes de au moins un véhicule par phase exclusive / dernière phase
    for (size_t p = 1; p != chemin.size(); ++p){ // Ignore la première phase
        if (chemin.phase(p).exclusive || p == chemin.size()-1){
            row.clear();
            int k = chemin.phase(p).code;
            size_t j = P[k].index(p);
            for (size_t i = 0; i != demandesPriorite.size(); ++i){
                if (demandesPriorite[i].code == k)
                    row.add(colH[i]+j, 1);
            }
            add_constraintex(lp, row.count(), row.values(), row.cols(), GE, 1);
        }
    }

    // Contraintes de max 120s de rouge
    for (size_t i = 0; i != chemin.carrefour().numLignes(); ++i){
        if (chemin.carrefour().ligne(i).solicitee && chemin.carrefour().ligne(i).rouge ){
            int numPhases = 1; // Nombre de phases (fermées) du chemin jusqu'à l'ouverture de la ligne
            int sommeInterphases = 0; // Somme des durées des interphases jusqu'à l'ouverture de la ligne
            int sommeNominales = 0; // Somme des durées nominales des phases hors chemin jusqu'à l'ouverture de la ligne
            bool ouvertureTrouve = false; // Indique si l'instant d'ouverture de la ligne a déjà été trouvé

            // Analyse les phases dans le chemin
            for (size_t j = 1; j != chemin.size(); ++j){
                sommeInterphases += chemin.carrefour().interphase(chemin.phase(j-1), chemin.phase(j) ).duree;
                if (chemin.phase(j).lignesOuvertes[i]){
                    ouvertureTrouve = true;
                    break;
                }
                else
                    ++numPhases;
            }

            // Si la ligne n'ouvre pas dans le chemin
            if (!ouvertureTrouve){
                Phase phase1 = chemin.phase(-1);

                // Calcule la phase qui vient immédiatement après la fin du chemin
                Phase phase2 = chemin.carrefour().phaseSuivante(phase1);
                while (!phase2.solicitee)
                    phase2 = chemin.carrefour().phaseSuivante(phase2);

                sommeInterphases += chemin.carrefour().interphase(phase1, phase2).duree;

                // Analyse la séquence de phases hors chemin jusqu'à l'ouverture de la ligne
                while (!phase2.lignesOuvertes[i]){
                    phase1 = phase2;
                    do
                        phase2 = chemin.carrefour().phaseSuivante(phase2);
                    while (!phase2.solicitee);

                    sommeInterphases += chemin.carrefour().interphase(phase1, phase2).duree;
                    sommeNominales += phase1.dureeNominale;
                }
            }

                // Ecrit la contrainte
                row.clear();
                for (int k = 0; k != numPhases; ++k)
                    row.add(colX+k, 1);
                int rhs = 120 - chemin.carrefour().ligne(i).rougeAccumule + chemin.carrefour().tempsEcoule()
                          - sommeInterphases - sommeNominales;
                add_constraintex(lp, row.count(), row.values(), row.cols(), LE, rhs);
        }
    }

    // Contraintes d'intervalle entre véhicules dans la même phase exclusive
    for (size_t m = 0; m != demandesPriorite.size()-1; ++m){
        for (size_t n = m+1; n != demandesPriorite.size(); ++n){
            int k = demandesPriorite[m].code;
            if (demandesPriorite[n].code == k){
                for (size_t j = 0; j != P[k].size(); ++j){
                    int intervalle = chemin.phase(P[k][j]).intervalle;
                    if (chemin.phase(P[k][j]).exclusive && intervalle >= 0){
                        int rhs;

                        // Equation 1
                        row.clear();
                        row.add(colR+m, 1);
                        row.add(colR+n, -1);
                        row.add(colH[m]+j, M);
                        row.add(colH[n]+j, M);
                        rhs = intervalle + 2*M - demandesPriorite[m].delaiApproche
                                               + demandesPriorite[n].delaiApproche;
                        add_constraintex(lp, row.count(), row.values(), row.cols(), LE, rhs);

                        // Equation 2
                        row.clear();
                        row.add(colR+m, -1);
                        row.add(colR+n, 1);
                        row.add(colH[m]+j, M);
                        row.add(colH[n]+j, M);
                        rhs = intervalle + 2*M + demandesPriorite[m].delaiApproche
                                               - demandesPriorite[n].delaiApproche;
                        add_constraintex(lp, row.count(), row.values(), row.cols(), LE, rhs);
                    }
                }
            }
        }
    }

    // RÉSOLUTION
    set_add_rowmode(lp, FALSE);
    set_verbose(lp, IMPORTANT);
    ResultatLP resultat;

//    write_LP(lp, stdout);

    int status = solve(lp);

    if (status == OPTIMAL){
        resultat.optimumTrouve = true;

        REAL variables[100+1];
        get_variables(lp, variables+1); // Ignore la position 0 pour suivre les mêmes conventions de la librairie

        // Variables X
        for (size_t i = 0; i != nX; ++i)
            resultat.durees.push_back(variables[colX+i]);
        // Variables U
        for (size_t i = 0; i != nU; ++i)
            resultat.deviations.push_back(variables[colU+i]);
        // Variables R
        for (size_t i = 0; i != nR; ++i)
            resultat.retards.push_back(variables[colR+i]);
        // Calcule score final
        resultat.calcScore();
    }

    // Supprime les donnés du problème
    delete_lp(lp);

    return resultat;
}

