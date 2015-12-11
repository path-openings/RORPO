#ifndef SORTING_HPP_INCLUDED
#define SORTING_HPP_INCLUDED

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

#include <algorithm>
#include "Image/Image.hpp"

template<typename PixelType>
static inline void sort7_sorting_network_simple_swap(PixelType ** d){

#define mini(x, y) (x<y?x:y)
#define maxi(x, y) (x<y?y:x)
//#define SWAP(x,y) { const int a = min(d[x], d[y]);
//  const int b = max(d[x], d[y]); d[x] = a; d[y] = b;}
#define SWAP(x,y) { const int a = mini(*d[x], *d[y]); const int b = maxi(*d[x], *d[y]); *d[x] = a; *d[y] = b;}


    SWAP(1, 2);
    SWAP(3, 4);
    SWAP(5, 6);
    SWAP(0, 2);
    SWAP(4, 6);
    SWAP(3, 5);
    SWAP(2, 6);
    SWAP(1, 5);
    SWAP(0, 4);
    SWAP(2, 5);
    SWAP(0, 3);
    SWAP(2, 4);
    SWAP(1, 3);
    SWAP(0, 1);
    SWAP(2, 3);
    SWAP(4, 5);

#undef SWAP
#undef min
#undef max
}


template<typename PixelType>
static void sorting(Image3D<PixelType> &image1, Image3D<PixelType> &I2,
                    Image3D<PixelType> &I3, Image3D<PixelType> &I4,
                    Image3D<PixelType> &I5, Image3D<PixelType> &I6,
                    Image3D<PixelType> &I7, int N)
{
    PixelType* d[7];
    d[0] = image1.get_pointer();
    d[1] = I2.get_pointer();
    d[2] = I3.get_pointer();
    d[3] = I4.get_pointer();
    d[4] = I5.get_pointer();
    d[5] = I6.get_pointer();
    d[6] = I7.get_pointer();

    for (int i=0; i<N; i++) {
        sort7_sorting_network_simple_swap(d);
        for (int j=0;j<7;j++)
            d[j]++;
    }
}

#ifdef _TEST_SORT_
int main(int argc, char **argv) {
   for (int i=0; i<10; i++){
        PixelType d[7], *b[7];
        for (int j=0;j<7; j++) {d[j]=rand()%256;b[j]=d+j;}
        sort7_sorting_network_simple_swap(b);
        for (int j=0; j<7; j++) std::cout<< d[j] << "--" ;
        std::cout << std::endl;
    }
    int i;
    std::cin >> i;
}
#endif // _TEST_SORT_

template<typename PixelType>
bool my_sorting_function (const PixelType *i,const PixelType *j)
// Input: i, j : two variables containing memory adress pointing to
// PixelType variables.

// Return : True if the variable pointed by i is smaller than the
// variabled pointed by j
{
	return (*i<*j);
}

template<typename PixelType,typename IndexType>
std::vector<IndexType> sort_image_value(PixelType *image, int size)
//  Return pixels index of image sorted according to intensity
{
	std::vector<IndexType> index_image(size);
	std::vector<PixelType*>index_pointer_adress(size);
	IndexType it;
	typename std::vector<PixelType>::iterator it1;
	typename std::vector<PixelType*>::iterator it2;
	typename std::vector<IndexType>::iterator it3;

    // Fill index_pointer_adress with memory adress of variables in image
    for (it = 0, it2 = index_pointer_adress.begin(); it != size; ++it, ++it2)
	{
        *it2 = &image[it];
	}

	// Sorting adresses according to intensity
    std::sort(index_pointer_adress.begin(), index_pointer_adress.end(),
              my_sorting_function<PixelType>);

	// Conversion from adresses to index of image I
    for (it3 = index_image.begin(), it = 0; it != size; ++it, ++it3)
	{
        *it3 = static_cast<IndexType>(index_pointer_adress[it] - &image[0]);
	}
	return index_image;
}

#endif // SORTING_HPP_INCLUDED
