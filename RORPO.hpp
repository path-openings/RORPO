#ifndef RORPO_INCLUDED
#define RORPO_INCLUDED

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

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <omp.h>
#include <cstdlib>

#include "sorting.hpp"
#include "Algo.hpp"
#include "Geodilation.hpp"
#include "RPO.hpp"

extern "C" {
	#include "nifti1_io.h"
}

template<typename T>
Image<T> RORPO(const Image<T> &I, int L, int nb_core)
{

	// ######################################### RPO  #########################################
	
	// the 7 RPO images with a 2-pixel border 
	Image<T> RPO1(I.Dimx() + 4, I.Dimy() + 4, I.Dimz() + 4, 2);
	Image<T> RPO2(I.Dimx() + 4, I.Dimy() + 4, I.Dimz() + 4, 2);
	Image<T> RPO3(I.Dimx() + 4, I.Dimy() + 4, I.Dimz() + 4, 2);
	Image<T> RPO4(I.Dimx() + 4, I.Dimy() + 4, I.Dimz() + 4, 2);
	Image<T> RPO5(I.Dimx() + 4, I.Dimy() + 4, I.Dimz() + 4, 2);
	Image<T> RPO6(I.Dimx() + 4, I.Dimy() + 4, I.Dimz() + 4, 2);
	Image<T> RPO7(I.Dimx() + 4, I.Dimy() + 4, I.Dimz() + 4, 2);
	
	RPO(I, L, RPO1, RPO2, RPO3, RPO4, RPO5,RPO6, RPO7, nb_core);
	
	
	 // ######################################### Limit Orientations Treatment #########################################

	 // ------------------------- Computation of Imin ----------------------------
	 
	 // ---- Imin limit case 4 orientations ----
	 
	 Image<T> Imin4(I.Dimx(), I.Dimy(), I.Dimz());
	  
	 //horizontal + vertical + diag1 + diag4 
	 Image<T> Imin4_1 = RPO1.Copy_image();
	 MinCrush(Imin4_1, RPO2);
	 MinCrush(Imin4_1, RPO4);
	 MinCrush(Imin4_1, RPO7);
	 
	 MaxCrush(Imin4, Imin4_1);
	 Imin4_1.ClearImage();

     //horizontal + vertical + diag2 + diag3
     Image<T> Imin4_2 = RPO1.Copy_image();
	 MinCrush(Imin4_2, RPO2);
	 MinCrush(Imin4_2, RPO5);
	 MinCrush(Imin4_2, RPO6);
	 
	 MaxCrush(Imin4, Imin4_2);
	 Imin4_2.ClearImage();


     //horizontal + profondeur + diag2+ diag4
     Image<T> Imin4_3 = RPO1.Copy_image();
	 MinCrush(Imin4_3, RPO3);
	 MinCrush(Imin4_3, RPO5);
	 MinCrush(Imin4_3, RPO7);

	 MaxCrush(Imin4, Imin4_3);
	 Imin4_3.ClearImage();
	 
	
     //horizontal + profondeur + diag1+ diag3
     Image<T> Imin4_4 = RPO1.Copy_image();
	 MinCrush(Imin4_4, RPO3);
	 MinCrush(Imin4_4, RPO4);
	 MinCrush(Imin4_4, RPO6);

	 MaxCrush(Imin4, Imin4_4);
	 Imin4_4.ClearImage();
	 
     //vertical + profondeur + diag1+ diag2
     Image<T> Imin4_5 = RPO2.Copy_image();
	 MinCrush(Imin4_5, RPO3);
	 MinCrush(Imin4_5, RPO4);
	 MinCrush(Imin4_5, RPO5);

	 MaxCrush(Imin4, Imin4_5);
	 Imin4_5.ClearImage();	 

     //vertical + profondeur + diag3+ diag4
     Image<T> Imin4_6 = RPO2.Copy_image();
	 MinCrush(Imin4_6, RPO3);
	 MinCrush(Imin4_6, RPO6);
	 MinCrush(Imin4_6, RPO7);
 
 	 MaxCrush(Imin4, Imin4_6);
	 Imin4_6.ClearImage();    

 

	 // ---- Imin limit case 5 orientations ----
	 Image<T> Imin5 = RPO4.Copy_image();
	 MinCrush(Imin5, RPO5);
	 MinCrush(Imin5, RPO6);
	 MinCrush(Imin5, RPO7);
	
	
	// ######################################### Sorting RPO orientations #########################################
	
	Image<T> RPOt1 = RPO1.Copy_image();
	Image<T> RPOt2 = RPO2.Copy_image();
	Image<T> RPOt3 = RPO3.Copy_image();
	Image<T> RPOt4 = RPO4.Copy_image();
	Image<T> RPOt5 = RPO5.Copy_image();
	Image<T> RPOt6 = RPO6.Copy_image();
	Image<T> RPOt7 = RPO7.Copy_image();
	
	 // Clear Images which are non useful anymore
	 RPO1.ClearImage();
	 RPO2.ClearImage();
	 RPO3.ClearImage();
	 RPO4.ClearImage();
	 RPO5.ClearImage();
	 RPO6.ClearImage();
	 RPO7.ClearImage();
	
    sorting(RPOt1, RPOt2, RPOt3, RPOt4, RPOt5, RPOt6, RPOt7, RPOt1.ImageSize());
	
	 // Clear Images which are non useful anymore
    RPOt1.ClearImage();
    RPOt5.ClearImage();
    RPOt6.ClearImage();

	// Compute RORPO without limit orientations
    Image<T> RORPO_res = Diff(RPOt7, RPOt4);
    RPOt7.ClearImage();
    
    
   /* int t[]={3,RORPO_res.Dimx(),RORPO_res.Dimy(),RORPO_res.Dimz(),0,0,0,0};
	nifti_image* nim=nifti_make_new_nim(t,4,1);
	
	nim->data=(void*)RORPO_res.GetPointer();
	int test=nifti_set_filenames(nim,"../Results/RORPO_pas_limit_ori_30.nii",0,nim->byteorder);
	nifti_image_write(nim);
	
	nim->data=(void*)RPOt4.GetPointer();
	int test2=nifti_set_filenames(nim,"../Results/RPOt4_30.nii",0,nim->byteorder);
	nifti_image_write(nim);*/


    // ------------------------- Computation of Imin2 ----------------------------
    //geodesic reconstruction of RPO6 in RPO4
    Image<T> RPO6_geo = Geodilation(RPOt2, RPOt4, 18, -1);
    RPOt2.ClearImage();

    //geodesic reconstruction of RPO5 in RPO4
    Image<T> RPO5_geo = Geodilation(RPOt3, RPOt4, 18, -1);;
    RPOt3.ClearImage();
    RPOt4.ClearImage();

    // ---- Imin2 limit case 4 orientations ----
    Image<T> Imin2_4 = Imin4.Copy_image();
    MinCrush(Imin2_4, RPO5_geo);
    RPO5_geo.ClearImage();


    // ---- Imin2 limit case 5 orientations ----
    Image<T> Imin2_5 = Imin5.Copy_image();
    MinCrush(Imin2_5, RPO6_geo);
    
    /*nim->data=(void*)Imin5.GetPointer();
	int test7=nifti_set_filenames(nim,"../Results/Imin5.nii",0,nim->byteorder);
	nifti_image_write(nim);
	
    nim->data=(void*)RPO6_geo.GetPointer();
	int test6=nifti_set_filenames(nim,"../Results/RPO6_geo.nii",0,nim->byteorder);
	nifti_image_write(nim);*/
	
	
    RPO6_geo.ClearImage();


    // ------------------------- Final Result --------------------------------

	Image<T> Diff_Imin4 = Diff(Imin4, Imin2_4);
	Image<T> Diff_Imin5 = Diff(Imin5, Imin2_5);
	
	/*nim->data=(void*)Diff_Imin4.GetPointer();
	int test3=nifti_set_filenames(nim,"../Results/Diff_Imin4.nii",0,nim->byteorder);
	nifti_image_write(nim);
	
	nim->data=(void*)Diff_Imin5.GetPointer();
	int test4=nifti_set_filenames(nim,"../Results/Diff_Imin5.nii",0,nim->byteorder);
	nifti_image_write(nim);
	
	nim->data=(void*)Imin2_5.GetPointer();
	int test8=nifti_set_filenames(nim,"../Results/Imin2_5.nii",0,nim->byteorder);
	nifti_image_write(nim);*/
	
	Imin4.ClearImage();
	Imin2_4.ClearImage();
	Imin5.ClearImage();
	Imin2_5.ClearImage();
	
	MaxCrush(RORPO_res, Diff_Imin4);
	
	/*nim->data=(void*)RORPO_res.GetPointer();
	int test5=nifti_set_filenames(nim,"../Results/RORPO_OL4.nii",0,nim->byteorder);
	nifti_image_write(nim);*/
	
	MaxCrush(RORPO_res, Diff_Imin5);
	

   return RORPO_res;
   
}

#endif // RORPO_INCLUDED
