#ifndef RPO_INCLUDED
#define RPO_INCLUDED

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

#include <iostream>
#include <omp.h>
#include <vector>

#include "sorting.hpp"
#include "rect3dmm.hpp"


#include "Algo.hpp"
#include "Image.hpp"
#include "PO.hpp"

#define OMP


template<typename T>
void RPO(const Image<T> &I, int L, Image<T> &RPO1, Image<T> &RPO2, Image<T> &RPO3, Image<T> &RPO4, Image<T> &RPO5,Image<T> &RPO6,Image<T> &RPO7,int nb_core){


// #################### Definition of the orientations ##############################

	// orientation vector
	std::vector<int> orientation1(3);
	orientation1[0] = 1;
	orientation1[1] = 0;
	orientation1[2] = 0;
	std::vector<int> orientation2(3);
	orientation2[0] = 0;
	orientation2[1] = 1;
	orientation2[2] = 0;
	std::vector<int> orientation3(3);
	orientation3[0] = 0;
	orientation3[1] = 0;
	orientation3[2] = 1;
	std::vector<int> orientation4(3);
	orientation4[0] = 1;
	orientation4[1] = 1;
	orientation4[2] = 1;
	std::vector<int> orientation5(3);
	orientation5[0] = 1;
	orientation5[1] = 1;
	orientation5[2] = -1;
	std::vector<int> orientation6(3);
	orientation6[0] = -1;
	orientation6[1] = 1;
	orientation6[2] = 1;
	std::vector<int> orientation7(3);
	orientation7[0] = -1;
	orientation7[1] = 1;
	orientation7[2] = -1;
	
	// ########################## Dilation + Add border on I #########################
	
	// Dilatation
	Image<T> I_dilat=I.Copy_image();
	
	rect3dminmax(I_dilat.GetPointer(), I_dilat.Dimx(), I_dilat.Dimy(), I_dilat.Dimz(), 3, 3, 3, false);	
	
	Image<T> I_dilat_bordered=I_dilat.Add_border(2);
	I_dilat.ClearImage();


	RPO1.Copy_image(I_dilat_bordered);
	RPO2.Copy_image(I_dilat_bordered);
	RPO3.Copy_image(I_dilat_bordered);
	RPO4.Copy_image(I_dilat_bordered);
	RPO5.Copy_image(I_dilat_bordered);
	RPO6.Copy_image(I_dilat_bordered);
	RPO7.Copy_image(I_dilat_bordered);


	// ########################## Sort the grey level intensity in a vector #########################
	std::vector<long> index_image=sort_image_value<T,long>(I_dilat_bordered.GetPointer(), I_dilat_bordered.ImageSize());
	
	// #################### Compute the 7 RPO orientations ####################################
	
	std::cout<<"------- RPO computation with scale " <<L<< "-------"<<std::endl;
	// Calling PO for each orientation
	   omp_set_num_threads(nb_core);
	
	#ifdef OMP
	#pragma omp parallel shared(I_dilat_bordered, index_image)
	{
		#pragma omp sections nowait
		{	
			#pragma omp section
			{
			#endif //OMP
			
			 PO_3D<T>(I_dilat_bordered, L, index_image, orientation1, RPO1);
			 std::cout<<"orientation1 1 0 0 : passed"<<std::endl;
			#ifdef OMP
			}
			
			#pragma omp section
			{
				#endif //OMP
			    PO_3D<T>(I_dilat_bordered, L, index_image, orientation2, RPO2);
			    std::cout<<"orientation2 0 1 0 : passed"<<std::endl;
			#ifdef OMP
			}
			#pragma omp section
			{
				#endif
			    PO_3D<T>(I_dilat_bordered, L, index_image, orientation3, RPO3);
			    std::cout<<"orientation3 0 0 1 : passed"<<std::endl;
			
			#ifdef OMP
			}
			#pragma omp section
			{
			 	#endif
			    PO_3D<T>(I_dilat_bordered, L, index_image, orientation4, RPO4);
			    std::cout<<"orientation4 1 1 1 : passed"<<std::endl;
			
			#ifdef OMP
			}
			#pragma omp section
			{
				#endif
			    PO_3D<T>(I_dilat_bordered, L, index_image, orientation5, RPO5);
			    std::cout<<"orientation5 1 1 -1 : passed"<<std::endl;
			
			#ifdef OMP
			}
			#pragma omp section
			{
				#endif
			    PO_3D<T>(I_dilat_bordered, L, index_image, orientation6, RPO6);
			    std::cout<<"orientation6 -1 1 1 : passed"<<std::endl;
			
			#ifdef OMP
			}
			#pragma omp section
			{
				#endif
			    PO_3D<T>(I_dilat_bordered, L, index_image, orientation7, RPO7);
			    std::cout<<"orientation7 -1 1 -1 : passed"<<std::endl;
			
			#ifdef OMP
			}
		}
	}			
	#endif
	
	 std::cout<<"RPO computation completed"<<std::endl;
	 
	 
	
	I_dilat_bordered.ClearImage();
	
	// Minimum between the computed RPO on the dilation and the initial image and remove border
	
	RPO1.Remove_border(RPO1.GetBorder());
	RPO2.Remove_border(RPO2.GetBorder());
	RPO3.Remove_border(RPO3.GetBorder());
	RPO4.Remove_border(RPO4.GetBorder());
	RPO5.Remove_border(RPO5.GetBorder());
	RPO6.Remove_border(RPO6.GetBorder());
	RPO7.Remove_border(RPO7.GetBorder());

	MinCrush(RPO1, I);
	MinCrush(RPO2, I);
	MinCrush(RPO3, I);
	MinCrush(RPO4, I);
	MinCrush(RPO5, I);
	MinCrush(RPO6, I);
	MinCrush(RPO7, I);
	
}

#endif //RPO_INCLUDED
		
		
