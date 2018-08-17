#ifndef VECTEUR_H
#define VECTEUR_H

#include <iostream>
#include <stdexcept>

template<class T>
class Vecteur{
public:
    const static size_t MAX_SIZE = 100;

    typedef T* iterator;
    typedef const T* const_iterator;

    Vecteur() : fin(data), limite(data+MAX_SIZE) {}
    explicit Vecteur(size_t n) : fin(data+n), limite(data+MAX_SIZE) {}
    Vecteur(size_t, const T&);
    Vecteur(const Vecteur&);
    Vecteur(const T[], size_t);
    Vecteur& operator=(const Vecteur&);

    T& operator[](int);
    const T& operator[](int) const;

    iterator begin() { return data; }
    iterator end() { return fin; }
    const_iterator begin() const { return data; }
    const_iterator end() const { return fin; }
    iterator cyclicNext(iterator iter){
        if (++iter == fin)
            return data;
        return iter;
    }
    const_iterator cyclicNext(const_iterator iter) const{
        if (++iter == fin)
            return data;
        return iter;
    }

    size_t size() const { return fin - data; }
    void setSize(size_t);
    void clear() { fin = data; }
    void push_back(const T&);
    size_t index(const T&) const;
    T somme() const;


private:
    T data[MAX_SIZE];
    T* fin;
    T* const limite;
};

// Constructeurs
template<class T>
Vecteur<T>::Vecteur(size_t len, const T& value) : fin(data), limite(data+MAX_SIZE){
    if (len > MAX_SIZE)
        throw std::domain_error("Longueur depasse taille max du vecteur");
    else{
        fin = data + len;
        for (size_t i=0; i<len; ++i)
            data[i] = value;
    }
}

template<class T>
Vecteur<T>::Vecteur(const Vecteur& v) : fin(data+v.size()), limite(data+MAX_SIZE) {
    for (size_t i=0; i<v.size(); ++i)
        data[i] = v.data[i];
}

template<class T>
Vecteur<T>::Vecteur(const T arr[], size_t len) : limite(data+MAX_SIZE){
    if (len > MAX_SIZE)
        throw std::domain_error("Longueur depasse taille max du vecteur");
    else{
        fin = data + len;
        for (size_t i=0; i<len; ++i)
            data[i] = arr[i];
    }
}

// Opérateurs
template<class T>
Vecteur<T>& Vecteur<T>::operator=(const Vecteur& v){
    fin = data + v.size();
    for (size_t i=0; i<v.size(); ++i)
        data[i] = v.data[i];

    return *this;
}

template<class T>
T& Vecteur<T>::operator[](int n){
    if (n < -int(size()) || n > int(size())-1 )
        throw std::domain_error("Indice du vecteur hors limite");
    else if (n >= 0)
        return *(data+n);
    else
        return *(fin+n);
}

template<class T>
const T& Vecteur<T>::operator[](int n) const{
    if (n < -int(size()) || n > int(size()-1) )
        throw std::domain_error("Indice du vecteur hors limite");
    else if (n >= 0)
        return *(data+n);
    else
        return *(fin+n);
}

template<class T>
std::ostream& operator<<(std::ostream& os, const Vecteur<T>& vec){
    for (typename Vecteur<T>::const_iterator iter=vec.begin(); iter!=vec.end(); ++iter)
        os << *iter << " ";
    return os;
}

// Fonctions
template<class T>
void Vecteur<T>::setSize(size_t n){
    if (n > MAX_SIZE)
        throw std::domain_error("Longueur plus grande que le max du vecteur");
    else
        fin = data + n;
}

template<class T>
void Vecteur<T>::push_back(const T& element){
    if (fin == limite)
        throw std::runtime_error("Vecteur deja plein");
    else
        *(fin++) = element;
}

template<class T>
size_t Vecteur<T>::index(const T& element) const{
    for (size_t i=0; i!=size(); ++i){
        if (data[i] == element)
            return i;
    }
    throw std::runtime_error("Element pas present dans le vecteur");
}

template <class T>
T Vecteur<T>::somme() const{
    T somme = 0;
    for (Vecteur<T>::const_iterator iter=begin(); iter!=end(); ++iter)
        somme += *iter;

    return somme;
}

#endif // VECTEUR_H

