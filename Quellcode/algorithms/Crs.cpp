#include "Crs.h"

algorithms::CRS::A1(int const n)
{
    int numRows = n*n;
    int nnz = 5 * numRows - 24 * n +28; // non-zero elements
    ptr = new int[numRows];
    index = new int[nnz];
    value = new int[nnz];

    for(int i=0; i<n; i++) {
        ptr[i] = 0;
    }
    for(int i=1; i<n-2; i++) {
        ptr[i*n] = ptr[i*n-1];
        ptr[i*n+1] = ptr[i*n];
        ptr[i*n+2] = ptr[i*n] + 3;
        for(int j = 1; j<(i+1)*n-2; j++) {
            ptr[i*n+2+j] = ptr[i*n+2+j-1] + 4;
        }
        ptr[(i+1)*n-2] = ptr[(i+1)*n-3] + 3;
        ptr[(i+1)*n-1] = ptr[(i+1)*n-2];


}
