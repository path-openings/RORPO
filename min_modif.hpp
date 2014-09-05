#ifndef MIN_MODIF_INCLUDED
#define MIN_MODIF_INCLUDED


#include <stdlib.h>
#include <iostream>
#include <string>

extern "C" {
	#include "mcimage.h"
	#include "mccodimage.h"
	#include "larith.h"
}

//typedef unsigned char T;
// The result is stored in an other buffer
template<typename T>
T* min_alloc(T* I1, T* I2, int dimx, int dimy, int dimz)
{
    struct xvimage * image1;
    struct xvimage * image2;
    struct xvimage * image3;
    int32_t typepixel;

	if (sizeof(T)==1)
   		typepixel = VFF_TYP_1_BYTE;
   	else if (sizeof(T)==2)
		typepixel = VFF_TYP_2_BYTE;
	else if (sizeof(T)==4)
		typepixel = VFF_TYP_4_BYTE;
	else
		return NULL;
		
    image1=allocheader(NULL,dimx,dimy,dimz,typepixel);
    image1->image_data=I1;

    image2=allocheader(NULL,dimx,dimy,dimz,typepixel);
    image2->image_data=I2;

    image3=copyimage(image1);
    // lmin ecrase image3->image_data
    lmin(image3,image2);

    T* res=(T*)(image3->image_data);

    free(image1);
    free(image2);
    free(image3);

   return res;
}

//The result is stored in I1
template<typename T>
int min_crush(T* I1, T* I2, int dimx, int dimy, int dimz)
{
    struct xvimage * image1;
    struct xvimage * image2;
    int32_t typepixel;
    
	if (sizeof(T)==1)
   		typepixel = VFF_TYP_1_BYTE;
   	else if (sizeof(T)==2)
		typepixel = VFF_TYP_2_BYTE;
	else if (sizeof(T)==4)
		typepixel = VFF_TYP_4_BYTE;
	else
		return 1;

    image1=allocheader(NULL,dimx,dimy,dimz,typepixel);
    image1->image_data=I1;

    image2=allocheader(NULL,dimx,dimy,dimz,typepixel);
    image2->image_data=I2;

    lmin(image1,image2);

    free(image1);
    free(image2);
    return 0;
}
#endif // MIN_MODIF_INCLUDED

