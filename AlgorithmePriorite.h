#ifndef ALGORITHME_PRIORITE_H
#define ALGORITHME_PRIORITE_H

#include <iostream>

#include "DonneesCarrefour.h"
#include "Vecteur.h"

// Classes
class Chemin{
public:
    typedef Vecteur<Phase>::const_iterator const_iterator;

    Chemin() {}
    explicit Chemin(const Carrefour&);

    const Phase& phase(int n) const { return _phases[n]; }
    int sommeMin() const { return _sommeMin; }
    int comptageCode(int) const;
    int incrementeCode(int);
    int numCodes() const { return _comptageCodes.size(); }

    const Carrefour& carrefour() const { return *_pCarrefour; }

    const_iterator begin() const { return _phases.begin(); }
    const_iterator end() const { return _phases.end(); }
    size_t size() const { return _phases.size(); }
    void push_back(const Phase&);

    bool valide() const;
    bool transitionPossible(const Phase&) const;

private:
    Vecteur<Phase> _phases;
    int _sommeMin;
    Vecteur<int> _comptageCodes;
    const Carrefour* _pCarrefour;
};
std::ostream& operator<<(std::ostream&, const Chemin&);

struct ResultatLP{
public:
    static const size_t MAX_SIZE = Vecteur<int>::MAX_SIZE;
    bool optimumTrouve;
    Vecteur<double> durees;
    Vecteur<double> deviations;
    Vecteur<double> retards;
    double score;

    ResultatLP() : optimumTrouve(false), score(-1) {}
    void calcScore() { score = deviations.somme()+100*retards.somme(); }

    operator bool(){ return optimumTrouve; }
    static bool compare(const ResultatLP& r1, const ResultatLP& r2) { return r1.score < r2.score; }
};
inline std::ostream& operator<<(std::ostream& os, const ResultatLP& resultat){
        os << "Durees: " << resultat.durees << std::endl;
        os << "Deviations: " << resultat.deviations << std::endl;
        os << "Retards: " << resultat.retards << std::endl;
        os << "Score: " << resultat.score;
        return os;
    }




// Fonctions Principales
Vecteur<Chemin> rechercheChemins(const Carrefour&);
ResultatLP analyseLP(const Chemin&);

// Fonctions Auxiliaires
Graphe<Phase> calcGraphe(const Carrefour&);
void rechercheRecursive(const Graphe<Phase>&, const Chemin&, Vecteur<Chemin>&);
bool finDeBranche(const Graphe<Phase>&, const Chemin&);

#endif // ALGORITHME_PRIORITE_H


