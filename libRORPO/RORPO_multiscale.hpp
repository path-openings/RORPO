#ifndef RORPO_MULTISCALE_INCLUDED
#define RORPO_MULTISCALE_INCLUDED

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
#include <fstream>
#include <sstream>
#include <vector>

#include "rect3dmm.hpp"
#include "RORPO.hpp"


template<typename PixelType, typename MaskType>
Image<PixelType> RORPO_multiscale(Image<PixelType> &I, std::vector<int>S_list, int nb_core, int debug_flag, Image<MaskType> &Mask)
{
    
    // ---------------------- Mask Image -----------------------------------
	if (Mask.ImageSize() > 0) // A mask image is given
	{
   		if (debug_flag){
		std::cout<<"Mask Application"<<std::endl;
		std::cout<<"Dilation of the Mask by a ball of radius "<<S_list[S_list.size()-1]/2<<std::endl;
		}
		int r_dilat= int(S_list[S_list.size()-1]/2);
		Image<MaskType> Mask_dilat=Mask.Copy_image();
		
		rect3dminmax(Mask_dilat.GetPointer(), Mask_dilat.Dimx(), Mask_dilat.Dimy(), Mask_dilat.Dimz(), r_dilat, r_dilat, r_dilat, false);	
		
   		// Application of the dilated mask to input image
		MaskImage<PixelType, MaskType>(I, Mask_dilat);
		Mask_dilat.ClearImage();
	}
    

	// ####################### Computation of RORPO for each scale #######################
	
	Image<PixelType> Multiscale(I.Dimx(), I.Dimy(), I.Dimz());

	std::vector<int>::iterator it;
	
	for (it=S_list.begin();it!=S_list.end();++it)
	{
		Image<PixelType> One_Scale = RORPO<PixelType>(I, *it, nb_core);
		// ----------------- Max of scales ---------------
	    MaxCrush(Multiscale, One_Scale); 
	}

    // ----------------- Dynamic Enhancement ---------------
	// Find Max value of output_buffer
	int max_value_RORPO = Multiscale.Max_value();
	int max_value_I = I.Max_value();

	//std::cout<<"max_output and max_value "<<max_output<<";"<<max_value<<std::endl;
	
		// Contrast Enhancement
	for (int z = 0; z < Multiscale.Dimz() ; ++z){
		for (int y = 0; y < Multiscale.Dimy(); ++y){
			for (int x = 0; x < Multiscale.Dimx(); ++x){
				Multiscale(x, y, z) = (PixelType)((Multiscale(x, y, z) / (float)max_value_RORPO ) * max_value_I);
			}
		}
	}

	MinCrush(Multiscale, I);
		
		
	if (Mask.ImageSize() > 0) // A mask image is given
	{
		// Application of the non dilated mask to output
    	MaskImage<PixelType, MaskType>(Multiscale, Mask);
	}

	return Multiscale;
}

#endif // RORPO_MULTISCALE_INCLUDED
