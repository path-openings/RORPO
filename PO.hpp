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
   
*/

#include <iostream>
#include <string>
#include <omp.h>
#include <vector>
#include <queue>
#include <algorithm>
#include <iterator>
#include <cassert>

#include "rect3dmm.hpp"
#include "sorting.hpp"
#include "Image.hpp"

typedef long IndexType;


void createNeighbourhood(	int nb_col,
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
    //from up to down main orientation [0 1 0]
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
    //from left to right main orientation [0 0 1]
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
    //1st main diagonal [1 1 1]
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
    //2nd main diagonal [1 1 -1]
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
    //3 main diagonal [-1 1 1]
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
    //4 main diagonal [-1 1 -1]
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
void propagate(IndexType p, std::vector<int>&lambda, std::vector<int>&nf, std::vector<int>&nb, std::vector<bool>&b, std::queue<IndexType> &Qc)
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

// Compute PO with an image which already has a 2-pixel border (good to compute the 7 orientation in parallel)
template<typename T>
void PO_3D(	Image<T> &I,
			int L,
			std::vector<IndexType> &index_image,
			std::vector<int> &orientations,
			Image<T> &Output)
			
//Path opening with orientation
{
	
	int new_dimz=I.Dimz();
	int new_dimy=I.Dimy();
	int new_dimx=I.Dimx();
	
	// Create the temporary image b  (0 for a 1-pixel border, 1 elsewhere)
	std::vector<bool>b(I.ImageSize(),1);
	
	// z=0
	for (int y = 0; y < I.Dimy() ; ++y){
		for (int x = 0 ; x < I.Dimx() ; ++x){
			b[y*new_dimx+x]=0;
		}
	}
	
	//z=dimz-1
	for (int y = 0; y < I.Dimy() ; ++y){
		for (int x = 0 ; x < I.Dimx() ; ++x){
			b[(new_dimz-1)*new_dimx*new_dimy+y*new_dimx+x]=0;
		}
	}
	
	//x=0
	for (int z = 0 ; z < I.Dimz() ; ++z){
		for (int y = 0 ; y < I.Dimy() ; ++y){
			b[z*new_dimx*new_dimy+y*new_dimx]=0;
		}
	}
	
	//x=dimx-1
	for (int z = 0 ; z < I.Dimz() ; ++z){
		for (int y = 0 ; y < I.Dimy() ; ++y){
			b[z*new_dimx*new_dimy+y*new_dimx+new_dimx-1]=0;
		}
	}
	
	// y=0
	for (int z = 0 ; z < I.Dimz(); ++z){
		for (int x = 0 ; x < I.Dimx() ; ++x){
			b[z*new_dimy*new_dimx+x]=0;
		}
	}
	
	// y=dimy-1
	for (int z = 0 ; z < I.Dimz() ; ++z){
		for (int x = 0 ; x < I.Dimx() ; ++x){
			b[z*new_dimy*new_dimx+(new_dimy-1)*new_dimx+x]=0;
		}
	}
	
	// Create the offset np and nm
	std::vector<int>np;
	std::vector<int>nm;
	createNeighbourhood(I.Dimx(), I.Dimx() * I.Dimy(), orientations, np, nm);

	//Create other temporary images
	std::vector<int>Lp(I.ImageSize(),L);
	std::vector<int>Lm(I.ImageSize(),L);


	//Create FIFO queue Qc
	std::queue<IndexType> Qc;

	// Propagate
	std::vector<IndexType>::iterator it;
	//std::cerr<<"Avant propagation"<<std::endl;
	//std::cerr<<"size np et nm:"<<np.size()<<" "<<nm.size()<<std::endl;
	
	//std::vector<unsigned char>b_bis=b.GetData();

	int indice;
	for (it = index_image.begin(), indice = 0 ; it != index_image.end() ; ++it , ++indice)
	{

		//std::cerr<<"propagation"<<std::endl;
		if (b[*it])
		{
			//std::cout<<(float(indice)/index_image.size())*100<<std::endl;

			
			propagate<T>(*it, Lm, np, nm, b, Qc);
			propagate<T>(*it, Lp, nm, np, b, Qc);

			while (not Qc.empty())
			{
				IndexType q = Qc.front();
				Qc.pop();
				if (Lp[q] + Lm[q]-1 < L)
				{
					//std::cout <<"Image["<<q<< "]= "<< Image[*it]<< std::endl;
					Output.GetData()[q] = Output.GetData()[*it];
					b[q] = 0;
					Lp[q] = 0;
					Lm[q] = 0;
				}
			}
		}
	}

	//Outputbuffer=&Image[0];

	return ;
}

// Compute PO with an image which doesn't have a 2-pixels border. (Stand alone PO)
template<typename T>
void PO_3D_border(	T* Inputbuffer,
								int dimz,
								int dimy,
								int dimx,
								int L,
								std::vector<int>orientations,
								T* Outputbuffer)
//Path opening with orientation
{

	// Add border of 2 pixels which value is 0 to *Inputbuffer
	std::vector<T> Image((dimz+4)*(dimy+4)*(dimx+4),0);
    //std::vector<bool>b(Image.size(),0);
	int new_dimy=dimy+4;
	int new_dimx=dimx+4;
	for (int z=0; z<dimz; ++z){
		for (int y=0; y<dimy; ++y){
			for (int x=0; x<dimx; ++x){
				Image[(z+2)*(new_dimx*new_dimy)+(y+2)*new_dimx+(x+2)]=Inputbuffer[z*dimx*dimy+y*dimx+x];
				//b[(z+2)*(new_dimx*new_dimy)+(y+2)*new_dimx+(x+2)]=1;
			}
		}
	}

	// Create the temporary image b  (0 for a 1-pixel border, 1 elsewhere)
	std::vector<bool>b(Image.size(),0);
	for (int z=0; z<dimz+2; ++z){
		for (int y=0; y<dimy+2; ++y){
			for (int x=0; x<dimx+2; ++x)
			{
				b[(z+1)*(new_dimx*new_dimy)+(y+1)*new_dimx+(x+1)]=1;
			}
		}
	}

	int dim_frame=new_dimx*new_dimy;

	// Create the sorted list of index of Image according to intensity
	std::vector<IndexType>index_image;
	index_image=sort_image_value<T>(Image);

	// Create the offset np and nm
	std::vector<int>np;
	std::vector<int>nm;
	createNeighbourhood(new_dimx, dim_frame,orientations,np,nm);

	//Create other temporary images
	std::vector<int>Lp(Image.size(),L);
	std::vector<int>Lm(Image.size(),L);

	//Create FIFO queue Qc
	std::queue<IndexType> Qc;

	// Propagate
	std::vector<IndexType>::iterator it;
	//std::cerr<<"Avant propagation"<<std::endl;
	//std::cerr<<"size np et nm:"<<np.size()<<" "<<nm.size()<<std::endl;

	int indice;
	for (it=index_image.begin(), indice=0; it!=index_image.end(); ++it, ++indice)
	{

		//std::cerr<<"propagation"<<std::endl;
		if (b[*it])
		{
			propagate<T>(*it,Lm,np,nm,b,Qc);
			propagate<T>(*it,Lp,nm,np,b,Qc);



			while (not Qc.empty())
			{
				IndexType q=Qc.front();
				Qc.pop();
				if (Lp[q]+Lm[q]-1<L)
				{
					//std::cout <<"Image["<<q<< "]= "<< Image[*it]<< std::endl;
					Image[q]=Image[*it];
					b[q]=0;
					Lp[q]=0;
					Lm[q]=0;
				}
			}
		}
	}

	//Outputbuffer=&Image[0];

	// Remove border
	for (int z=0; z<dimz; ++z){
		for (int y=0; y<dimy; ++y){
			for (int x=0; x<dimx; ++x){
				Outputbuffer[z*dimx*dimy+y*dimx+x]=Image[(z+2)*(new_dimx*new_dimy)+(y+2)*new_dimx+(x+2)];
			}
		}
	}
	return ;
}


#endif // PO_INCLUDED








