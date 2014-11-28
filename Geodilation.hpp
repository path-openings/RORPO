#ifndef GEODILATION_INCLUDED
#define GEODILATION_MODIF_INCLUDED

/* Copyright (C) 2014 Odyssee Merveille
 
This file is part of libRORPO

    libRORPO is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libRORPO is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libRORPO.  If not, see <http://www.gnu.org/licenses/>.
   
*/

#include <stdlib.h>
#include <iostream>
#include <string>


extern "C" {
	#include "mcimage.h"
	#include "mccodimage.h"
	#include "lgeodesic.h"
}


template<typename T>
Image<T> Geodilation(Image<T> &G, Image<T> &R, int connex, int niter)
{
	Image<T> Geodilat(G.Dimx(), G.Dimy(), G.Dimz());
	
	// Pink Images
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
		std::cerr<<"Error in Geodilation : ImageType not known"<<std::endl;

    imageG=allocheader(NULL,G.Dimx(),G.Dimy(),G.Dimz(),typepixel);
    imageG->image_data= G.GetPointer();

    imageR=allocheader(NULL,G.Dimx(),G.Dimy(),G.Dimz(),typepixel);
    imageR->image_data= R.GetPointer();

    temp=copyimage(imageG);

    lgeodilat(temp,imageR,connex,niter);

    for (int z = 0; z<G.Dimz()  ; ++z){
		for (int y = 0; y<G.Dimy() ; ++y){
			for (int x = 0; x<G.Dimx(); ++x){
					Geodilat(x, y, z) = ((T *)(temp->image_data))[x + y * G.Dimx() + z * G.Dimx() * G.Dimy()];
			}
		}
	}
	
    free(imageR);
    free(imageG);
    free(temp);

   return Geodilat;
}

#endif // GEODILATION_INCLUDED
