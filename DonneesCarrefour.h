#ifndef DONNEES_CARREFOUR_H
#define DONNEES_CARREFOUR_H

#include <iostream>

#include "Matrice.h"
#include "Graphe.h"
#include "Vecteur.h"

// Phase
struct Phase{
    int numero;
    int dureeMinimale;
    int dureeNominale;
    int dureeMaximale;
    bool escamotable;
    int code;
    bool exclusive;
    bool solicitee;

    Phase() : numero(0),
              dureeMinimale(0),
              dureeNominale(0),
              dureeMaximale(0),
              escamotable(false),
              code(0),
              exclusive(false),
              solicitee(true) {}

    Phase(int n, int dMin, int dNom, int dMax, bool esc, int c, bool exc, bool sol) :
          numero(n),
          dureeMinimale(dMin),
          dureeNominale(dNom),
          dureeMaximale(dMax),
          escamotable(esc),
          code(c),
          exclusive(exc),
          solicitee(sol){}

};
inline std::ostream& operator<<(std::ostream& os, const Phase& p){
    os << "Phase " << p.numero;
    return os;
}
inline operator==(const Phase& p1, const Phase& p2){
    return p1.numero == p2.numero;
}

// Interphase
struct Interphase{
    int origine;
    int destination;
    int duree;

    Interphase() : origine(0), destination(0), duree(0) {}
    Interphase(int ori, int dest, int dur) :
               origine(ori), destination(dest), duree(dur) {}
};
inline std::ostream& operator<<(std::ostream& os, const Interphase& ip){
    os << "Interphase (" << ip.origine << ", " << ip.destination << ")";
    return os;
}


// LigneDeFeu

// DemandePriorite
struct DemandePriorite{
    int delaiApproche;
    int code;

    DemandePriorite() : delaiApproche(0),
                        code(0) {}

    DemandePriorite(int d, int c) : delaiApproche(d),
                                             code(c) {}
};
inline std::ostream& operator<<(std::ostream& os, const DemandePriorite& d){
    os << "Delai " << d.delaiApproche << ", Code " << d.code;
    return os;
}

// Carrefour
class Carrefour{
public:
    Carrefour() : _phaseActuelle(0),
                  _tempsEcoule(0) {}

    Carrefour(const Vecteur<Phase>& p, const Matrice<Interphase>& mi, const Vecteur<DemandePriorite>& d,
              const Phase* pa, int te) :
              _phases(p),
              _interphases(mi),
              _demandes(d),
              _phaseActuelle(pa),
              _tempsEcoule(te) {}

    const Phase& phase(unsigned int i) const { return _phases[i]; }
    Vecteur<Phase>::size_type numPhases() const { return _phases.size(); }

    const Interphase& interphase(unsigned int i, unsigned int j) const { return _interphases.element(i,j); }
    const Interphase& interphase(const Phase& p1, const Phase& p2) const { return interphase(p1.numero, p2.numero); }

    const Vecteur<DemandePriorite>& demandesPriorite() const { return _demandes; }

    const Phase& phaseActuelle() const { return *_phaseActuelle; }
    int tempsEcoule() const { return _tempsEcoule; }

private:
    Vecteur<Phase> _phases;
    Matrice<Interphase> _interphases;
    Vecteur<DemandePriorite> _demandes;
    const Phase *_phaseActuelle;
    int _tempsEcoule;
};

#endif // DONNEES_CARREFOUR_H


