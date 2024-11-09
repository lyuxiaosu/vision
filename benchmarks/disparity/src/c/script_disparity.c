/********************************
Author: Sravanthi Kota Venkata
********************************/

#include <stdio.h>
#include <stdlib.h>
#include "disparity.h"

#define GENERATE_OUTPUT 1
int main(int argc, char* argv[])
{
    int rows = 32;
    int cols = 32;
    I2D *imleft, *imright, *retDisparity;

    unsigned int *start, *endC, *elapsed;
    
    int i, j;
    char im1[100], im2[100], timFile[100];
    int WIN_SZ=8, SHIFT=64;
    FILE* fp;
    
    unsigned char *inbuf = malloc(MAX_IMG_SZ);
    if (!inbuf) return -1;

    size_t imgsSz = read(0, inbuf, MAX_IMG_SZ);
    if (imgsSz <= 0) return -1;

    size_t current_offset = 0;
    ImageInfo img_info;
    int image_index = 1;
    int ret = read_next_image(inbuf, imgsSz, &current_offset, &img_info);
    if (ret != 0) {
        printf("image size is 0\n");
        return -1;
    }

    /** Read input image **/
    imleft = readImage_from_buf(inbuf + img_info.offset, img_info.length);
    
    ret = read_next_image(inbuf, imgsSz, &current_offset, &img_info);
    if (ret != 0) {
        printf("No right image\n");
        return -1;
    }

    imright = readImage_from_buf(inbuf + img_info.offset, img_info.length);

    rows = imleft->height;
    cols = imleft->width;

#ifdef test
    WIN_SZ = 2;
    SHIFT = 1;
#endif
#ifdef sim_fast
    WIN_SZ = 4;
    SHIFT = 4;
#endif
#ifdef sim
    WIN_SZ = 4;
    SHIFT = 8;
#endif

    //start = photonStartTiming();
    retDisparity = getDisparity(imleft, imright, WIN_SZ, SHIFT);
    //endC = photonEndTiming();

    //printf("Input size\t\t- (%dx%d)\n", rows, cols);
#ifdef CHECK   
    /** Self checking - use expected.txt from data directory  **/
    {
        int tol, ret=0;
        tol = 2;
#ifdef GENERATE_OUTPUT
        writeMatrix(retDisparity, NULL);
#endif
        //ret = selfCheck(retDisparity, argv[1], tol);
        //if (ret == -1)
        //    printf("Error in Disparity Map\n");
    }
    /** Self checking done **/
#endif

    //elapsed = photonReportTiming(start, endC);
    //photonPrintTiming(elapsed);
    
    iFreeHandle(imleft);
    iFreeHandle(imright);
    iFreeHandle(retDisparity);
    //free(start);
    //free(endC);
    //free(elapsed);

    return 0;
}
