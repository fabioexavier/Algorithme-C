#ifndef GRAPHE_H
#define GRAPHE_H

#include "Matrice.h"
#include "Vecteur.h"

template<class T>
class Graphe{
public:
    Graphe() {}
    Graphe(Vecteur<T> sommets, Matrice<bool> transitions) : vecSommets(sommets), matTransitions(transitions) {}

    const Vecteur<T>& sommets() const { return vecSommets; }
    Vecteur<T> enfants(const T&) const;

private:
    Vecteur<T> vecSommets;
    Matrice<bool> matTransitions;
};

template<class T>
Vecteur<T> Graphe<T>::enfants(const T& sommet) const{
    Vecteur<T> vecEnfants;

    typename Vecteur<T>::size_type i = vecSommets.index(sommet);
    for (typename Vecteur<T>::size_type j=0; j!=vecSommets.size(); ++j){
        if (matTransitions.element(i,j) == true)
            vecEnfants.push_back(vecSommets[j]);
    }
    return vecEnfants;
}

#endif // GRAPHE_H
