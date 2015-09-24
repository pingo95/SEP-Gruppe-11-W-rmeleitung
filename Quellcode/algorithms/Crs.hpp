#include "Crs.h"

template <class T>
algorithms::CRS<T>::CRS() {

}

template <class T>
algorithms::CRS<T>::CRS(const CRS<T> & rhs) {
    size = rhs.size;
    index = rhs.index;
    ptr = rhs.ptr;
    value = rhs.value;
}

template <class T>
algorithms::CRS<T>& algorithms::CRS<T>::operator=(CRS<T> const & rhs) {
    size = rhs.size;
    ptr = rhs.ptr;
    index = rhs.index;
    value = rhs.value;
    return *this;
}

template <class T>
QVector<T> algorithms::CRS<T>::operator*(QVector<T> const & vec) const {
    QVector<T> res(size);
    for(int i=0; i<size; ++i)
        for(int j=ptr[i]; j<ptr[i+1]; ++j)
            res[i] += value[j] * vec[index[j]];
    return res;
}

template <class T>
algorithms::CRS<T> algorithms::CRS<T>::operator+(algorithms::CRS<T> const & rMat) const {
    assert(size==rMat.size);
    algorithms::CRS<T> diff;
    diff.size=size;
    diff.ptr.resize(size+1);
    int jl, jr;
    diff.ptr[0] = 0;
    for(int i=0; i<diff.size; ++i) {
        diff.ptr[i+1] = diff.ptr[i];
        jl = ptr[i];
        jr = rMat.ptr[i];
        while(jl < ptr[i+1] || jr < rMat.ptr[i+1]) {
            if(jr == rMat.ptr[i+1]) {
                    diff.index.append(index[jl]);
                    diff.value.append(value[jl]);
                    ++jl;
                }
            else {
                if(jl == ptr[i+1]) {
                    diff.index.append(rMat.index[jr]);
                    diff.value.append(rMat.value[jr]);
                    ++jr;
                }
                else {
                    if(index[jl] < rMat.index[jr]) {
                        diff.index.append(index[jl]);
                        diff.value.append(value[jl]);
                        ++jl;
                    }
                    else {
                        if(index[jl] > rMat.index[jr]) {
                            diff.index.append(rMat.index[jr]);
                            diff.value.append(rMat.value[jr]);
                            ++jr;
                        }
                        else {
                            diff.index.append(index[jl]);
                            diff.value.append(value[jl]+rMat.value[jr]);
                            ++jl; ++jr;
                        }
                    }
                }

            }
            ++diff.ptr[i+1];
        }
    }
    diff.ptr[size]=diff.value.size();
    return diff;
}

template <class T>
algorithms::CRS<T> algorithms::CRS<T>::operator-(algorithms::CRS<T> const & rMat) const {
    return (*this+((T)-1*rMat));
}

template <class T>
void algorithms::CRS<T>::A1(int const n) {
    int nnz = 5 * (n-2) * (n-2); // non-zero elements
    ptr.resize(n*n+1);
    size = n*n;
    index.resize(nnz);
    value.resize(nnz);

    int valCount = 0;

    for(int i=0; i<n; ++i)
        ptr[i] = 0;
    for(int i=1; i<n-1; ++i) {
        ptr[i*n] = ptr[i*n-1];
        ptr[i*n+1] = ptr[i*n];
        for(int k=1; k<n-1; ++k) {
            value[valCount] = 1;
            index[valCount++] = (i-1)*n+k;
            value[valCount] = 1;
            index[valCount++] = i*n+k-1;
            value[valCount] = -4;
            index[valCount++] = i*n+k;
            value[valCount] = 1;
            index[valCount++] = i*n+k+1;
            value[valCount] = 1;
            index[valCount++] = (i+1)*n+k;
            ptr[i*n+k+1] = ptr[i*n+k] + 5;
        }
    }
    for(int i=0; i<n; ++i)
        ptr[n*n-n+i] =  ptr[n*n-n-1];
    ptr[n*n] = nnz;
}


template <class T>
void algorithms::CRS<T>::diag(QVector<T> const & diag) {
    int n = diag.size();
    int nnz = n; // non-zero elements
    ptr.resize(n+1);
    size = n;
    index.resize(nnz);
    value.resize(nnz);
    for(int i=0; i<n; ++i) {
        ptr[i] = i;
        value[i] = diag[i];
        index[i] = i;
    }
    ptr[n] = nnz;
}

template <class T>
void algorithms::CRS<T>::eye(int const n)
{
    int nnz = n*n; // non-zero elements
    ptr.resize(n*n+1);
    size = n*n;
    index.resize(nnz);
    value.resize(nnz);

    for(int i=0; i<n*n; ++i) {
        ptr[i] = 0+i;
        value[i] = 1;
        index[i] = i;
    }
    ptr[n*n] = nnz;
}

template <class T>
void algorithms::CRS<T>::full(QVector<QVector<T> > & full) const {
    full.resize(size);
    for(int i=0; i<size; ++i) {
        full[i].resize(size);
    }

    for(int i=0; i<size; ++i) {
        for(int j=ptr[i]; j<ptr[i+1]; ++j)
            full[i][index[j]] = value[j];
    }
}

template <class T>
int algorithms::CRS<T>::getSize() const {
    return size;
}

template <class T>
T algorithms::CRS<T>::getValue(int const i, int const j) const {
    for(int k=ptr[i]; k<ptr[i+1]; ++k) {
        if(index[k]==j)
            return value[k];
    }
    return 0;
}

template <class T>
algorithms::CRS<T> algorithms::CRS<T>::multCRSCRS(CRS<T> const & rMat) const {
    algorithms::CRS<T> mult;
    mult.size=size;
    mult.ptr.resize(size+1);
    int j;
    mult.ptr[0]=0;
    for(int i=0; i<size; ++i) {
        for(j=rMat.ptr[i]; j<rMat.ptr[i+1]; ++j) {
            mult.value.append(rMat.value[j]*value[i]);
            mult.index.append(rMat.index[j]);
        }
        mult.ptr[i+1] = j;
    }
    mult.ptr[size] = mult.value.size();
    return mult;
}

template <class T>
T algorithms::CRS<T>::multRowQVector(int const i, QVector<T> const & vec) const {
    T res=0;
    for(int j=ptr[i]; j<ptr[i+1]; ++j) {
        res += value[j] * vec[index[j]];
    }
    return res;
}

template <class T>
T algorithms::CRS<T>::multRowQVectorAbs(int const i, QVector<T> const & vec) const {
    T res=0;
    for(int j=ptr[i]; j<ptr[i+1]; ++j) {
        res += fabs(value[j] * vec[index[j]]);
    }
    return res;
}

template <class T>
algorithms::CRS<T> algorithms::operator *(T const & scalar, algorithms::CRS<T> const & mat) {
    CRS<T> mult;
    mult = mat;
    for(int i=0; i<mat.value.size(); ++i) {
        mult.value[i] *= scalar;
    }
    return mult;
}

template <class T>
QVector<T> algorithms::operator *(T const & scalar, QVector<T> const & vec) {
    QVector<T> mult = vec;
    for(int i=0; i<vec.size(); ++i) mult[i] *= scalar;
    return mult;
}

template <class T>
QVector<T> algorithms::addQVectors(const QVector<T> & lhs, const QVector<T> & rhs) {
    QVector<T> sum = lhs;
    for(int i=0; i<sum.size(); ++i) sum[i] += rhs[i];
    return sum;
}
