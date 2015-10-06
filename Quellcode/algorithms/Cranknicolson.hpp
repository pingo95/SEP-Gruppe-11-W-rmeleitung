#ifndef CRANKNICOLSON_HPP
#define CRANKNICOLSON_HPP


#include "Cranknicolson.h"

template <class T>
algorithms::CrankNicolson<T>::CrankNicolson() {

}

template <class T>
void algorithms::CrankNicolson<T>::calcNextStep(QVector<T> const & last, QVector<T> & next, QVector< QVector<T>* > const & heatSources) const {
    T tmp = this->deltaT/((T)2);
    QVector<T> rhs = algorithms::addQVectors(this->rhsMatrix*last,tmp*(algorithms::addQVectors(*(heatSources[1]),*(heatSources[0]))));
    this->activeSolver->solve(next,this->itMatrix,rhs);
}

template <class T>
void algorithms::CrankNicolson<T>::getNeedetHeatSources(QVector<T> & neededTimeSteps, bool & reusable) const {
    reusable = true;
    neededTimeSteps.resize(2);
    neededTimeSteps[0]=1;
    neededTimeSteps[1]=0;
}

template <class T>
void algorithms::CrankNicolson<T>::setUpSpecific(QVector<T> const & thermalDiffusivities) {
    algorithms::CRS<T> A1, diag;
    A1.A1(this->n);
    diag.diag(thermalDiffusivities);
    diag = diag.multCRSCRS(A1);
    A1.eye(this->n);
    T tmp = this->deltaT/((T)2*this->deltaX*this->deltaX);
    diag = tmp * diag;
    this->itMatrix = A1 - diag;
    this->rhsMatrix = A1 + diag;
}

#endif // CRANKNICOLSON_HPP
