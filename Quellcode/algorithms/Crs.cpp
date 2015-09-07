#include "Crs.h"

algorithms::CRS::CRS() {

}

algorithms::CRS algorithms::CRS::operator-(CRS const &rMat) {

}

algorithms::CRS& algorithms::CRS::operator=(CRS const &rhs) {

}

void algorithms::CRS::A1(int const n)
{
    int nnz = 5 * (n-2) * (n-2); // non-zero elements
    ptr.resize(n*n+1);
    index.resize(nnz);
    value.resize(nnz);

    int valCount = 0;

    for(int i=0; i<n; ++i) {
        ptr[i] = 0;
    }
    int j;
    for(int i=1; i<n-1; ++i) {
        j=i*n;
        ptr[j] = ptr[j-1];
        ptr[j+1] = ptr[j];
        for(int k=1; k<n-1; ++k) {
            value[valCount] = 1;
            index[valCount++] = j-n+k;
            value[valCount] = 1;
            index[valCount++] = j+k-1;
            value[valCount] = -4;
            index[valCount++] = j+k;
            value[valCount] = 1;
            index[valCount++] = j+k+1;
            value[valCount] = 1;
            index[valCount++] = j+k+n;
            ptr[j+k+1] = ptr[j+k] + 5;
        }
    }
    for(int i=0; i<n; ++i) {
        ptr[n*n-n+i+1] =  ptr[n*n-n];
    }
    ptr[n*n] = nnz + 1;
}

void algorithms::CRS::eye(int const n)
{
    int nnz = n*n; // non-zero elements
    ptr.resize(n*n+1);
    index.resize(nnz);
    value.resize(nnz);

    for(int i=0; i<n*n; ++i) {
        ptr[i] = 0+i;
        value[i] = 1;
        index[i] = i;
    }
    ptr[n*n] = nnz + 1;
}

algorithms::CRS algorithms::operator*(double const &scalar, algorithms::CRS const &Mat) {

}
