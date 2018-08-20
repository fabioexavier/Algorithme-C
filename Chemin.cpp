#include <algorithm>
#include <iostream>
#include <stdexcept>

#include "AlgorithmePriorite.h"

using std::domain_error;
using std::find;
using std::ostream;

Chemin::Chemin(const Carrefour& c) : _pCarrefour(&c){
    Phase origine = c.phaseActuelle();
    _phases.push_back(origine);
    _sommeMin = origine.dureeMinimale - c.tempsEcoule() > 0 ? origine.dureeMinimale - c.tempsEcoule() : 0;
    incrementeCode(origine.code);
}

int Chemin::comptageCode(int c) const {
    size_t code = c;

    if (code >= _comptageCodes.MAX_SIZE)
        throw domain_error("Code hors limite");

    if (code >= _comptageCodes.size())
        return 0;

    return _comptageCodes[code];
}

int Chemin::incrementeCode(int c){
    size_t code = c;

    if (code >= _comptageCodes.MAX_SIZE)
        throw domain_error("Code hors limite");

    if (code >= _comptageCodes.size() ){
        size_t oldSize = _comptageCodes.size();
        _comptageCodes.setSize(code+1);
        for (size_t i=oldSize; i<code+1; i++)
            _comptageCodes[i] = 0;
    }
    return ++_comptageCodes[code];
}

ostream& operator<<(ostream& os, const Chemin& c){
    for (Chemin::const_iterator iter=c.begin(); iter!=c.end(); ++iter){
        os << *iter << " ";
    }
    return os;
}

void Chemin::push_back(const Phase& phase){
    _phases.push_back(phase);
    _sommeMin += _pCarrefour->interphase(_phases[-2], _phases[-1]).duree;
    _sommeMin += phase.dureeMinimale;
    incrementeCode(phase.code);
}

// Le chemin est valide si la dernière phase est prioritaire et a été demandée, et s'il y a au moins une phase pour chaque code demandé
bool Chemin::valide() const {
    bool boolDerniere = false;
    bool boolComptage = true;
    Vecteur<DemandePriorite> demandesPriorite = this->carrefour().demandesPriorite();

    for (Vecteur<DemandePriorite>::const_iterator demande=demandesPriorite.begin();
                                                  demande!=demandesPriorite.end(); ++demande){
        // Si la dernière phase a été solicitée, le chemin est valide
        if (phase(-1).code == demande->code)
            boolDerniere = true;
        // Si au moins un code demande n'est pas présent dans le chemin, le chemin n'est pas valide
        if (comptageCode(demande->code) == 0){
            boolComptage = false;
            break;
        }
    }

    return boolDerniere && boolComptage;
}

bool Chemin::transitionPossible(const Phase& phase) const{
    Carrefour carrefour = this->carrefour();
    Vecteur<DemandePriorite> demandesPriorite = carrefour.demandesPriorite();

    bool transitionPossible = true;
    if (phase.escamotable){
        // Phase ESC
        if (phase.code == 0){
            // Si la phase est deja dans le chemin
            if (find(this->begin(), this->end(), phase) != this->end())
                transitionPossible = false;
        }
        // Phase PEE ou PENE
        else{
            int code = phase.code;

            // Nombre max admissible de phases avec ce code dans le chemin
            // Admet une phase en plus si la première phase est PEE et a le même code
            int maxPhases = this->phase(0).exclusive && (this->phase(0).code == code) ? 1 : 0;
            // +1 pour chaque demande avec le même code
            for (Vecteur<DemandePriorite>::const_iterator demande=demandesPriorite.begin();
                                                          demande!=demandesPriorite.end(); ++demande){
                if (demande->code == code)
                    ++maxPhases;
            }
            // +1 pour chaque phase PENE distincte, solicitée, avec le même code et dans le chemin
            for (size_t i=0; i!=carrefour.numPhases(); ++i){
                if (!carrefour.phase(i).exclusive &&
                    carrefour.phase(i).solicitee &&
                    carrefour.phase(i).code == code &&
                    find(this->begin(), this->end(), carrefour.phase(i) ) != this->end() )
                    ++maxPhases;
            }

            // Empêche la transition si le nombre max de phases est déjà atteint
            if (this->comptageCode(code) == maxPhases)
                transitionPossible = false;
        }
    }
    return transitionPossible;
}
