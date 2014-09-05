#ifndef GEODILAT_MODIF_INCLUDED
#define GEODILAT_MODIF_INCLUDED

#include <stdlib.h>
#include <iostream>
#include <string>


extern "C" {
	#include "mcimage.h"
	#include "mccodimage.h"
	#include "lgeodesic.h"
	//#include "Pink/larith.h"
}


template<typename T>
T* geodilat_modif(T* G, T* R, int connex, int niter, int dimx, int dimy, int dimz)
{
    struct xvimage* imageG;
    struct xvimage* imageR;
    struct xvimage* temp;
    int32_t typepixel;
    
	if (sizeof(T)==1)
   		typepixel = VFF_TYP_1_BYTE;
   	else if (sizeof(T)==2)
		typepixel = VFF_TYP_2_BYTE;
	else if (sizeof(T)==4)
		typepixel = VFF_TYP_4_BYTE;
	else
		return NULL;

    imageG=allocheader(NULL,dimx,dimy,dimz,typepixel);
    imageG->image_data=G;

    imageR=allocheader(NULL,dimx,dimy,dimz,typepixel);
    imageR->image_data=R;

    temp=copyimage(imageG);

    lgeodilat(temp,imageR,connex,niter);

    T* res=(T*)(temp->image_data);

    free(imageR);
    free(imageG);
    free(temp);

    /*for (int i=0;i<dimx*dimy*dimz;i++)
        {
            std::cout<<"res["<<i<<"]="<< static_cast<int>(res[i])<<std::endl;

        }*/

   return res;
}

#endif // GEODILAT_MODIF_INCLUDED
