#ifndef AUX_VECTOR_H
#define AUX_VECTOR_H

#include <algorithm>
#include <vector>

template<class T>
size_t index(const T& element, const std::vector<T>& vec){
    typename std::vector<T>::const_iterator iter = std::find(vec.begin(), vec.end(), element);
    return iter - vec.begin();
}

template<class T>
typename std::vector<T>::iterator cyclicNext(typename std::vector<T>::iterator iter, const std::vector<T>& v){
    if (++iter == v.end() )
        return v.begin();
    return iter;
}

template<class T>
typename std::vector<T>::const_iterator cyclicNext(typename std::vector<T>::const_iterator iter, const std::vector<T>& v){
    if (++iter == v.end() )
        return v.begin();
    return iter;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec){
    for (typename std::vector<T>::const_iterator iter=vec.begin(); iter!=vec.end(); ++iter)
        os << *iter << " ";
    return os;
}

#endif // AUX_VECTOR_H
