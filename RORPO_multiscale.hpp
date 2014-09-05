#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "rect3dmm.hpp"
#include "RORPO.hpp"



template<typename T>
T* RORPO_multiscale(T* image_modif, std::vector<int>S_list, int nb_scales, int nb_core, int max_value, int dimx, int dimy, int dimz, int debug_flag, char* mask_buffer=NULL)
{

    int image_size=dimx*dimy*dimz;
    
    // ---------------------- Mask Image -----------------------------------
	if (mask_buffer!=NULL) // A mask image is given
	{
   		if (debug_flag){
		std::cout<<"Mask Application"<<std::endl;
		std::cout<<"Dilation of the Mask by a ball of radius "<<S_list[nb_scales-1]/2<<std::endl;
		}
		
		unsigned char* mask_buffer_dilat=new unsigned char[dimx*dimy*dimz];
		memcpy(&mask_buffer_dilat[0],&mask_buffer[0],(dimx*dimy*dimz)*sizeof(char));
		
   		//Dilation of the mask (Compute RPO in a larger zone than the mask to reduce border issues)
		rect3dminmax(mask_buffer_dilat, dimx, dimy, dimz,S_list[nb_scales-1]/2,S_list[nb_scales-1]/2,S_list[nb_scales-1]/2,false);

   		// Application of the dilated mask to input image
		for (int i=0; i<image_size;++i){
			if (mask_buffer_dilat[i]==0){
	       		image_modif[i]=0;
	        }
		}
		delete[](mask_buffer_dilat);
	}
    
	// -------------------------- Dilation input image by a cube of size 3x3x3 ----------------------------
	T *input_buffer=new T[dimx*dimy*dimz];
	memcpy(input_buffer,image_modif,(dimx*dimy*dimz)*sizeof(T));
	rect3dminmax(image_modif, dimx, dimy, dimz, 3,3,3,false);
	

	// ####################### Computation of RORPO for each scale #######################
	T* multiscale=new T[image_size];
	memset(multiscale,0,image_size*sizeof(T));

	std::vector<int>::iterator it;
	T *output_buffer;
	
	for (it=S_list.begin();it!=S_list.end();++it)
	{
	    T *output_buffer=RORPO<T>(input_buffer, image_modif, *it, nb_core, dimx, dimy, dimz);
		// ----------------- Max of scales ---------------
	    max_crush(multiscale,output_buffer,dimx,dimy,dimz); 
	    free(output_buffer);
	  
	}

    // ----------------- Dynamic Enhancement ---------------
	// Find Max value of output_buffer
	int max_output=multiscale[0];

	for (int i=0; i<image_size;++i)
	{
	    if (multiscale[i]>max_output)
	        max_output=multiscale[i];
	}

	std::cout<<"max_output and max_value "<<max_output<<";"<<max_value<<std::endl;
	// Contrast Enhancement
	for (int i=0; i<image_size; ++i)
	{
	    multiscale[i]=static_cast<T>((static_cast<float>(multiscale[i])/static_cast<float>(max_output))*max_value);
	}	
			    
	min_crush(multiscale,input_buffer,dimx,dimy,dimz);
		
		
	if (mask_buffer!=NULL) 
	{
		// Application of the non dilated mask to output
    	for (int i=0; i<image_size;++i){
    		if (mask_buffer[i]==0){
            	multiscale[i]=0;
            }
        }
	}
	
	delete[](input_buffer);
	return multiscale;
}

