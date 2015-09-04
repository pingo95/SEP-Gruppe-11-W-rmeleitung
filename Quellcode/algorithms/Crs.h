#ifndef CRS_H
#define CRS_H

namespace algorithms {

    class CRS
    {
        int* index;
        int* ptr;
        double* value;
    public:
        CRS();

        void A1(int const n); // 2D-Diskretisierungsmatrix Laplace-Operator
    };

}

#endif // CRS_H
