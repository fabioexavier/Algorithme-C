#ifndef ALGORITHME_PRIORITE_H
#define ALGORITHME_PRIORITE_H

#include <iostream>

#include "DonneesCarrefour.h"
#include "Vecteur.h"

// Classes
class Chemin{
public:
    typedef Vecteur<Phase>::const_iterator const_iterator;
    typedef Vecteur<Phase>::size_type size_type;

    Chemin() {}
    explicit Chemin(const Carrefour&);

    const Phase& phase(int n) const { return _phases[n]; }
    int sommeMin() const { return _sommeMin; }
    int comptageCode(int) const;
    int incrementeCode(int);

    const Carrefour& carrefour() const { return *_pCarrefour; }

    const_iterator begin() const { return _phases.begin(); }
    const_iterator end() const { return _phases.end(); }
    size_type size() const { return _phases.size(); }
    void push_back(const Phase&);

    bool valide(const Vecteur<DemandePriorite>&) const;

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
    Vecteur<int> durees;
    Vecteur<int> deviations;
    Vecteur<int> retards;

    ResultatLP() : optimumTrouve(false) {}
    ResultatLP(bool ot, const Vecteur<int>& dur, const Vecteur<int>& dev, const Vecteur<int>& ret) :
        optimumTrouve(ot),
        durees(dur),
        deviations(dev),
        retards(ret) {}

    operator bool(){ return optimumTrouve; }
};
inline std::ostream& operator<<(std::ostream& os, const ResultatLP& resultat){
    os << "Durees: " << resultat.durees << std::endl;
    os << "Deviations: " << resultat.deviations << std::endl;
    os << "Retards: " << resultat.retards;
    return os;
}

// Fonctions Principales
Vecteur<Chemin> rechercheChemins(const Carrefour&);
ResultatLP analyseLP(const Chemin&, const Vecteur<DemandePriorite>&);

// Fonctions Auxiliaires
Graphe<Phase> calcGraphe(const Carrefour&);
void rechercheRecursive(const Graphe<Phase>&, const Chemin&, const Vecteur<DemandePriorite>&, Vecteur<Chemin>&);
bool finDeBranche(const Graphe<Phase>&, const Chemin&, const Vecteur<DemandePriorite>&);
bool transitionPossible(const Chemin&, const Phase&, const Vecteur<DemandePriorite>&);

#endif // ALGORITHME_PRIORITE_H


