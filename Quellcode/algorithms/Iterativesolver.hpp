#ifndef ITERATIVESOLVER_HPP
#define ITERATIVESOLVER_HPP

#include "Iterativesolver.h"

template <class T>
algorithms::IterativeSolver<T>::IterativeSolver() {

}

template <class T>
algorithms::IterativeSolver<T>::~IterativeSolver(){

}

template <class T>
double algorithms::IterativeSolver<T>::getEps() const {
    return this->eps;
}

template <class T>
int algorithms::IterativeSolver<T>::getItCount() const {
    return this->itCount;
}

template <class T>
int algorithms::IterativeSolver<T>::getMaxIt() const {
    return this->maxIt;
}

template <class T>
void algorithms::IterativeSolver<T>::setEps(double const eps) {
    this->eps = eps;
}

template <class T>
void algorithms::IterativeSolver<T>::setMaxIt(const int maxIt) {
    this->maxIt = maxIt;
}

#endif // ITERATIVESOLVER_HPP
