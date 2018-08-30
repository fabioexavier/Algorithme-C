#ifndef MATRICE_H
#define MATRICE_H

#include <vector>

template<class T>
class Matrice{
public:
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;

    Matrice() : _rows(0), _cols(0) {}
    Matrice(size_t rows, size_t cols) : _rows(rows), _cols(cols), _elements(rows*cols) {}
    Matrice(size_t rows, size_t cols, const T& value) : _rows(rows), _cols(cols),
                                                              _elements(rows*cols, value) {}

    iterator begin() { return _elements.begin(); }
    iterator end() { return _elements.end(); }
    const_iterator begin() const { return _elements.begin(); }
    const_iterator end() const { return _elements.end(); }

    size_t rows() const { return _rows; }
    size_t cols() const { return _cols; }

    T& element(size_t i, size_t j);
    const T& element(size_t i, size_t j) const;

private:
    size_t _rows, _cols;
    std::vector<T> _elements;
};

template<class T>
std::ostream& operator<<(std::ostream& os, const Matrice<T>& mat){
    size_t n = 0;
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
T& Matrice<T>::element(size_t i, size_t j){
    if (i < _rows && j < _cols)
        return _elements[i*_cols + j];
    else
        throw std::domain_error("Indice de la matrice hors limite");
}
template<class T>
const T& Matrice<T>::element(size_t i, size_t j) const{
    if (i < _rows && j < _cols)
        return _elements[i*_cols + j];
    else
        throw std::domain_error("Indice de la matrice hors limite");
}

#endif // MATRICE_H
