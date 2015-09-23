#include "Jacobi.h"

template <class T>
algorithms::Jacobi<T>::Jacobi() {

}

template <class T>
void algorithms::Jacobi<T>::solve(QVector<T> & result, CRS<T> const & matrix, QVector<T> const & rhs) {
    T rel=1, sum;
    this->itCount = 0;

    QVector<T> old = result;
    while(rel-this->eps>0 && this->itCount < this->maxIt) {
        for(int i=0; i<result.size(); ++i) {
            assert(matrix.getValue(i,i)!=0);
            sum=matrix.multRowQVector(i,old);
            result[i] += 1./matrix.getValue(i,i)*(rhs[i]-sum);
        }
        old = result;
//        rel = norm2(addQVectors(matrix*result,(-1.) * rhs))/norm2(rhs);
        rel = normInf(addQVectors(matrix*result,(-1.) * rhs))/normInf(rhs);
        ++(this->itCount);
    }
}
