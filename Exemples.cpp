#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "AuxVector.h"
#include "DonneesCarrefour.h"
#include "Matrice.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::istream;
using std::istringstream;
using std::runtime_error;
using std::string;
using std::vector;

bool char2bool(char c){
        switch(c){
        case 'T':
            return true;
        case 'F':
            return false;
        default:
            throw runtime_error("Charactere invalide dans la convertion char2bool");
        }
    }

istream& operator>>(istream& is, Phase& phase){
    is >> phase.numero;

    string s;
    is >> s;
    for (string::const_iterator iter = s.begin(); iter != s.end(); ++iter)
        phase.lignesOuvertes.push_back(char2bool(*iter) );

    is >> phase.dureeMinimale >> phase.dureeNominale >> phase.dureeMaximale;

    char c;

    is >> c;
    phase.escamotable = char2bool(c);

    is >> phase.code;

    is >> c;
    phase.exclusive = char2bool(c);

    is >> c;
    phase.solicitee = char2bool(c);

    is >> phase.marge >> phase.intervalle;

    return is;
}

void Carrefour::loadFile(const char* fileName){
    ifstream file(fileName);
    if (file.is_open() ){
        string line;
        istringstream iss;

        while (getline(file, line) ){
            if (line == "<lignes>"){
                _lignes.clear();
                getline(file, line);
                iss.clear();
                iss.str(line);

                int n;
                while(iss >> n)
                    _lignes.push_back(n);

                getline(file, line); // </lignes>
            }

            if (line == "<phases>"){
                _phases.clear();
                getline(file, line);
                while (line != "</phases>"){
                    iss.clear();
                    iss.str(line);

                    Phase phase;
                    iss >> phase;
                    _phases.push_back(phase);
                    getline(file, line);
                }
            }

            if (line == "<interphases>"){
                _interphases = Matrice<Interphase>(numPhases(), numPhases());
                for (size_t i = 0; i != numPhases(); ++i){
                    getline(file, line);
                    iss.clear();
                    iss.str(line);

                    for (size_t j = 0; j != numPhases(); ++j){
                        Interphase interphase;
                        interphase.origine = i;
                        interphase.destination = j;
                        iss >> interphase.duree;
                        _interphases.element(i,j) = interphase;
                    }
                }
                getline(file, line); // </interphases>
            }

            if (line == "<demandes>"){
                _demandes.clear();
                getline(file, line);
                while (line != "</demandes>"){
                    iss.clear();
                    iss.str(line);

                    DemandePriorite demande;
                    iss >> demande.delaiApproche >> demande.code;
                    _demandes.push_back(demande);
                    getline(file, line);
                }
            }

            if (line == "<actuelle>"){
                getline(file, line);
                iss.clear();
                iss.str(line);

                int numPhaseActuelle;
                iss >> numPhaseActuelle >> _tempsEcoule;
                _phaseActuelle = &_phases[numPhaseActuelle];
            }

        }
    }
    else
        throw runtime_error("Le fichier n'existe pas.");
}












void Carrefour::loadExemple(int n){
    switch (n){
    case 1:
        // Lignes
        const int arrLignes[] = { -1,
                                   5,
                                   5,
                                  -1,
                                  -1,
                                   5,
                                   5,
                                  -1,
                                  -1,
                                   5 };

        _lignes = vector<int>(arrLignes, arrLignes+sizeof(arrLignes)/sizeof(arrLignes[0]));

        // Phases
        // Numero, lignes, min, nom, max, escamotable, code, exclusive, solicitee, marge, intervalle
        const Phase arrPhases[] = { Phase(0, "FFFTTFFTTF", 14, 41, 60, false, 2, false, true, 4, -1),
                                    Phase(1, "TTFFFFFFFT", 4, 4, 30, true, 1, true, false, 4, 15),
                                    Phase(2, "FTTFFTTFFT", 11, 15, 40, false, 0, false, true, 4, -1),
                                    Phase(3, "TFFTFFFTFF", 4, 4, 30, true, 1, true, false, 4, 15) };
        const size_t numPhases = sizeof(arrPhases)/sizeof(arrPhases[0]);
        _phases =  vector<Phase>(arrPhases, arrPhases+numPhases);

        // Interphases
        const unsigned int dureeInterphases[numPhases][numPhases] = { {0, 7, 7, 5},
                                                                      {8, 0, 7, 0},
                                                                      {8, 8, 0, 8},
                                                                      {5, 0, 7, 0} };
        _interphases = Matrice<Interphase>(numPhases, numPhases);
        for (size_t i=0; i!=numPhases; ++i)
            for (size_t j=0; j!=numPhases; ++j)
                _interphases.element(i,j) = Interphase(i, j, dureeInterphases[i][j]);

        // Demandes Priorite
        const DemandePriorite arrDemandes[] = { DemandePriorite(130,1) };
        _demandes =  vector<DemandePriorite>(arrDemandes, arrDemandes+sizeof(arrDemandes)/sizeof(arrDemandes[0]) );

        // Phase Actuelle
        _phaseActuelle = &_phases[0];

        // Temps Ecoulé
        _tempsEcoule = 5;

        break;
    }
}


