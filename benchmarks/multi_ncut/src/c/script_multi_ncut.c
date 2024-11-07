/********************************
Author: Sravanthi Kota Venkata
********************************/

#include <stdio.h>
#include <stdlib.h>
#include "segment.h"

#define GENERATE_OUTPUT 1
int main(int argc, char* argv[])
{
    float sigma = 0.6;
    float k = 4;
    int min_size = 10;
    char im1[256];
    int num_ccs[1] = {0};
    I2D *out;
    I2D* im;
    I2D* seg;
    unsigned int* start, *endC, *elapsed;
    int ret;

    im = readImage(NULL);
    
    //printf("Input size\t\t- (%dx%d)\n", im->height, im->width);

    //start = photonStartTiming();
    seg = segment_image(im, sigma, k, min_size, num_ccs);
    //endC = photonEndTiming();
    //elapsed = photonReportTiming(start, endC);
    out = seg;

#ifdef CHECK   
    /** Self checking - use expected.txt from data directory  **/
    {
        int ret=0;
        float tol = 0;
        
#ifdef GENERATE_OUTPUT
        writeMatrix(out, NULL);
#endif
 
    //ret = selfCheck(out, argv[1], tol);
    //if(ret < 0)
    //    printf("Error in Multi N Cut\n");
    }
#endif

    //photonPrintTiming(elapsed);
  
    iFreeHandle(im); 
    //free(start);
    //free(endC);
    //free(elapsed);
    iFreeHandle(seg);
    
    return 0;
}


