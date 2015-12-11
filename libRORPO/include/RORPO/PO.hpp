#ifndef PO_INCLUDED
#define PO_INCLUDED

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


We used the Luengo Hendriks path opening algorithm presented in Fig2 of this paper:
Luengo Hendriks, C.L., "Constrained and Dimensionality-Independent Path Openings,"
in Image Processing, IEEE Transactions on , vol.19, no.6, pp.1587-1595, June 2010
doi: 10.1109/TIP.2010.2044959

*/

#include <iostream>
#include <string>
#include <omp.h>
#include <vector>
#include <queue>
#include <algorithm>
#include <iterator>
#include <cassert>

#include "RORPO/pink/rect3dmm.hpp"
#include "RORPO/sorting.hpp"
#include "Image/Image.hpp"
#include "RORPO/Algo.hpp"

typedef long IndexType;


void create_neighbourhood(int nb_col,
			int dim_frame,
			std::vector<int> & orientation,
			std::vector<int> & upList,
			std::vector<int> & downList) {

    int col_shift = orientation[0];
    int line_shift = orientation[1];
    int depth_shift = orientation[2];

    //depth orientation [1 0 0]
    if((depth_shift == 1 && line_shift == 0 && col_shift == 0) ||
       (depth_shift == -1 && line_shift == 0 && col_shift == 0) ) {
       upList.push_back( dim_frame - nb_col - 1);
       upList.push_back( dim_frame - nb_col + 1);
       upList.push_back( dim_frame + nb_col -1 );
       upList.push_back( dim_frame + nb_col + 1);

       upList.push_back( dim_frame - 1);
       upList.push_back( dim_frame - nb_col);
       upList.push_back( dim_frame + 1);
       upList.push_back( dim_frame + nb_col);
       upList.push_back( dim_frame );

       downList.push_back( -dim_frame + nb_col + 1);
       downList.push_back( -dim_frame + nb_col - 1);
       downList.push_back( -dim_frame - nb_col + 1 );
       downList.push_back( -dim_frame - nb_col - 1);

       downList.push_back( -dim_frame + 1);
       downList.push_back( -dim_frame + nb_col);
       downList.push_back( -dim_frame - 1);
       downList.push_back( -dim_frame - nb_col);
       downList.push_back( -dim_frame );

    }
    //from up to down orientation [0 1 0]
    if((depth_shift == 0 && line_shift == 1 && col_shift == 0) ||
      (depth_shift == 0 && line_shift == -1 && col_shift == 0)) {

       upList.push_back(dim_frame + nb_col - 1 );
       upList.push_back(dim_frame + nb_col + 1);
       upList.push_back( -dim_frame + nb_col -1 );
       upList.push_back( -dim_frame + nb_col + 1);


       upList.push_back(nb_col - 1);
       upList.push_back(dim_frame + nb_col);
       upList.push_back( nb_col + 1);
       upList.push_back( -dim_frame + nb_col);
       upList.push_back( nb_col );

       downList.push_back(-dim_frame - nb_col + 1 );
       downList.push_back(-dim_frame - nb_col - 1);
       downList.push_back( dim_frame - nb_col + 1 );
       downList.push_back( dim_frame - nb_col - 1);

       downList.push_back(-nb_col + 1);
       downList.push_back(-dim_frame - nb_col);
       downList.push_back( -nb_col - 1);
       downList.push_back( dim_frame - nb_col);
       downList.push_back( -nb_col );

    }
    //from left to right orientation [0 0 1]
    if((depth_shift == 0 && line_shift == 0 && col_shift == 1) ||
      (depth_shift == 0 && line_shift == 0 && col_shift == -1)) {

       upList.push_back(-nb_col +1 - dim_frame);
       upList.push_back(-nb_col +1 + dim_frame);
       upList.push_back(nb_col +1 - dim_frame);
       upList.push_back( nb_col +1 + dim_frame);

       upList.push_back(dim_frame + 1);
       upList.push_back(-nb_col + 1);
       upList.push_back(-dim_frame + 1);
       upList.push_back( nb_col + 1);
       upList.push_back( 1 );

       downList.push_back(nb_col -1 + dim_frame);
       downList.push_back(nb_col -1 - dim_frame);
       downList.push_back(-nb_col -1 + dim_frame);
       downList.push_back( -nb_col -1 - dim_frame);

       downList.push_back(-dim_frame - 1);
       downList.push_back(nb_col - 1);
       downList.push_back(dim_frame - 1);
       downList.push_back( -nb_col - 1);
       downList.push_back( -1 );

    }
    //1st diagonal [1 1 1]
    if((depth_shift == 1 && line_shift == 1 && col_shift == 1) ||
      (depth_shift == -1 && line_shift == -1 && col_shift == -1)) {

	   upList.push_back(1);
	   upList.push_back(dim_frame);
	   upList.push_back(nb_col);

	   upList.push_back(dim_frame + nb_col);
       upList.push_back(nb_col + 1);
       upList.push_back(dim_frame + 1 );
       //main direction
       upList.push_back(dim_frame + nb_col + 1);

	   downList.push_back(-1);
	   downList.push_back(-dim_frame);
	   downList.push_back(-nb_col);

       downList.push_back( -dim_frame - nb_col);
       downList.push_back( -nb_col - 1);
       downList.push_back( -dim_frame - 1 );

       //main direction
       downList.push_back(-dim_frame - nb_col - 1);

    }
    //2nd diagonal [1 1 -1]
    if((depth_shift == 1 && line_shift == 1 && col_shift == -1) ||
      (depth_shift == -1 && line_shift == -1 && col_shift == 1)) {

	   upList.push_back(-1);
	   upList.push_back(dim_frame);
	   upList.push_back(nb_col);

       upList.push_back(dim_frame + nb_col);
       upList.push_back(nb_col - 1);
       upList.push_back(dim_frame - 1);

       //main direction
       upList.push_back( dim_frame + nb_col - 1 );

       downList.push_back( 1 );
	   downList.push_back( -dim_frame  );
	   downList.push_back(-nb_col);

	   downList.push_back(-dim_frame - nb_col);
       downList.push_back(-nb_col +1);
       downList.push_back(-dim_frame + 1 );

       //main direction
       downList.push_back( -dim_frame - nb_col + 1 );

    }
    //3rd diagonal [-1 1 1]
    if((depth_shift == -1 && line_shift == 1 && col_shift == 1) ||
      (depth_shift == 1 && line_shift == -1 && col_shift == -1)) {

        upList.push_back( 1 );
		upList.push_back( -dim_frame );
        upList.push_back(nb_col);

		upList.push_back(-dim_frame + nb_col);
        upList.push_back( nb_col + 1);
        upList.push_back( -dim_frame + 1 );

        //main direction
        upList.push_back( -dim_frame + nb_col + 1 );

		downList.push_back( -1 );
		downList.push_back( dim_frame );
		downList.push_back(-nb_col);

        downList.push_back(dim_frame - nb_col);
        downList.push_back( -nb_col -1);
        downList.push_back(  dim_frame - 1 );

        //main direction
        downList.push_back( dim_frame - nb_col -1 );

    }
    //4th diagonal [-1 1 -1]
    if((depth_shift == -1 && line_shift == 1 && col_shift == -1) ||
      (depth_shift == 1 && line_shift == -1 && col_shift == 1)) {

		upList.push_back( -1 );
		upList.push_back( -dim_frame );
		upList.push_back(nb_col);

        upList.push_back(-dim_frame + nb_col);
        upList.push_back( nb_col - 1);
        upList.push_back( -dim_frame - 1 );

        //main direction
        upList.push_back( -dim_frame + nb_col - 1 );

		downList.push_back( 1 );
		downList.push_back( dim_frame );
		downList.push_back(-nb_col);

        downList.push_back(dim_frame - nb_col);
        downList.push_back( -nb_col + 1);
        downList.push_back(  dim_frame + 1 );

        //main direction
        downList.push_back( dim_frame - nb_col + 1 );

    }

}


template<typename PixelType>
void propagate(IndexType p, std::vector<int>&lambda, std::vector<int>&nf,
               std::vector<int>&nb, std::vector<bool>&b,
               std::queue<IndexType> &Qc)

// Propagation from pixel p
{
	std::queue<IndexType> Qq;
	lambda[p]=0;

	std::vector<int>::iterator it;
	for (it=nf.begin(); it!=nf.end();++it)
	{
		if ((p+*it)<lambda.size() and b[p+*it])
		{
			Qq.push(p+*it);
		}
	}

	while (not Qq.empty())
	{
		IndexType q=Qq.front();
		Qq.pop();
		int l=0;
		for (it=nb.begin(); it!=nb.end();++it)
		{
			l=std::max(lambda[q+*it],l);
		}
		l+=1;

		if (l<lambda[q])
		{
			lambda[q]=l;
			Qc.push(q);
			for (it=nf.begin(); it!=nf.end(); ++it)
			{
				if (b[q+*it])
				{
					Qq.push(q+*it);
				}
			}
		}
	}
}

template<typename T, typename MaskType>
void PO_3D(Image3D<T> &image,
		int L,
		std::vector<IndexType> &index_image,
		std::vector<int> &orientations,
		Image3D<T> &Output,
		std::vector<bool> b)
			
{
	
	// Create the offset np and nm
	std::vector<int>np;
	std::vector<int>nm;
    create_neighbourhood(image.Dimx(), image.Dimx() * image.Dimy(),
                         orientations, np, nm);

	//Create other temporary images
    std::vector<int>Lp(image.image_size(), L);
    std::vector<int>Lm(image.image_size(), L);

	//Create FIFO queue Qc
	std::queue<IndexType> Qc;
	
	// Propagate
	std::vector<IndexType>::iterator it;
	int indice;
    for (it = index_image.begin(), indice = 0 ; it != index_image.end() ;
         ++it , ++indice)
	{
		if (b[*it])
		{
			propagate<T>(*it, Lm, np, nm, b, Qc);
			propagate<T>(*it, Lp, nm, np, b, Qc);

			while (not Qc.empty())
			{
				IndexType q = Qc.front();
				Qc.pop();
				if (Lp[q] + Lm[q]-1 < L)
				{
					Output.get_data()[q] = Output.get_data()[*it];
					b[q] = 0;
					Lp[q] = 0;
					Lm[q] = 0;
				}
			}
		}
	}

	return ;
}


#endif // PO_INCLUDED








