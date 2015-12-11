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

#include "RORPO/pink/rect3dmm.hpp"
#include "RORPO/RORPO.hpp"
#include "RORPO/Algo.hpp"


template<typename PixelType, typename MaskType>
Image3D<PixelType> RORPO_multiscale(Image3D<PixelType> &I,
                                    std::vector<int>S_list,
                                    int nb_core,
                                    int debug_flag,
                                    Image3D<MaskType> &Mask)
{

    // ################## Computation of RORPO for each scale ##################
	
    Image3D<PixelType> Multiscale(I.Dimx(), I.Dimy(), I.Dimz());

	std::vector<int>::iterator it;
	
	for (it=S_list.begin();it!=S_list.end();++it)
	{
        Image3D<PixelType> One_Scale =
                RORPO<PixelType, MaskType>(I, *it, nb_core, Mask);

        // Max of scales
	    max_crush(Multiscale, One_Scale); 
	}

    // ----------------- Dynamic Enhancement ---------------
	// Find Max value of output_buffer
	int max_value_RORPO = Multiscale.max_value();
	int max_value_I = I.max_value();

    // Contrast Enhancement
	for (int z = 0; z < Multiscale.Dimz() ; ++z){
		for (int y = 0; y < Multiscale.Dimy(); ++y){
			for (int x = 0; x < Multiscale.Dimx(); ++x){
                Multiscale(x, y, z) =
                        (PixelType)((Multiscale(x, y, z)
                                     / (float)max_value_RORPO ) * max_value_I);
			}
		}
	}

	min_crush(Multiscale, I);
		
    if (!Mask.empty()) // A mask image is given
	{
		// Application of the non dilated mask to output
        mask_image<PixelType, MaskType>(Multiscale, Mask);
	}
	return Multiscale;
}

#endif // RORPO_MULTISCALE_INCLUDED
