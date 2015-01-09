#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <getopt.h>
#include <typeinfo>

#include "Image.hpp"
#include "RORPO_multiscale.hpp"

typedef unsigned char FastPixelType;

extern "C" {
	#include "nifti1_io.h"
}

void usage()
{
	std::cout<<"RORPO parameters :"<<std::endl;
	std::cout<<"--input (-i) : path to the .nii image. The image type can be uint8, short, int32 or float."<<std::endl;
	std::cout<<"--output (-o) : path to write the resulting image."<<std::endl;
	std::cout<<"--smin (-s) : Minimum path length (int)"<<std::endl;
	std::cout<<"--factor (-f) : factor for the geometric sequence of scales; scale_(n+1)=factor*scale_(n) (float)"<<std::endl;
	std::cout<<"--nb_scales (-n) : Number of scales (int)"<<std::endl;
	
	std::cout<<"--window (-w): (optional) Min and max values of the new intensity range of input image (2 int) \n"
	<< "     Convert input image to uint8. Intensities inferior to window_min become 0, intensities superior to window_max become 255; Linear transformation between window_min and window_max"<<std::endl;
	std::cout<<"--core (-c): (optional) nb of CPUs used for RPO computation (int)"<<std::endl;
	std::cout<<"--mask or -m : (optional) path to a mask for the input image (0 for the background and 1 for the foreground). The image type must be uint8."<<std::endl;
	std::cout<<"--debug : (optional) Activation of a debug mode."<<std::endl;
	std::cout<<""<<std::endl;
	std::cout<<"Usage Exemple : ./RORPO_multiscale --debug --input input_image.nii -output result.nii -s 30 -f 1.5 -n 2 --window 100 3250"<<std::endl;
}

template<typename PixelType>
void Call_RORPO(nifti_image* nim, std::string writing_path, std::vector<int> S_list,  int window_min, int window_max, int nb_core, int debug_flag, std::string mask_path="NULL"){
	
	int dimz=nim->nz;
	int dimy=nim->ny;
	int dimx=nim->nx;
	
	Image<PixelType> I(dimx, dimy, dimz);
	I.Add_data_from_pointer((PixelType*)(nim->data));
	nifti_image_free(nim);
	
	if (debug_flag){
		std::cout<<""<<std::endl;
		std::cout << "------ INPUT IMAGE -------" << std::endl;
		std::cout<<"NIFTI Image"<<std::endl;
		std::cout<<"dimx="<<dimx<<" ; dimy="<<dimy<<" ; dimz="<<dimz<<std::endl;
	}

	// ---------- Find min and max value --------

	std::vector<int>minmax = I.MinMax_value();
	int min_value = minmax[0];
	int max_value = minmax[1];

	if (debug_flag){
		std::cout<<"Min intensity value : "<<min_value<<std::endl;
		std::cout<<"Max intensity value : "<<max_value<<std::endl;
		std::cout<<" "<<std::endl;
	}

	// ---------------------- Mask Image -----------------------------------
		
	Image<unsigned char> Mask(I.Dimx(), I.Dimy(), I.Dimz());

	if (mask_path!="NULL") // A mask image is given
	{
		
		// Reading the mask image
		nifti_image *nim_mask=NULL;
		nim_mask = nifti_image_read(mask_path.c_str(), 1);
		int dimz=nim_mask->nz;
		int dimy=nim_mask->ny;
		int dimx=nim_mask->nx;
		
		if (dimx==I.Dimx() and dimy==I.Dimy() and dimz==I.Dimz()){
			Mask.Add_data_from_pointer((unsigned char*)(nim_mask->data));
			//nifti_image_free(nim_mask);
		}
		else {
			std::cerr<<"Error, size of the mask image is not the same as the size of the input image"<<std::endl;
			std::cerr<<"Mask image: "<<dimx<<"x"<<dimy<<"x"<<dimz<<std::endl;
			std::cerr<<"Input image : "<<I.Dimx()<<"x"<<I.Dimy()<<"x"<<I.Dimz()<<std::endl;
		}
	}
	else // No mask is given
	{
		Mask.ClearImage();
	}
	

	// ############################# WINDOW IMAGE and CHAR CONVERSION  ##############################

	// --------------- Convert the InputType input image to FastPixelType image and rescale it between window_min and window_max -----------
	if (window_min!=-1 and window_max!=-1)
	{
		if (min_value>window_min)
			window_min=min_value;

		if (max_value<window_max)
			window_max=max_value; 

		if(debug_flag){
			std::cout<<"Dynamic Changement"<<std::endl;
			std::cout<<"New image intensity range :"<<std::endl;
			std::cout<<"["<<window_min << " ; " << window_max<<"] converted in char [0,255]"<<std::endl;
			std::cout<<""<<std::endl;
		}

		I.Window_Dynamic(window_min, window_max);
		Image<FastPixelType> Image_Char=I.Copy_image_2_uchar();
		I.ClearImage();
		
		minmax = Image_Char.MinMax_value();
		min_value = minmax[0];
		max_value = minmax[1];

		if (debug_flag){
			std::cout<<"Image_Char : "<<std::endl;
			std::cout<<"Min intensity value : "<<min_value<<std::endl;
			std::cout<<"Max intensity value : "<<max_value<<std::endl;
			std::cout<<" "<<std::endl;
		}

		// Run RORPO
		Image<FastPixelType> Multiscale=RORPO_multiscale<FastPixelType,unsigned char>(Image_Char, S_list, nb_core, debug_flag, Mask);	
		
		minmax = Multiscale.MinMax_value();
		min_value = minmax[0];
		max_value = minmax[1];

		if (debug_flag){
			std::cout<<"Multiscale: "<<std::endl;
			std::cout<<"Min intensity value : "<<min_value<<std::endl;
			std::cout<<"Max intensity value : "<<max_value<<std::endl;
			std::cout<<" "<<std::endl;
		}

		// Write the result in nifti
		int t[8] = {3, Multiscale.Dimx(), Multiscale.Dimy(), Multiscale.Dimz(), 0,0,0,0};
		nifti_image *nim_write = nifti_make_new_nim(t, 2, 1);
		nim_write->data = Multiscale.GetPointer();
		int test=nifti_set_filenames(nim_write, writing_path.c_str(), 0, nim_write->byteorder);
		nifti_image_write(nim_write);
		//nifti_image_free(nim_write);
	
	}
	
	// ############################# CONVERSION from FLOAT to CHAR ##############################
	
	else if (typeid(PixelType)==typeid(float))
	{
			
		if(debug_flag){
			std::cout<<"Conversion from float to unsigned char"<<std::endl;
			std::cout<<"New dynamic range : [0 ; 255]"<<std::endl;
		}
		
		I.Window_Dynamic(min_value, max_value);
		Image<FastPixelType> Image_Char=I.Copy_image_2_uchar();
		I.ClearImage();
		
		// Run RORPO
		Image<FastPixelType> Multiscale=RORPO_multiscale<FastPixelType>(Image_Char, S_list, nb_core, debug_flag, Mask);	

		// Write the result in nifti
		int t[8] = {3, Multiscale.Dimx(), Multiscale.Dimy(), Multiscale.Dimz(), 0,0,0,0};
		nifti_image *nim_write = nifti_make_new_nim(t, 2, 1);
		nim_write->data = Multiscale.GetPointer();
		int test=nifti_set_filenames(nim_write, writing_path.c_str(), 0, nim_write->byteorder);
		nifti_image_write(nim_write);
		//nifti_image_free(nim_write);
	}
	
	// ############################# Image intensities non positives ##############################
	
		else if (min_value <0){
	
			if(debug_flag){
				std::cout<<"Minimum image intensity is smaller than 0"<<std::endl;
				std::cout<<"New image intensity range :"<<std::endl;
				std::cout<<"["<< 0 << " ; " << max_value<<std::endl;
			}
			I.Turn_Positive(min_value, max_value);
			
			// Run RORPO
			Image<PixelType> Multiscale=RORPO_multiscale<PixelType>(I,  S_list, nb_core, debug_flag, Mask);	
			
			// Write the result in nifti
			int t[8] = {3, Multiscale.Dimx(), Multiscale.Dimy(), Multiscale.Dimz(), 0,0,0,0};
			nifti_image *nim_write;
			if (typeid(PixelType)==typeid(short)){
				nim_write = nifti_make_new_nim(t, 4, 1);
			}
			if (typeid(PixelType)==typeid(int32_t)){
				nim_write = nifti_make_new_nim(t, 8, 1);
			}
			if (typeid(PixelType)==typeid(float)){
				nim_write = nifti_make_new_nim(t, 16, 1);
			}
			nim_write->data = Multiscale.GetPointer();
			int test=nifti_set_filenames(nim_write, writing_path.c_str(), 0, nim_write->byteorder);
			nifti_image_write(nim_write);
			//nifti_image_free(nim_write);
		}
	
	// ############################# ELSE ##############################

	else {

		// Run RORPO
		Image<PixelType> Multiscale = RORPO_multiscale<PixelType>(I, S_list, nb_core, debug_flag, Mask);		

		
		// Write the result in nifti
		int t[8] = {3, Multiscale.Dimx(), Multiscale.Dimy(), Multiscale.Dimz(), 0,0,0,0};
		nifti_image *nim_write;
		if (typeid(PixelType)==typeid(short)){
			nim_write = nifti_make_new_nim(t, 4, 1);
		}
		if (typeid(PixelType)==typeid(unsigned short)){
			nim_write = nifti_make_new_nim(t, 512, 1);
		}
		if (typeid(PixelType)==typeid(unsigned char)){
			nim_write = nifti_make_new_nim(t, 2, 1);
		}
		if (typeid(PixelType)==typeid(int32_t)){
			nim_write = nifti_make_new_nim(t, 8, 1);
		}
		if (typeid(PixelType)==typeid(float)){
			nim_write = nifti_make_new_nim(t, 16, 1);
		}
		nim_write->data = Multiscale.GetPointer();
		int test=nifti_set_filenames(nim_write, writing_path.c_str(), 0, nim_write->byteorder);
		nifti_image_write(nim_write);
		//nifti_image_free(nim_write);

	}
}




int main(int argc, char **argv)
{

	// ------------------------ Parameters -----------------------------
	std::string mask_path="NULL";
	std::string image_path="-1";
	float Smin=-1;
	float factor=-1;
	int nb_scales=-1;
	float window_min=-1;
	float window_max=-1;
	std::string writing_path="-1";
	int nb_core=1;
	int debug_flag=0;
	int binmask_flag=0;

	std::cout << " " << std::endl;
	std::cout << "------ PARAMETERS -------" << std::endl;
	std::cout << " " << std::endl;
	// Optional arguments
	int a;
 	while (1)
    {
       static struct option long_options[] =
         {
           {"input",required_argument,0,'i'},
           {"output",required_argument,0,'o'},
           {"smin",required_argument,0,'s'},
           {"factor",required_argument,0,'f'},
           {"nb_scales",required_argument,0,'n'},  
           
           //OPTIONAL
           {"debug", no_argument, &debug_flag, 1}, //flag
           {"mask",required_argument,0,'m'},
           {"window",required_argument,0,'w'},
           {"core",required_argument,0,'c'},
           {0, 0, 0, 0}
         };
         
       int option_index = 0;
       a = getopt_long (argc,
       				    argv,
       				    "i:o:s:f:n:dm:w:c:",
       				    long_options,
       				    &option_index);

       /* Detect the end of the options. */
       
       if (a == -1)
         break;

 	   switch (a)
 	   {
 	   	case 0: //flag
            break;

		case 'i':
        	image_path=optarg;
        	std::cout << "image_path= " <<  optarg<<std::endl;
         	break;
         	
		case 'o':
        	writing_path=optarg;
        	std::cout << "writing_path= " << optarg<<std::endl;
         	break;   
         	
		case 's':
        	Smin=atof(optarg);
        	std::cout << "Smin= " << optarg<<std::endl;
         	break;
         	
		case 'f':
        	factor=atof(optarg);
        	std::cout << "factor= " <<optarg<<std::endl;
         	break;      

		case 'n':
        	nb_scales=atoi(optarg);
        	std::cout << "nb_scales= " << optarg<<std::endl;
         	break;        	
     
		case 'm':
        	mask_path=optarg;
        	std::cout << "mask_path= " << optarg<<std::endl;
         	break;

		case 'c':
        	nb_core=atoi(optarg);
        	std::cout << "nb_core " << optarg<<std::endl;
         	break;
         	
         case 'w':
         	window_min=atoi(argv[optind-1]);
         	window_max=atoi(argv[optind]);
         	std::cout << "window min= " << argv[optind-1]<<std::endl;
         	std::cout << "window max= " << argv[optind]<<std::endl;
		}
	}
	
	
	// --------------- Test parameters validity -----------------        	
   if (image_path=="-1")
	{
		std::cout<<" "<<std::endl;
		std::cout<<" ********* Error with image path ********"<<std::endl;
		std::cout<<" "<<std::endl;
		usage();
		return 0;
	}
	else if (writing_path=="-1")
	{
		std::cout<<" "<<std::endl;
		std::cout<<" ********* Error with writing path *********"<<std::endl;
		std::cout<<" "<<std::endl;
		usage();
		return 0;
	}
	
	if (Smin==-1)
	{
		std::cout<<" "<<std::endl;
		std::cout<<" ********* Error with Smin ********"<<std::endl;
		std::cout<<" "<<std::endl;
		usage();
		return 0;
	}
	
	if (factor==-1)
	{
		std::cout<<" "<<std::endl;
		std::cout<<" ********* Error with factor ********"<<std::endl;
		std::cout<<" "<<std::endl;
		usage();
		return 0;
	}
	
	if (nb_scales==-1)
	{
		std::cout<<" "<<std::endl;
		std::cout<<" ********* Error with nb_scales ********"<<std::endl;
		std::cout<<" "<<std::endl;
		usage();
		return 0;
	}
		
	else // Program can be run
	{
         
     	// ################################# Scales Computations ###################################
		
		std::vector<int> S_list(nb_scales);
		S_list[0]=Smin;

		if (debug_flag){
			std::cout<<"Scales : ";
			std::cout<<S_list[0]<< " ";
		}
				
		for (int i=1; i<nb_scales;++i)
		{
			S_list[i]=int(Smin*pow(factor,i));
			if (debug_flag){
				std::cout<<S_list[i]<< " ";
				
			}
		}
		std::cout<<std::endl;
		
		/*for (int i=1; i<nb_scales-1;i++)
		{
		    S_list[i]=Smin+i*((factor-Smin)/(nb_scales-1));
		    std::cout<<S_list[i]<< " ";
		}
		S_list[nb_scales-1]=factor;
		std::cout<<factor<< " ";*/
		
		// #######################################################################################      
		// ################################# Reading Image #######################################
		// #######################################################################################
		
		int image_size;

		// ------------ Reading Nifti Image -------------------
		nifti_image *nim=NULL;
		nim = nifti_image_read(image_path.c_str(), 1);
		switch(nim->datatype){
	
			case 2: {
				if (debug_flag){
					std::cout<<"Input image type : Unsigned Char "<<std::endl;
				}
				Call_RORPO<unsigned char>(nim, writing_path, S_list, window_min, window_max, nb_core, debug_flag, mask_path);
				break;
			}
			case 4: {
				if (debug_flag){
					std::cout<<"Input image type : Short "<<std::endl;
				}
				Call_RORPO<short>(nim, writing_path, S_list, window_min, window_max, nb_core, debug_flag, mask_path);
				break;
			}
			case 8: {
				if (debug_flag){
					std::cout<<"Input image type : Long "<<std::endl;
				}
				Call_RORPO<long>(nim, writing_path,  S_list, window_min, window_max, nb_core, debug_flag, mask_path);
				break;
			}
			case 16: {
				if (debug_flag){
					std::cout<<"Input image type : Float "<<std::endl;		
				}
				Call_RORPO<float>(nim, writing_path, S_list, window_min, window_max, nb_core, debug_flag, mask_path);
				break;
			}
			case 256: {
				if (debug_flag){
					std::cout<<"Input image type : Char "<<std::endl;
				}
				Call_RORPO<char>(nim, writing_path, S_list, window_min, window_max, nb_core, debug_flag, mask_path);
				break;
			}
			case 512: {
				if (debug_flag){
					std::cout<<"Input image type : Unsigned Short "<<std::endl;
				}
				Call_RORPO<unsigned short>(nim, writing_path, S_list, window_min, window_max, nb_core, debug_flag, mask_path);
				break;
			}
			case 768: {
				if (debug_flag){
					std::cout<<"Input image type : Unsigned long "<<std::endl;
				}
				Call_RORPO<unsigned long>(nim, writing_path, S_list, window_min, window_max, nb_core, debug_flag, mask_path);
				break;
			}
			default : {
				std::cout<<"Input image type not supported "<<std::endl;		
				break;
			}
		}
	}
	return 1;
}//end main

