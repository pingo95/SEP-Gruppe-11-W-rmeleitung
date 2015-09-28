#ifndef GAUSSSEIDEL_HPP
#define GAUSSSEIDEL_HPP

#include "Gaussseidel.h"

template <class T>
algorithms::GaussSeidel<T>::GaussSeidel() {

}

template <class T>
void algorithms::GaussSeidel<T>::solve(QVector<T> & result, CRS<T> const & matrix, QVector<T> const & rhs) {
    T rel=1, sum;
    this->itCount = 0;

    while(rel-this->eps>0 && this->itCount < this->maxIt) {
        for(int i=0; i<result.size(); ++i) {
            assert(matrix.getValue(i,i)!=0);
            sum=matrix.multRowQVector(i,result);
            result[i] += 1./matrix.getValue(i,i)*(rhs[i]-sum);
        }
        rel = norm2(addQVectors(matrix*result,(-1.)*rhs))/norm2(rhs);
        ++this->itCount;
    }
}

#endif // GAUSSSEIDEL_HPP
