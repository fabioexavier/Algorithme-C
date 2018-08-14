#ifndef MATRICE_H
#define MATRICE_H

#include "Vecteur.h"

template<class T>
class Matrice{
public:
    typedef typename Vecteur<T>::iterator iterator;
    typedef typename Vecteur<T>::const_iterator const_iterator;
    typedef typename Vecteur<T>::size_type size_type;

    Matrice() : _rows(0), _cols(0) {}
    Matrice(size_type rows, size_type cols) : _rows(rows), _cols(cols) { initMatrice(); }
    Matrice(size_type rows, size_type cols, const T& value) : _rows(rows), _cols(cols),
                                                              _elements(rows*cols, value) { initMatrice(); }

    iterator begin() { return _elements.begin(); }
    iterator end() { return _elements.end(); }
    const_iterator begin() const { return _elements.begin(); }
    const_iterator end() const { return _elements.end(); }

    size_type rows() const { return _rows; }
    size_type cols() const { return _cols; }

    T& element(size_type i, size_type j);
    const T& element(size_type i, size_type j) const;

private:
    size_type _rows, _cols;
    Vecteur<T> _elements;

    void initMatrice();
};

template<class T>
std::ostream& operator<<(std::ostream& os, const Matrice<T>& mat){
    typename Matrice<T>::size_type n = 0;
    for (typename Matrice<T>::const_iterator iter=mat.begin(); iter!=mat.end(); ++iter){
        os << *iter << " ";
        ++n;
        if (n == mat.cols()){
            os << std::endl;
            n = 0;
        }
    }
    return os;
}

template<class T>
T& Matrice<T>::element(size_type i, size_type j){
    if (i < _rows && j < _cols)
        return _elements[i*_cols + j];
    else
        throw std::domain_error("Indice de la matrice hors limite");
}
template<class T>
const T& Matrice<T>::element(size_type i, size_type j) const{
    if (i < _rows && j < _cols)
        return _elements[i*_cols + j];
    else
        throw std::domain_error("Indice de la matrice hors limite");
}

template<class T>
void Matrice<T>::initMatrice(){
    size_type numElements = _rows*_cols;
    if (numElements > Vecteur<T>::MAX_SIZE)
        throw std::domain_error("Nombre d'elements depasse taille max de la matrice");
    else{
        _elements.setSize(numElements);
    }
}

#endif // MATRICE_H
