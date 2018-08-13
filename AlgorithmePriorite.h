#ifndef ALGORITHME_PRIORITE_H
#define ALGORITHME_PRIORITE_H

#include <iostream>

#include "DonneesCarrefour.h"
#include "Vecteur.h"

// Chemin
class Chemin{
public:
    typedef Vecteur<Phase>::const_iterator const_iterator;
    typedef Vecteur<Phase>::size_type size_type;

    Chemin() {}
    explicit Chemin(const Carrefour&);

    const Phase& phase(int n) const { return vecPhases[n]; }
    unsigned int sommeMin() const { return _sommeMin; }
    unsigned int comptageCode(unsigned int) const;
    unsigned int incrementeCode(unsigned int);

    const_iterator begin() const { return vecPhases.begin(); }
    const_iterator end() const { return vecPhases.end(); }
    size_type size() const { return vecPhases.size(); }
    void push_back(const Phase&);

    bool valide(const Vecteur<DemandePriorite>&) const;
    bool transitionPossible(const Phase&, const Vecteur<DemandePriorite>&) const;

private:
    Vecteur<Phase> vecPhases;
    unsigned int _sommeMin;
    Vecteur<unsigned int> vecComptageCodes;
    const Carrefour* pCarrefour;
};
std::ostream& operator<<(std::ostream&, const Chemin&);

// Fonctions
Vecteur<Chemin> rechercheChemins(const Carrefour&);

#endif // ALGORITHME_PRIORITE_H


