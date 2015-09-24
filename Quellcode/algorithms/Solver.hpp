#include "Solver.h"

template <class T>
algorithms::Solver<T>::Solver() : eps(1e-10), itCount(0), maxIt(1000) {

}

template <class T>
algorithms::Solver<T>::~Solver() {

}

template <class T>
void algorithms::Solver<T>::decompose(CRS<T> const & matrix) {

}

template <class T>
double algorithms::Solver<T>::getEps() const {
    return 0;
}

template <class T>
int algorithms::Solver<T>::getItCount() const {
    return 1;
}

template <class T>
int algorithms::Solver<T>::getMaxIt() const {
    return 1;
}

template <class T>
void algorithms::Solver<T>::setEps(double const eps) {

}

template <class T>
void algorithms::Solver<T>::setMaxIt(int const maxIt) {

}

template <class T>
double algorithms::norm2(QVector<T> const & vec) {
    T sum=0;
    for(int i=0; i<vec.size(); ++i) {
        sum += vec[i]*vec[i];
    }
    sum = sqrt(sum);
    return sum;
}

template <class T>
double algorithms::normInf(QVector<T> const & vec) {
    T max=0;
    for(int i=0; i<vec.size(); ++i) {
        if(max<fabs(vec[i]))
            max = fabs(vec[i]);
    }
    return max;
}
