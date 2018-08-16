#include <algorithm>
#include <iostream>
#include <stdexcept>

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

ResultatLP analyseLP(const Chemin& chemin, const Vecteur<DemandePriorite>& demandesPriorite){
    // INITIALISATION

    // Définit le nombre de variables
    size_t colX = 1,            nX = chemin.size();
    size_t colU = colX + nX,    nU = chemin.size();
    size_t colR = colU + nU,    nR = demandesPriorite.size();
    size_t nCols = nX + nU + nR;

    if (nCols > ResultatLP::MAX_SIZE)
        throw domain_error("Nombre de variables du LP dépasse le maximum");

    // Crée le problème
    lprec *lp;
    lp = make_lp(0, nCols);

    RowLP row;


    // LIMITES DES VARIABLES

    // Variables X
    double LBPremierePhase = max(chemin.carrefour().tempsEcoule(), chemin.phase(0).dureeMinimale);
    set_bounds(lp, colX+0, LBPremierePhase, chemin.phase(0).dureeMaximale);
    for (size_t i = 1; i != nX; ++i)
        set_bounds(lp, colX+i, chemin.phase(i).dureeMinimale, chemin.phase(i).dureeMaximale);


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
    set_obj_fnex(lp, row.count(), row.values(), row.cols());


    // CONTRAINTES

    set_add_rowmode(lp, TRUE);

    // Contraintes des variables U
    for (Chemin::size_type i = 0; i != chemin.size(); ++i){
        // Equation 1
        row.clear();
        row.add(colX+i, 1);
        row.add(colU+i, -1);
        add_constraintex(lp, row.count(), row.values(), row.cols(), LE, chemin.phase(i).dureeNominale);

        // Equation 2
        row.clear();
        row.add(colX+i, 1);
        row.add(colU+i, 1);
        add_constraintex(lp, row.count(), row.values(), row.cols(), GE, chemin.phase(i).dureeNominale);
    }


    // RÉSOLUTION
    set_add_rowmode(lp, FALSE);
    set_verbose(lp, SEVERE);
    ResultatLP resultat;

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
    }

    // Supprime les donnés du problème
    delete_lp(lp);

    return resultat;
}

