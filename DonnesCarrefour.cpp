#include <iostream>

#include "DonneesCarrefour.h"

using std::ostream;

// Carrefour

// Phase
ostream& operator<<(ostream& os, const Phase& p){
    os << "Phase " << p.numero;
    return os;
}

bool operator==(const Phase& p1, const Phase& p2){
    return p1.numero == p2.numero;
}

// Interphase
ostream& operator<<(ostream& os, const Interphase& ip){
    os << "Interphase (" << ip.origine << ", " << ip.destination << ")";
    return os;
}

// LigneDeFeu

// Demande Priorite
ostream& operator<<(ostream& os, const DemandePriorite& d){
    os << "Delai " << d.delaiApproche << ", Code " << d.code;
    return os;
}
