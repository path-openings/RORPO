#ifndef IMAGE_INCLUDED
#define IMAGE_INCLUDED

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
#include <vector>

template<typename T>
class Image {
	
	public : 
	
		Image(): 
			m_nImageSize(0){}
	
		Image(int dimX, int dimY, int dimZ, int border=0, int value=0):
			 m_nBorder(border), m_nDimx(dimX), m_nDimy(dimY), m_nDimz(dimZ), m_nImageSize(dimX*dimY*dimZ), m_vImage(dimX*dimY*dimZ, value){}
		
		~Image(){}
	
	
		T& operator ()(int x, int y, int z) {
			return m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy];
		}
		const T& operator ()(int x, int y, int z) const {
			return m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy];
		}

		T& operator ()(int i) {
			return m_vImage[i];
		}
		const T& operator ()(int i) const {
			return m_vImage[i];
		}
	
	
		const int Dimx() const {
			return m_nDimx;
		}
		
		const int Dimy() const {
			return m_nDimy;
		}
	
		const int Dimz() const {
			return m_nDimz;
		}
		
		const int ImageSize() const {
			return m_nImageSize;
		}
		
		const int GetBorder() const {
			return m_nBorder;
		}

		std::vector<T>& GetData(){
			return m_vImage;
		}	
		const std::vector<T>& GetData() const {
			return m_vImage;
		}	
		
		const T* GetPointer() const {
			return m_vImage.data();
		}
		T* GetPointer(){
			return m_vImage.data();
		}
	
		// Fill the image with data pointed by p_Pointer
		void Add_data_from_pointer(T* p_Pointer){
	
			for (int z=0; z<m_nDimz ; ++z){
				for (int y=0; y<m_nDimy; ++y){
					for (int x=0; x<m_nDimx; ++x){
				
						m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy]=p_Pointer[x + y * m_nDimx + z * m_nDimx * m_nDimy];
					}		
				}
			}
		}
		
		void ClearImage(){
			m_vImage.clear();
			std::vector<T>().swap(m_vImage);
			m_nImageSize=0;
				
		}	
	
		// Return a new image "bordered_image" which is the self image with a "border"-pixel border
		const Image Add_border(int border, int value=0) const {
	
			Image bordered_image(m_nDimx + 2 * border, m_nDimy + 2 * border, m_nDimz + 2 * border, border, value);
			for (int z=0; z<m_nDimz ; ++z){
				for (int y=0; y<m_nDimy; ++y){
					for (int x=0; x<m_nDimx; ++x){
				
						bordered_image(x + border, y + border, z + border)=m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy];
					}		
				}
			}
			return bordered_image;
		}
		
		// Return a new image "bordered_image" which is the self image with a "border"-pixel border
		void Remove_border(int border) {
			
			int ind=0;
			for (int z = border; z<m_nDimz - border ; ++z){
				for (int y = border; y<m_nDimy - border; ++y){
					for (int x = border; x<m_nDimx - border; ++x){
						m_vImage[ind]=m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy];
						ind++;
					}		
				}
			}
			m_nBorder = 0;
			m_nDimx = m_nDimx - 2 * border;
			m_nDimy = m_nDimy - 2 * border;
			m_nDimz = m_nDimz - 2 * border;
			m_nImageSize = m_nDimx * m_nDimy * m_nDimz;
			m_vImage.resize(m_nImageSize);
		}
	
	// return a new image which is the copy of this 
	const Image<unsigned char> Copy_image_2_uchar() const {
		
		Image<unsigned char> copy(m_nDimx , m_nDimy , m_nDimz , m_nBorder);
		for (int z=0; z<m_nDimz ; ++z){
			for (int y=0; y<m_nDimy; ++y){
				for (int x=0; x<m_nDimx; ++x){
					copy(x, y, z) = (unsigned char) m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy];	
				}
			}
		}
		return copy;
	}
	
	// return a new image which is the copy of this 
	const Image Copy_image() const {
		
		Image copy(m_nDimx , m_nDimy , m_nDimz , m_nBorder);
		copy.GetData() = m_vImage;
		return copy;
	}
	
	// Copy I into this
	void Copy_image(Image &I) {
		if (I.ImageSize() != m_nImageSize)
		{
			m_vImage.resize(I.ImageSize());
			m_vImage=I.GetData();
			m_nBorder = I.GetBorder();
			m_nDimx = I.Dimx();		
			m_nDimy = I.Dimy();
			m_nDimz = I.Dimz();
			m_nImageSize = I.ImageSize();
		}
		else {
			m_vImage=I.GetData();
		}

	}
	
	
	// Return the maximum value of this
	int Min_value(){
		int min = m_vImage[0];
		for (int z=0; z<m_nDimz ; ++z){
			for (int y=0; y<m_nDimy; ++y){
				for (int x=0; x<m_nDimx; ++x){
					if (m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy] < min){
						min = m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy];	
					}
				}
			}
		}
		return min;
	}
	
	
	// Return the maximum value of this
	int Max_value(){
		int max = m_vImage[0];
		for (int z=0; z<m_nDimz ; ++z){
			for (int y=0; y<m_nDimy; ++y){
				for (int x=0; x<m_nDimx; ++x){
					if (m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy] > max){
						max = m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy];	
					}
				}
			}
		}
		return max;
	}
	
	// Return the minimum and maximum value of this
	std::vector<int> MinMax_value(){
		std::vector<int> minmax(2);
		minmax[0] = m_vImage[0];
		minmax[1] = m_vImage[0];
		for (int z=0; z<m_nDimz ; ++z){
			for (int y=0; y<m_nDimy; ++y){
				for (int x=0; x<m_nDimx; ++x){
					if (m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy] > minmax[1]){
						minmax[1] = m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy];	
					}
					if (m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy] < minmax[0]){
						minmax[0] = m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy];	
					}
				}
			}
		}
		return minmax;
	}
	
	// Change the dynamique of image this from [window_min, window_max] to [0, 255]. Intensities smaller than window_min are set to 0 and larger than window_max are set to 255
	void Window_Dynamic(int window_min, int window_max){
	
	for (int z=0; z<m_nDimz ; ++z){
			for (int y=0; y<m_nDimy; ++y){
				for (int x=0; x<m_nDimx; ++x){

					if (m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy] <= window_min){
						m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy] = 0; 
						}
					else if (m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy] > window_max){
						 m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy] = 255;
						 
						 }
					else {
						m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy] = (T)(255 * ((m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy] - (float)window_min) / (window_max - window_min)));
						} 
				}
			}
		}
	}
	
// Change  the dynamique of image this from [min_value, max_value] to [ 0 , max_value]
	void Turn_Positive(int min_value, int max_value){
	
	for (int z=0; z<m_nDimz ; ++z){
			for (int y=0; y<m_nDimy; ++y){
				for (int x=0; x<m_nDimx; ++x){

					m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy] = (T)(max_value * ((m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy] - (float) min_value) / (max_value - min_value)));
					 
				}
			}
		}
	}
	
	
	
	private :
		int m_nBorder;
		int m_nDimx;
		int m_nDimy;
		int m_nDimz;
		int m_nImageSize;
		std::vector<T>m_vImage;
	
};

#endif // IMAGE_INCLUDED

