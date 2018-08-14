#ifndef GRAPHE_H
#define GRAPHE_H

#include "Matrice.h"
#include "Vecteur.h"

template<class T>
class Graphe{
public:
    Graphe() {}
    Graphe(Vecteur<T> sommets, Matrice<bool> transitions) : _sommets(sommets), _transitions(transitions) {}

    const Vecteur<T>& sommets() const { return _sommets; }
    Vecteur<T> enfants(const T&) const;

private:
    Vecteur<T> _sommets;
    Matrice<bool> _transitions;
};

template<class T>
Vecteur<T> Graphe<T>::enfants(const T& sommet) const{
    Vecteur<T> enfants;

    typename Vecteur<T>::size_type i = sommets().index(sommet);
    for (typename Vecteur<T>::size_type j=0; j!=sommets().size(); ++j){
        if (_transitions.element(i,j) == true)
            enfants.push_back(sommets()[j]);
    }
    return enfants;
}

#endif // GRAPHE_H
