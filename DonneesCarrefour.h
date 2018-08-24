#ifndef DONNEES_CARREFOUR_H
#define DONNEES_CARREFOUR_H

#include <iostream>
#include <string>

#include "Matrice.h"
#include "Graphe.h"
#include "Vecteur.h"

// Phase
struct Phase{
    int numero;
    Vecteur<bool> lignesOuvertes;
    int dureeMinimale;
    int dureeNominale;
    int dureeMaximale;
    bool escamotable;
    int code;
    bool exclusive;
    bool solicitee;
    int marge;
    int intervalle;

    Phase() {}

    Phase(int n, std::string lignes, int dMin, int dNom, int dMax, bool esc, int c, bool exc, bool sol, int mar, int inter) :
          numero(n),
          dureeMinimale(dMin),
          dureeNominale(dNom),
          dureeMaximale(dMax),
          escamotable(esc),
          code(c),
          exclusive(exc),
          solicitee(sol),
          marge(mar),
          intervalle (inter) {

        for (std::string::const_iterator iChar = lignes.begin(); iChar != lignes.end(); ++iChar){
            switch(*iChar){
            case 'T':
                lignesOuvertes.push_back(true);
                break;
            case 'F':
                lignesOuvertes.push_back(false);
                break;
            default:
                throw std::domain_error(" Inicialisation incorrecte d'objet phase");
            }
        }
    }
};
inline std::ostream& operator<<(std::ostream& os, const Phase& p){
    os << "Phase " << p.numero;
    return os;
}
inline bool operator==(const Phase& p1, const Phase& p2){
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
struct LigneDeFeu{
    bool solicitee;
    bool rouge;
    int rougeAccumule;

    LigneDeFeu() {}

    LigneDeFeu(bool sol, bool rg, int rgAcc) :
        solicitee(sol),
        rouge(rg),
        rougeAccumule(rgAcc) {}
};

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

    void loadExemple(int);

    const LigneDeFeu& ligne(size_t i) const { return _lignes[i]; }
    size_t numLignes() const { return _lignes.size(); }

    const Phase& phase(size_t i) const { return _phases[i]; }
    size_t numPhases() const { return _phases.size(); }
    const Phase& phaseSuivante(const Phase& phase) const { return _phases[(phase.numero+1) % numPhases()]; }

    const Interphase& interphase(size_t i, size_t j) const { return _interphases.element(i,j); }
    const Interphase& interphase(const Phase& p1, const Phase& p2) const { return interphase(p1.numero, p2.numero); }

    const Vecteur<DemandePriorite>& demandesPriorite() const { return _demandes; }

    const Phase& phaseActuelle() const { return *_phaseActuelle; }
    int tempsEcoule() const { return _tempsEcoule; }

private:
    Vecteur<LigneDeFeu> _lignes;
    Vecteur<Phase> _phases;
    Matrice<Interphase> _interphases;
    Vecteur<DemandePriorite> _demandes;
    const Phase *_phaseActuelle;
    int _tempsEcoule;
};

#endif // DONNEES_CARREFOUR_H


