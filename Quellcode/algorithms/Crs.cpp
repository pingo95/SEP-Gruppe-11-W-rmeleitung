#include "Crs.h"

algorithms::CRS::CRS() {

}

algorithms::CRS::CRS(const algorithms::CRS &rhs) {
    this->size = rhs.size;
    this->index = rhs.index;
    this->ptr = rhs.ptr;
    this->value = rhs.value;
}

algorithms::CRS& algorithms::CRS::operator=(algorithms::CRS const &rhs) {
    this->ptr = rhs.ptr;
    this->index = rhs.index;
    this->value = rhs.value;
    return *this;
}

algorithms::CRS algorithms::CRS::operator*(CRS const &rMat) {
    algorithms::CRS mult; mult.size=this->size; mult.ptr.resize(size+1);
    int j=0; mult.ptr[0]=0;
    for(int i=0; i<mult.size; ++i) {
        for(j=rMat.ptr[i]; j<rMat.ptr[i+1]; ++j) {
            value.append(rMat.value[j]*this->value[i]);
            index.append(rMat.index[j]);
        }
        mult.ptr[i+1] = j;
    }
    mult.ptr[size] = mult.value.size();
    return mult;
}

QVector<double> algorithms::CRS::operator*(QVector<double> const &vec) {
    QVector<double> res(this->size);
    for(int i=0; i<size; i++) {
        res[i] = 0;
        for(int j=this->ptr[i]; j<this->ptr[i+1]; ++j) {
            res[i] += this->value[j] * vec[this->index[j]];
        }
    }
    return res;
}

algorithms::CRS algorithms::CRS::operator+(algorithms::CRS const &rMat) {
    assert(this->size==rMat.size);
    algorithms::CRS diff; diff.size=this->size; diff.ptr.resize(size+1);
    int jl, jr;
    for(int i=0; i<diff.size; i++) {
        if(i==0) diff.ptr[i] = 0;
        else diff.ptr[i] = diff.ptr[i-1];
        jl = this->ptr[i]; jr = rMat.ptr[i];
        while(jl < this->ptr[i+1] && jl < rMat.ptr[i+1]) {
            if(this->index[jl] < rMat.index[jr]) {
                diff.index.append(jl);
                diff.value.append(this->value[jl]);
                jl++;
            }
            else {
                if(this->index[jl] > rMat.index[jr]) {
                    diff.index.append(jr);
                    diff.value.append(rMat.value[jr]);
                    jr++;
                }
                else {
                    diff.index.append(jl);
                    diff.value.append(this->value[jl]+rMat.value[jr]);
                    jl++; jr++;
                }
            }
            diff.ptr[i]++;
        }
    }
    diff.ptr[size]=diff.value.size();
    return diff;
}

algorithms::CRS algorithms::CRS::operator-(algorithms::CRS const &rMat) {
    assert(this->size==rMat.size);
    algorithms::CRS diff; diff.size=this->size; diff.ptr.resize(size+1);
    int jl, jr;
    for(int i=0; i<diff.size; i++) {
        if(i==0) diff.ptr[i] = 0;
        else diff.ptr[i] = diff.ptr[i-1];
        jl = this->ptr[i]; jr = rMat.ptr[i];
        while(jl < this->ptr[i+1] && jl < rMat.ptr[i+1]) {
            if(this->index[jl] < rMat.index[jr]) {
                diff.index.append(jl);
                diff.value.append(this->value[jl]);
                jl++;
            }
            else {
                if(this->index[jl] > rMat.index[jr]) {
                    diff.index.append(jr);
                    diff.value.append(-rMat.value[jr]);
                    jr++;
                }
                else {
                    diff.index.append(jl);
                    diff.value.append(this->value[jl]-rMat.value[jr]);
                    jl++; jr++;
                }
            }
            diff.ptr[i]++;
        }
    }
    diff.ptr[size]=diff.value.size();
    return diff;
}

void algorithms::CRS::A1(int const n)
{
    int nnz = 5 * (n-2) * (n-2); // non-zero elements
    ptr.resize(n*n+1);
    size = n*n;
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
    ptr[n*n] = nnz;
}


algorithms::CRS& algorithms::CRS::diag(QVector<double> const &diag) {
    int n = diag.size();
    int nnz = n*n; // non-zero elements
    ptr.resize(n*n+1);
    size = n*n;
    index.resize(nnz);
    value.resize(nnz);
    for(int i=0; i<n*n; ++i) {
        ptr[i] = i;
        value[i] = diag[i];
        index[i] = i;
    }
    ptr[n*n] = nnz;
    return *this;
}

void algorithms::CRS::eye(int const n)
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

algorithms::CRS algorithms::operator*(double const &scalar, algorithms::CRS const &Mat) {
    algorithms::CRS mult;
    mult = Mat;
    for(int i=0; i<Mat.value.size(); i++) {
        mult.value[i] *= scalar;
    }
    return mult;
}
