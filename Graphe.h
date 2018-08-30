#ifndef GRAPHE_H
#define GRAPHE_H

#include <iostream>
#include <vector>

#include "Matrice.h"

template<class T>
class Graphe{
public:
    Graphe() {}
    Graphe(std::vector<T> sommets, Matrice<int> transitions) : _sommets(sommets), _transitions(transitions) {}

    const std::vector<T>& sommets() const { return _sommets; }
    std::vector<T> enfants(const T&) const;

private:
    std::vector<T> _sommets;
    Matrice<int> _transitions;
};

template<class T>
std::vector<T> Graphe<T>::enfants(const T& sommet) const{
    std::vector<T> enfants;

//    typename std::vector<T>::const_iterator iter = find(_sommets.begin(), _sommets.end(), sommet);
//    size_t i = iter - _sommets.begin();

    size_t i = index(sommet, _sommets);

    for (size_t j = (i+1)%(_sommets.size() ); j != i; j = (j+1)%(_sommets.size() ) ) {
        if (_transitions.element(i,j) == 1)
            enfants.push_back(_sommets[j]);
    }
    return enfants;
}

#endif // GRAPHE_H
