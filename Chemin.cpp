#include <algorithm>
#include <iostream>
#include <stdexcept>

#include "AlgorithmePriorite.h"

using std::domain_error;
using std::find;
using std::ostream;

Chemin::Chemin(const Carrefour& c) : pCarrefour(&c){
    Phase origine = c.phaseActuelle();
    vecPhases.push_back(origine);
    _sommeMin = origine.dureeMinimale - c.tempsEcoule() > 0 ? origine.dureeMinimale - c.tempsEcoule() : 0;
    incrementeCode(origine.code);
}

unsigned int Chemin::comptageCode(unsigned int code) const {
    if (code >= vecComptageCodes.MAX_SIZE)
        throw domain_error("Code hors limite");
    if (code >= vecComptageCodes.size())
        return 0;
    return vecComptageCodes[code];
}

unsigned int Chemin::incrementeCode(unsigned int code){
    if (code >= vecComptageCodes.MAX_SIZE)
        throw domain_error("Code hors limite");

    if (code >= vecComptageCodes.size() ){
        Vecteur<unsigned int>::size_type oldSize = vecComptageCodes.size();
        vecComptageCodes.setSize(code+1);
        for (size_t i=oldSize; i<code+1; i++)
            vecComptageCodes[i] = 0;
    }
    return ++vecComptageCodes[code];
}

ostream& operator<<(ostream& os, const Chemin& c){
    for (Chemin::const_iterator iter=c.begin(); iter!=c.end(); ++iter){
        os << *iter << " ";
    }
    return os;
}

void Chemin::push_back(const Phase& phase){
    vecPhases.push_back(phase);
    _sommeMin += pCarrefour->interphase(vecPhases[-2], vecPhases[-1]).duree;
    _sommeMin += phase.dureeMinimale;
    incrementeCode(phase.code);
}

// Le chemin est valide si la dernière phase est prioritaire et s'il y a au moins une phase pour chaque code demandé
bool Chemin::valide(const Vecteur<DemandePriorite>& demandesPriorite) const {
    bool cheminValide = true;

    // Si la dernière phase n'est pas prioritaire
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

bool Chemin::transitionPossible(const Phase& phase, const Vecteur<DemandePriorite>& demandesPriorite) const {
    bool ret = true;
    if (phase.escamotable){
        // Phase ESC
        if (phase.code == 0){
            // Si le phase est deja dans le chemin
            if (find(vecPhases.begin(), vecPhases.end(), phase) != vecPhases.end())
                ret = false;
        }
        // Phase PEE ou PENE - Il manque ajouter traitement phases PENE!!!
        else{
            unsigned int code = phase.code;
            // Nombre max admissible de phases avec ce code dans le chemin
            // Admet une phase en plus si la première phase possède le même code
            unsigned int maxPhases = this->phase(0).code == code ? 1 : 0;
            for (Vecteur<DemandePriorite>::const_iterator demande=demandesPriorite.begin();
                                                          demande!=demandesPriorite.end(); ++demande){
                if (demande->code == code)
                    ++maxPhases;
            }
            if (comptageCode(code) == maxPhases)
                ret = false;
        }
    }
    return ret;
}
