/********************************
Author: Sravanthi Kota Venkata
********************************/

#include <stdio.h>
#include <stdlib.h>
#include "sdvbs_common.h"

#define MAX_IMG_SZ (1024*1024) //1MB
I2D* deep_copy_I2D(const I2D* source) {
    if (source == NULL) {
        return NULL;
    }

    // 分配空间并初始化 dest
    I2D* dest = iMallocHandle(source->height, source->width);

    printf("dest height %d width %d\n", dest->height, dest->width);
    if (dest == NULL) {
        return NULL;
    } else {
        printf("malloc success\n");
    }
   
    // 深拷贝数据
    //for (int i = 0; i < source->height * source->width; i++) {
    //    printf("%d ", source->data[i]);
    //}

    //printf("\n");

    return dest;
}


I2D* readImage(const char* pathName)
{
    // Reading BMP image
    char signature[2];   
    int file_size;
    short int reserved1;
    short int reserved2;
    int loc_of_bitmap;

    int size_of_infoheader;
    int width;
    int height;
    short int number_of_planes;
    short int bits_per_pixel;

    int compression_method;
    int bytes_of_bitmap;
    int hori_reso;
    int vert_reso;
    int no_of_colors;
    int no_of_imp_colors;

    int nI,nJ;
    int pixSize;

    unsigned char tempb,tempg,tempr,tempjunk[12];
    int ta;
    I2D* srcImage;

    unsigned char *zInpbuf = malloc(MAX_IMG_SZ);
    if (!zInpbuf) return -1;
    size_t imgSz = read(0, zInpbuf, MAX_IMG_SZ);
    if (imgSz <= 0) return -1;

    {
        //start of header information
	unsigned char *ptr = zInpbuf;
	
        memcpy(signature, ptr, sizeof(signature));
	ptr += sizeof(signature);

	file_size = *(int *)ptr;
	ptr += sizeof(file_size);

	reserved1 = *(short int *)ptr;
	ptr += sizeof(reserved1);

	reserved2 = *(short int *)ptr;
	ptr += sizeof(reserved2);

	loc_of_bitmap = *(int *)ptr;
	ptr += sizeof(loc_of_bitmap);

	size_of_infoheader = *(int *)ptr;
	ptr += sizeof(size_of_infoheader);

	width = *(int *)ptr;
	ptr += sizeof(width);

	height = *(int *)ptr;
	ptr += sizeof(height);

	number_of_planes = *(short int *)ptr;
	ptr += sizeof(number_of_planes);

	bits_per_pixel = *(short int *)ptr;
	ptr += sizeof(bits_per_pixel);

	compression_method = *(int *)ptr;
	ptr += sizeof(compression_method);

	bytes_of_bitmap = *(int *)ptr;
	ptr += sizeof(bytes_of_bitmap);

	hori_reso = *(int *)ptr;
	ptr += sizeof(hori_reso);

	vert_reso = *(int *)ptr;
	ptr += sizeof(vert_reso);

	no_of_colors = *(int *)ptr;
	ptr += sizeof(no_of_colors);

	no_of_imp_colors = *(int *)ptr;
	ptr += sizeof(no_of_imp_colors);
	//end of header information

        srcImage = iMallocHandle(height, width);
        
        // Conditions to check whether the BMP is interleaved and handling few exceptions
        if(srcImage->height <= 0 || srcImage->width <= 0 || signature[0] != 'B' || signature[1] != 'M'  || ( bits_per_pixel !=24 && bits_per_pixel !=8 ) )
        {
            printf("ERROR in BMP read: The input file is not in standard BMP format");
            return NULL;
        }

	ptr = zInpbuf + loc_of_bitmap;

        if (bits_per_pixel == 8)
        {
            for(nI = (height - 1); nI >= 0 ; nI--)
            {
                for(nJ = 0;nJ < width; nJ++)
                {
		    tempg = *ptr++;
                    subsref(srcImage,nI,nJ) = (int)tempg;
                }
            }
        }
        else if (bits_per_pixel == 24)
        {
            for(nI = (height - 1); nI >= 0 ; nI--)
            {
                for(nJ = 0;nJ < width; nJ++)
                {
		    tempb = *ptr++;  // blue 
            	    tempg = *ptr++;  // green
                    tempr = *ptr++;  // red
                    ta = (3*tempr + 6*tempg + tempb)/10;
                    ta = tempg;
                    subsref(srcImage,nI,nJ) = (int)ta;
                }
            }
        }
        else
        {
            return NULL;
        }

        return srcImage;
    }
}
