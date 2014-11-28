#ifndef MIN_MODIF_INCLUDED
#define MIN_MODIF_INCLUDED

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

#include "Image.hpp"


// Return the difference between I1 and I2
template<typename T>
const Image<T> Diff(const Image<T> &I1, const Image<T> &I2){
	
	Image<T> Result(I1.Dimx(), I1.Dimy(), I1.Dimz());

	if (I1.ImageSize() != I2.ImageSize()){
		std::cout<<"Error Diff : Size of images are not the same."<<std::endl;
	}
	else {
		for(int z = 0; z < I1.Dimz(); ++z) {
			for(int y = 0 ; y < I1.Dimy(); ++y) {
				for(int x = 0; x < I1.Dimx(); ++x) {
					Result(x, y, z) = I1(x, y, z) - I2(x, y, z);
				}
			}
		}
	}
	return Result;
}



// Min between I1 and I2 into I1.
template<typename T>
int MinCrush(Image<T> &I1, const Image<T> &I2)
{
	if (I1.ImageSize() != I2.ImageSize()){
		std::cout<<"Error MinCrush : Size of images are not the same."<<std::endl;
	}
	else {
		for(int z = 0; z < I1.Dimz(); ++z) {
			for(int y = 0 ; y < I1.Dimy(); ++y) {
				for(int x = 0; x < I1.Dimx(); ++x) {
					I1(x, y, z) = std::min(I1(x, y, z), I2(x, y, z));
				}
			}
		}
	}
    return 0;
}


// Max between I1 and I2 into I1.
template<typename T>
int MaxCrush(Image<T> &I1, const Image<T> &I2)
{
	if (I1.ImageSize() != I2.ImageSize()){
		std::cout<<"Error MaxCrush : Size of images are not the same."<<std::endl;
	}
	else {
		for(int z = 0 ; z < I1.Dimz() ; ++z) {
			for(int y = 0 ; y < I1.Dimy() ; ++y) {
				for(int x = 0 ; x < I1.Dimx() ; ++x) {
					I1(x, y, z) = std::max(I1(x, y, z), I2(x, y, z));
				}
			}
		}
	}
    return 0;
}

// Apply the mask image M to image I
template<typename T1, typename T2>
void MaskImage(Image<T1> &I, const Image<T2> &M){
	if (I.ImageSize() != M.ImageSize()){
	std::cout<<"Error Masking : Size of images are not the same."<<std::endl;
	}
	else {
		for(int z = 0 ; z < M.Dimz() ; ++z) {
			for(int y = 0 ; y < M.Dimy() ; ++y) {
				for(int x = 0 ; x < M.Dimx() ; ++x) {
					if (M(x, y, z) == 0){
						I(x, y, z) = 0;
					}
				}
			}
		}
	}
}


#endif // MIN_MODIF_INCLUDED

