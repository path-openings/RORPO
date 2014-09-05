#include <iostream>
#include <omp.h>
#include <vector>

#include "min_modif.hpp"
#include "PO.hpp"

#define OMP


template<typename T>
void Get_RPO_Orientations(T *input_buffer, T *image_dilat, int L, int dimz, int dimy, int dimx, T *res1, T *res2, T *res3, T *res4, T *res5, T *res6, T *res7,int nb_core){

// ####################  Run PO for each orientation ##############################

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
	
	// ####################### Add border of 2 pixels which value is 0 to input_buffer #############################
	std::vector<T> Image((dimz+4)*(dimy+4)*(dimx+4),0);
	int new_dimy=dimy+4;
	int new_dimx=dimx+4;
	int new_dimz=dimz+4;
	for (int z=0; z<dimz; ++z){
		for (int y=0; y<dimy; ++y){
			for (int x=0; x<dimx; ++x){
				Image[(z+2)*(new_dimx*new_dimy)+(y+2)*new_dimx+(x+2)]=image_dilat[z*dimx*dimy+y*dimx+x];
				
			}
		}
	}
	
	std::cout<<"------- RPO computation with scale " <<L<< "-------"<<std::endl;
	// Calling PO for each orientation
	   omp_set_num_threads(nb_core);
	
	#ifdef OMP
	#pragma omp parallel shared(Image)
	{
		#pragma omp sections nowait
		{	
			#pragma omp section
			{
			#endif //OMP
			
			 PO_3D<T>(Image,new_dimz,new_dimy,new_dimx,L,orientation1,res1);
			 std::cout<<"orientation1 1 0 0 : passed"<<std::endl;
			#ifdef OMP
			}
			
			#pragma omp section
			{
				#endif //OMP
			    PO_3D<T>(Image,new_dimz,new_dimy,new_dimx,L,orientation2,res2);
			    std::cout<<"orientation2 0 1 0 : passed"<<std::endl;
			#ifdef OMP
			}
			#pragma omp section
			{
				#endif
			    PO_3D<T>(Image,new_dimz,new_dimy,new_dimx,L,orientation3,res3);
			    std::cout<<"orientation3 0 0 1 : passed"<<std::endl;
			
			#ifdef OMP
			}
			#pragma omp section
			{
			 	#endif
			    PO_3D<T>(Image,new_dimz,new_dimy,new_dimx,L,orientation4,res4);
			    std::cout<<"orientation4 1 1 1 : passed"<<std::endl;
			
			#ifdef OMP
			}
			#pragma omp section
			{
				#endif
			    PO_3D<T>(Image,new_dimz,new_dimy,new_dimx,L,orientation5,res5);
			    std::cout<<"orientation5 1 1 -1 : passed"<<std::endl;
			
			#ifdef OMP
			}
			#pragma omp section
			{
				#endif
			    PO_3D<T>(Image,new_dimz,new_dimy,new_dimx,L,orientation6,res6);
			    std::cout<<"orientation6 -1 1 1 : passed"<<std::endl;
			
			#ifdef OMP
			}
			#pragma omp section
			{
				#endif
			    PO_3D<T>(Image,new_dimz,new_dimy,new_dimx,L,orientation7,res7);
			    std::cout<<"orientation7 -1 1 -1 : passed"<<std::endl;
			
			#ifdef OMP
			}
		}
	}
	#endif
	 std::cout<<"RPO computation completed"<<std::endl;
	
	// Minimum between the computed RPO on the dilation and the initial image
	min_crush(res1, input_buffer, dimx, dimy, dimz);
	min_crush(res2, input_buffer, dimx, dimy, dimz);
	min_crush(res3, input_buffer, dimx, dimy, dimz);
	min_crush(res4, input_buffer, dimx, dimy, dimz);
	min_crush(res5, input_buffer, dimx, dimy, dimz);
	min_crush(res6, input_buffer, dimx, dimy, dimz);
	min_crush(res7, input_buffer, dimx, dimy, dimz);
		
}
		
		
