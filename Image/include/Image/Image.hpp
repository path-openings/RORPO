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



// ###################################################################################################################
// ############################################### 2D IMAGE ##########################################################
// ###################################################################################################################

template<typename T>
class Image2D {

    public :

        Image2D():
            m_nImageSize(0){}

        Image2D(int dimX, int dimY, int value=0):
             m_nDimx(dimX), m_nDimy(dimY), m_nImageSize(dimX*dimY), m_vImage(dimX*dimY, value){}

        ~Image2D(){}

        T& operator ()(int x, int y) {
            return m_vImage[x + y * m_nDimx];
        }
        const T& operator ()(int x, int y) const {
            return m_vImage[x + y * m_nDimx];
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

        const int image_size() const {
            return m_nImageSize;
        }

        bool empty() const {
            return m_nImageSize == 0;
        }

        std::vector<T>& get_data(){
            return m_vImage;
        }
        const std::vector<T>& get_data() const {
            return m_vImage;
        }

        const T* get_pointer() const {
            return m_vImage.data();
        }
        T* get_pointer(){
            return m_vImage.data();
        }

        // Fill the image with data pointed by p_Pointer
        void add_data_from_pointer(T* p_Pointer){
            for (int y=0; y<m_nDimy; ++y){
                for (int x=0; x<m_nDimx; ++x){
                    m_vImage[x + y * m_nDimx]=p_Pointer[x + y * m_nDimx];
                }
            }
        }

        void clear_image(){
            m_vImage.clear();
            std::vector<T>().swap(m_vImage);
            m_nImageSize=0;

        }


    private :
        int m_nDimx;
        int m_nDimy;
        int m_nImageSize;
        std::vector<T>m_vImage;

};


// ###################################################################################################################
// ############################################### 3D IMAGE ##########################################################
// ###################################################################################################################

template<typename T>
class Image3D {
	
	public : 
	
        Image3D():
			m_nImageSize(0){}
	
        Image3D(int dimX, int dimY, int dimZ, T value=0):
             m_nDimx(dimX), m_nDimy(dimY), m_nDimz(dimZ), m_nImageSize(dimX*dimY*dimZ), m_vImage(dimX*dimY*dimZ, value){}
		
        ~Image3D(){}

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
		
		const int image_size() const {
			return m_nImageSize;
		}
		
        bool empty() const {
            return m_nImageSize == 0;
        }

	std::vector<T>& get_data(){
		return m_vImage;
	}	
	const std::vector<T>& get_data() const {
		return m_vImage;
	}	
	
	const T* get_pointer() const {
		return m_vImage.data();
	}
	T* get_pointer(){
		return m_vImage.data();
	}

	// Fill the image with data pointed by p_Pointer
	void add_data_from_pointer(T* p_Pointer){

		for (int z=0; z<m_nDimz ; ++z){
			for (int y=0; y<m_nDimy; ++y){
				for (int x=0; x<m_nDimx; ++x){
			
					m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy]=p_Pointer[x + y * m_nDimx + z * m_nDimx * m_nDimy];
				}		
			}
		}
	}
	
	void clear_image(){
		m_vImage.clear();
		std::vector<T>().swap(m_vImage);
		m_nImageSize=0;
			
	}	
	
		// Return a new image "bordered_image" which is the self image with a "border"-pixel border
        Image3D<T> add_border(int border, int value=0) const {
	
            Image3D<T> bordered_image(m_nDimx + 2 * border, m_nDimy + 2 * border, m_nDimz + 2 * border, value);
			for (int z = 0; z < m_nDimz ; ++z){
				for (int y = 0 ; y < m_nDimy ; ++y){
					for (int x = 0 ; x < m_nDimx ; ++x){
				
						bordered_image(x + border, y + border, z + border)=m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy];
					}		
				}
			}
			return bordered_image;
		}

        // Return a new image with a border of "border" pixels fill with a mirror border condition.
        Image3D<T> mirror_image(Image3D<float>& image, int border)
        {
	    std::cout<<"miror foireux"<<std::endl;
            Image3D<T> mirror = image.add_border(border);

            for (int k = 0 ; k <border ; ++k)
            {
                for (int j = 0 ; j < border ; ++j)
                {
                    for (int i = 0 ; i < border ; ++i)
                    {
                        mirror(i,j,k) = mirror(2*border-i , 2*border-j , 2*border-k);
                    }
                }
            }

            return mirror;
        }
		
		// Return a new image "bordered_image" which is the self image with a "border"-pixel border
		void remove_border(int border) {
			
			int ind=0;
			for (int z = border; z<m_nDimz - border ; ++z){
				for (int y = border; y<m_nDimy - border; ++y){
					for (int x = border; x<m_nDimx - border; ++x){
						m_vImage[ind]=m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy];
						ind++;
					}		
				}
			}
			m_nDimx = m_nDimx - 2 * border;
			m_nDimy = m_nDimy - 2 * border;
			m_nDimz = m_nDimz - 2 * border;
			m_nImageSize = m_nDimx * m_nDimy * m_nDimz;
			m_vImage.resize(m_nImageSize);
		}
	
	// return a new image which is the copy of this 
    const Image3D<unsigned char> copy_image_2_uchar() const {
		
        Image3D<unsigned char> copy(m_nDimx , m_nDimy , m_nDimz);
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
    const Image3D copy_image() const {
		
        Image3D copy(m_nDimx , m_nDimy , m_nDimz);
		copy.get_data() = m_vImage;
		return copy;
	}
	
	// Copy I into this
    void copy_image(Image3D &I) {
		if (I.image_size() != m_nImageSize)
		{
			m_vImage.resize(I.image_size());
			m_vImage=I.get_data();
			m_nDimx = I.Dimx();		
			m_nDimy = I.Dimy();
			m_nDimz = I.Dimz();
			m_nImageSize = I.image_size();
		}
		else {
			m_vImage=I.get_data();
		}

	}
	
	
	// Return the maximum value of this
	int min_value(){
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
	int max_value(){
		T max = m_vImage[0];
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
    std::vector<T> min_max_value(){
        std::vector<T> minmax(2);
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
	
    // Change the dynamique of image, from [window_min, window_max] to [0, 255]. Intensities smaller than window_min are set to 0 and larger than window_max are set to 255
    void window_dynamic(T window_min, T window_max){
	
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
	void turn_positive(int min_value, int max_value){
	
	for (int z=0; z<m_nDimz ; ++z){
			for (int y=0; y<m_nDimy; ++y){
				for (int x=0; x<m_nDimx; ++x){

					m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy] = (T)(max_value * ((m_vImage[x + y * m_nDimx + z * m_nDimx * m_nDimy] - (float) min_value) / (max_value - min_value)));
					 
				}
			}
		}
	}
	
	
	
	private :
		int m_nDimx;
		int m_nDimy;
		int m_nDimz;
		int m_nImageSize;
		std::vector<T>m_vImage;
	
};

template<typename T1, typename T2>
void operator +(Image3D<T1> &image, T2 scalar){
    for (int z = 0; z < image.Dimz(); ++z){
        for (int y = 0; y < image.Dimy(); ++y){
            for (int x = 0; x < image.Dimx(); ++x){
               image(x,y,z) += scalar;
            }
        }
    }

}

template<typename T1, typename T2 >
void operator -(Image3D<T1> &image, T2 scalar){
    for (int z = 0; z < image.Dimz(); ++z){
        for (int y = 0; y < image.Dimy(); ++y){
            for (int x = 0; x < image.Dimx(); ++x){
               image(x,y,z) -= scalar;
            }
        }
    }

}

#endif // IMAGE_INCLUDED

