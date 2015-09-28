#ifndef IMPEULER_HPP
#define IMPEULER_HPP

#include "Impeuler.h"

template <class T>
algorithms::ImpEuler<T>::ImpEuler() {

}

template <class T>
void algorithms::ImpEuler<T>::calcNextStep(QVector<T> const & last, QVector<T> & next, QVector< QVector<T>* > const & heatSources) const {
    QVector<T> rhs = addQVectors(last,this->deltaT*(*(heatSources[0])));
    this->activeSolver->solve(next,this->itMatrix,rhs);
}

template <class T>
void algorithms::ImpEuler<T>::getNeedetHeatSources(QVector<T> & neededTimeSteps, bool & reusable) const {
    reusable = true;
    neededTimeSteps.resize(1);
    neededTimeSteps[0]=1;
}

template <class T>
void algorithms::ImpEuler<T>::setUpSpecific(QVector<T> const & thermalDiffusivities) {
    CRS<T> A1, diag;
    A1.A1(this->n);
    diag.diag(thermalDiffusivities);
    diag = diag.multCRSCRS(A1);
    A1.eye(this->n);
    this->itMatrix = this->deltaT/(this->deltaX*this->deltaX) * diag;
    this->itMatrix = A1 - this->itMatrix;
}

#endif // IMPEULER_HPP
