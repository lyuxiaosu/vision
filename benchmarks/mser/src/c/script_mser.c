/********************************
Author: Sravanthi Kota Venkata
********************************/

#include "mser.h"

#define min(a,b) (a<b)?a:b
#define max(a,b) (a>b)?a:b

#define GENERATE_OUTPUT 1

int main(int argc, char* argv[])
{

    int which_image;
    int i, j, k;
    I2D *idx;
    I2D *I;
    I2D *It = NULL;
    int rows=196, cols=98;
    int minVal = 1000;
    int maxVal = -1000;
    int lev = 10;

    unsigned int* start, *endC, *elapsed;

    I = readImage(NULL);
    rows = I->height;
    cols = I->width;

    It = iMallocHandle(I->height, I->width);
    
    if (It == NULL) {
   	printf("Malloc memory for It failed\n");
	return -1;
    }

    k = 0;
    for(i=0; i<cols; i++)
    {
        for(j=0; j<rows; j++)
        {
            asubsref(It,k++) = subsref(I,j,i);
        }
    }
   
    //start = photonStartTiming();
    idx = mser(It, 2);
    //endC = photonEndTiming();
    //elapsed = photonReportTiming(start, endC);

    //printf("Input size\t\t- (%dx%d)\n", rows, cols);
#ifdef CHECK   
    /** Self checking - use expected.txt from data directory  **/
    {
        int tol, ret=0;
        tol = 1;
#ifdef GENERATE_OUTPUT
        writeMatrix(idx, NULL);
#endif
    }
    /** Self checking done **/
#endif    
    //photonPrintTiming(elapsed);
   
    //free(start); 
    //free(endC); 
    //free(elapsed); 
    iFreeHandle(idx);
    iFreeHandle(I);
    iFreeHandle(It);
    
    return 0;
}

