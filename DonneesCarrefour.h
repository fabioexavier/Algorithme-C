#ifndef DONNEES_CARREFOUR_H
#define DONNEES_CARREFOUR_H

#include <iostream>

#include "Matrice.h"
#include "Graphe.h"
#include "Vecteur.h"

// Phase
struct Phase{
    unsigned int numero;
    unsigned int dureeMinimale;
    unsigned int dureeNominale;
    unsigned int dureeMaximale;
    bool escamotable;
    unsigned int code;
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

    Phase(unsigned int n, unsigned int dMin, unsigned int dNom, unsigned int dMax, bool esc, unsigned int c,
          bool exc, bool sol) :
          numero(n),
          dureeMinimale(dMin),
          dureeNominale(dNom),
          dureeMaximale(dMax),
          escamotable(esc),
          code(c),
          exclusive(exc),
          solicitee(sol){}

};
std::ostream& operator<<(std::ostream&, const Phase&);
bool operator==(const Phase&, const Phase&);

// Interphase
struct Interphase{
    unsigned int origine;
    unsigned int destination;
    unsigned int duree;

    Interphase() : origine(0), destination(0), duree(0) {}
    Interphase(unsigned int ori, unsigned int dest, unsigned int dur) :
               origine(ori), destination(dest), duree(dur) {}
};
std::ostream& operator<<(std::ostream&, const Interphase&);


// LigneDeFeu

// DemandePriorite
struct DemandePriorite{
    int delaiApproche;
    unsigned int code;

    DemandePriorite() : delaiApproche(0),
                        code(0) {}

    DemandePriorite(int d, unsigned int c) : delaiApproche(d),
                                             code(c) {}
};
std::ostream& operator<<(std::ostream&, const DemandePriorite&);

// Carrefour
class Carrefour{
public:
    Carrefour() : _phaseActuelle(0),
                  _tempsEcoule(0) {}

    Carrefour(const Vecteur<Phase>& p, const Matrice<Interphase>& mi, const Vecteur<DemandePriorite>& d,
              const Phase* pa, unsigned int te) :
              vecteurPhases(p),
              matriceInterphases(mi),
              vecteurDemandes(d),
              _phaseActuelle(pa),
              _tempsEcoule(te) {}

    const Phase& phase(unsigned int i) const { return vecteurPhases[i]; }
    size_t numPhases() const { return vecteurPhases.size(); }

    const Interphase& interphase(unsigned int i, unsigned int j) const { return matriceInterphases.element(i,j); }
    const Interphase& interphase(const Phase& p1, const Phase& p2) const { return interphase(p1.numero, p2.numero); }

    const Vecteur<DemandePriorite>& demandesPriorite() const { return vecteurDemandes; }

    const Phase& phaseActuelle() const { return *_phaseActuelle; }
    unsigned int tempsEcoule() const { return _tempsEcoule; }

private:
    Vecteur<Phase> vecteurPhases;
    Matrice<Interphase> matriceInterphases;
    Vecteur<DemandePriorite> vecteurDemandes;
    const Phase *_phaseActuelle;
    unsigned int _tempsEcoule;
};

#endif // DONNEES_CARREFOUR_H


