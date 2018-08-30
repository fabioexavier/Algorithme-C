#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "AlgorithmePriorite.h"

using std::domain_error;
using std::find;
using std::ostream;
using std::vector;

Chemin::Chemin(const Carrefour& c) : _pCarrefour(&c){
    Phase origine = c.phaseActuelle();
    _phases.push_back(origine);
    _sommeMin = origine.dureeMinimale - c.tempsEcoule() > 0 ? origine.dureeMinimale - c.tempsEcoule() : 0;
    incrementeCode(origine.code);
}

const Phase& Chemin::phase(int n) const{
    if (n >= 0 && (unsigned) n < _phases.size() )
        return _phases[n];
    else if (n < 0 && n >= (int) (-_phases.size()) )
        return _phases[_phases.size()+n];
    else
        throw domain_error("Indice de la phase hors limite");
}

int Chemin::comptageCode(int c) const {
    size_t code = c;

    if (code >= _comptageCodes.size())
        return 0;

    return _comptageCodes[code];
}

int Chemin::incrementeCode(int c){
    size_t code = c;

    if (code >= _comptageCodes.size() ){
        _comptageCodes.resize(code+1, 0);
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
    _sommeMin += _pCarrefour->interphase(this->phase(-2), this->phase(-1) ).duree;
    _sommeMin += phase.dureeMinimale;
    incrementeCode(phase.code);
}

// Le chemin est valide si la derni�re phase est prioritaire et a �t� demand�e, et s'il y a au moins une phase pour chaque code demand�
bool Chemin::valide() const {
    bool boolDerniere = false;
    bool boolComptage = true;
    vector<DemandePriorite> demandesPriorite = this->carrefour().demandesPriorite();

    for (vector<DemandePriorite>::const_iterator demande=demandesPriorite.begin();
                                                  demande!=demandesPriorite.end(); ++demande){
        // Si la derni�re phase a �t� solicit�e, le chemin est valide
        if (phase(-1).code == demande->code)
            boolDerniere = true;
        // Si au moins un code demande n'est pas pr�sent dans le chemin, le chemin n'est pas valide
        if (comptageCode(demande->code) == 0){
            boolComptage = false;
            break;
        }
    }

    return boolDerniere && boolComptage;
}

bool Chemin::transitionPossible(const Phase& phase) const{
    Carrefour carrefour = this->carrefour();
    vector<DemandePriorite> demandesPriorite = carrefour.demandesPriorite();

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
            // Admet une phase en plus si la premi�re phase est PEE et a le m�me code
            int maxPhases = this->phase(0).exclusive && (this->phase(0).code == code) ? 1 : 0;
            // +1 pour chaque demande avec le m�me code
            for (vector<DemandePriorite>::const_iterator demande=demandesPriorite.begin();
                                                          demande!=demandesPriorite.end(); ++demande){
                if (demande->code == code)
                    ++maxPhases;
            }
            // +1 pour chaque phase PENE distincte, solicit�e, avec le m�me code et dans le chemin
            for (size_t i=0; i!=carrefour.numPhases(); ++i){
                if (!carrefour.phase(i).exclusive &&
                    carrefour.phase(i).solicitee &&
                    carrefour.phase(i).code == code &&
                    find(this->begin(), this->end(), carrefour.phase(i) ) != this->end() )
                    ++maxPhases;
            }

            // Emp�che la transition si le nombre max de phases est d�j� atteint
            if (this->comptageCode(code) == maxPhases)
                transitionPossible = false;
        }
    }
    return transitionPossible;
}
