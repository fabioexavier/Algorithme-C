#include <iostream>
#include <stdexcept>

#include "AlgorithmePriorite.h"

using std::domain_error;
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

// Le chemin est valide si la dernière phase est prioritaire et s'il y a au moins une phase pour chaque code demandé
bool Chemin::valide(const Vecteur<DemandePriorite>& demandesPriorite) const {
    bool cheminValide = true;

    // Si la dernière phase n'est pas prioritaire, le chemin n'est pas valide
    if (phase(-1).code == 0)
        cheminValide = false;

    else{
        for (Vecteur<DemandePriorite>::const_iterator demande=demandesPriorite.begin();
                                                      demande!=demandesPriorite.end(); ++demande){
            // Si au moins un code demande n'est pas présent dans le chemin, le chemin n'est pas valide
            if (comptageCode(demande->code) == 0){
                cheminValide = false;
                break;
            }
        }
    }
    return cheminValide;
}
