#ifndef ALGO_INCLUDED
#define ALGO_INCLUDED

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

#include "Image/Image.hpp"


// Return the difference between images image1 and image2
template<typename T>
const Image3D<T> diff(const Image3D<T> &image1, const Image3D<T> &image2){
	
    Image3D<T> result(image1.Dimx(), image1.Dimy(), image1.Dimz());

    if (image1.image_size() != image2.image_size()){
		std::cout<<"Error Diff : Size of images are not the same."<<std::endl;
	}
	else {
        for(int z = 0; z < image1.Dimz(); ++z) {
            for(int y = 0 ; y < image1.Dimy(); ++y) {
                for(int x = 0; x < image1.Dimx(); ++x) {
                    result(x, y, z) = image1(x, y, z) - image2(x, y, z);
				}
			}
		}
	}
	return result;
}



// Min between image1 and image2. Result is stored in image1.
template<typename T>
int min_crush(Image3D<T> &image1, const Image3D<T> &image2)
{
    if (image1.image_size() != image2.image_size()){
        std::cout<<"Error in Algo.hpp (min_crush l 55): "
                   <<"Size of images is not the same."<<std::endl;
	}
	else {
        for(int z = 0; z < image1.Dimz(); ++z) {
            for(int y = 0 ; y < image1.Dimy(); ++y) {
                for(int x = 0; x < image1.Dimx(); ++x) {
                    image1(x, y, z)= std::min(image1(x, y, z), image2(x, y, z));
				}
			}
		}
	}
    return 0;
}


// Max between image1 and image2. Result is stored in image1.
template<typename T>
int max_crush(Image3D<T> &image1, const Image3D<T> &image2)
{
    if (image1.image_size() != image2.image_size()){
        std::cout<<"Error in Algo.hpp (max_crush l 76): "
                   <<"Size of images is not the same."<<std::endl;
	}
	else {
        for(int z = 0 ; z < image1.Dimz() ; ++z) {
            for(int y = 0 ; y < image1.Dimy() ; ++y) {
                for(int x = 0 ; x < image1.Dimx() ; ++x) {
                    image1(x, y, z)= std::max(image1(x, y, z), image2(x, y, z));
				}
			}
		}
	}
    return 0;
}

// Apply the mask image mask to image image
template<typename T1, typename T2>
void mask_image(Image3D<T1> &image, const Image3D<T2> &mask){
    if (image.image_size() != mask.image_size()){
    std::cout<<"Error in Algo.hpp (mask_image l 96): "
               <<"Size of image and mask is not the same."<<std::endl;
	}
	else {
        for(int z = 0 ; z < mask.Dimz() ; ++z) {
            for(int y = 0 ; y < mask.Dimy() ; ++y) {
                for(int x = 0 ; x < mask.Dimx() ; ++x) {
                    if (mask(x, y, z) == 0){
                        image(x, y, z) = 0;
					}
				}
			}
		}
	}
}


#endif // ALGO_INCLUDED

