#ifndef MATRICE_H
#define MATRICE_H

#include "Vecteur.h"

template<class T>
class Matrice{
public:
    typedef typename Vecteur<T>::iterator iterator;
    typedef typename Vecteur<T>::const_iterator const_iterator;
    typedef typename Vecteur<T>::size_type size_type;

    Matrice() : nRows(0), nCols(0) {}
    Matrice(size_type rows, size_type cols) : nRows(rows), nCols(cols) { initMatrice(); }
    Matrice(size_type rows, size_type cols, const T& value) : nRows(rows), nCols(cols),
                                                              vecElements(rows*cols, value) { initMatrice(); }

    iterator begin() { return vecElements.begin(); }
    iterator end() { return vecElements.end(); }
    const_iterator begin() const { return vecElements.begin(); }
    const_iterator end() const { return vecElements.end(); }

    size_type rows() const { return nRows; }
    size_type cols() const { return nCols; }

    T& element(size_type i, size_type j);
    const T& element(size_type i, size_type j) const;

private:
    size_type nRows, nCols;
    Vecteur<T> vecElements;

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
    if (i < nRows && j < nCols)
        return vecElements[i*nCols + j];
    else
        throw std::domain_error("Indice de la matrice hors limite");
}
template<class T>
const T& Matrice<T>::element(size_type i, size_type j) const{
    if (i < nRows && j < nCols)
        return vecElements[i*nCols + j];
    else
        throw std::domain_error("Indice de la matrice hors limite");
}

template<class T>
void Matrice<T>::initMatrice(){
    size_type numElements = nRows*nCols;
    if (numElements > Vecteur<T>::MAX_SIZE)
        throw std::domain_error("Nombre d'elements depasse taille max de la matrice");
    else{
        vecElements.setSize(numElements);
    }
}

#endif // MATRICE_H
