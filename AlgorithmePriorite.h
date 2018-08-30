#ifndef ALGORITHME_PRIORITE_H
#define ALGORITHME_PRIORITE_H

#include <iostream>
#include <numeric>
#include <vector>

#include "DonneesCarrefour.h"
#include "AuxVector.h"

// Classes
class Chemin{
public:
    typedef std::vector<Phase>::const_iterator const_iterator;

    Chemin() {}
    explicit Chemin(const Carrefour&);

    const Phase& phase(int) const;
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
    std::vector<Phase> _phases;
    int _sommeMin;
    std::vector<int> _comptageCodes;
    const Carrefour* _pCarrefour;
};
std::ostream& operator<<(std::ostream&, const Chemin&);

struct ResultatLP{
public:
    bool optimumTrouve;
    std::vector<double> durees;
    std::vector<double> deviations;
    std::vector<double> retards;
    double score;

    ResultatLP() : optimumTrouve(false), score(-1) {}
    void calcScore(){
        score = std::accumulate(deviations.begin(), deviations.end(), 0.0) +
                100*std::accumulate(retards.begin(), retards.end(), 0.0);
    }

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
void rechercheChemins(const Carrefour&, std::vector<Chemin>&);
ResultatLP analyseLP(const Chemin&);

// Fonctions Auxiliaires
Graphe<Phase> calcGraphe(const Carrefour&);
void rechercheRecursive(const Graphe<Phase>&, const Chemin&, std::vector<Chemin>&);
bool finDeBranche(const Graphe<Phase>&, const Chemin&);

#endif // ALGORITHME_PRIORITE_H


