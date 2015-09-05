#ifndef CRS_H
#define CRS_H

namespace algorithms {

    class CRS
    {
    //Funktionen:
    public:
        CRS();

        void A1(int const n); // 2D-Diskretisierungsmatrix Laplace-Operator

    //Attribute:
    private:
        int* index;
        int* ptr;
        double* value;
    };
}

#endif // CRS_H
