
#include <stdio.h>
#include <stdlib.h>

#include <cblas.h>

int main(int argc, char ** argv)
{
    double * x = (double*) malloc(sizeof(double) * 3);
    double * y = (double*) malloc(sizeof(double) * 3);

    x[0] =  1;
    x[1] =  3;
    x[2] = -2;

    y[0] =  4;
    y[1] = -2;
    y[2] = -1;

    double dot = cblas_ddot(3, x, 1, y, 1);

    printf("dot: %f\n", dot);

    free(x);
    free(y);

    return 0;
}

