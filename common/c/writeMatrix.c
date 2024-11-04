/********************************
Author: Sravanthi Kota Venkata
********************************/

#include <stdio.h>
#include <stdlib.h>
#include "sdvbs_common.h"

void writeMatrix(I2D* input, char* inpath)
{
    int rows,cols, i, j;

    rows = input->height;
    cols = input->width;

    for(i=0; i<rows; i++)
    {
        for(j=0; j<cols; j++)
        {
            printf("%d\t", subsref(input, i, j));
        }
        printf("\n");
    }

}



