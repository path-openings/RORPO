#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <getopt.h>

/* definition to expand macro then apply to pragma message */
#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var "="  VALUE(var)

//#define HAVE_VTK_H
#ifdef HAVE_VTK_H
//#pragma message VAR_NAME_VALUE(HAVE_VTK_H)
#include <vtkImageData.h>
#include <vtkMetaImageReader.h>
#include <vtkSmartPointer.h>
#include <vtkImageWriter.h>
#include <vtkImageImport.h>
#include <vtkImageExport.h>
#include <vtkMetaImageWriter.h>

#endif // HAVE_VTK_H

#include "rect3dmm.hpp"


typedef short InputType;
typedef unsigned char FastPixelType;

#include "RORPO_multiscale.hpp"

extern "C" {
	#include "nifti1_io.h"
}



void usage()
{
	std::cout<<"RORPO parameters :"<<std::endl;
	std::cout<<"--input (-i) : path to the .mha or .nii image. The image type must be short."<<std::endl;
	std::cout<<"--output (-o) : path to write the resulting image."<<std::endl;
	std::cout<<"--smin (-s) : Minimum path length (int)"<<std::endl;
	std::cout<<"--factor (-f) : factor for the geometric sequence of scales; scale_(n+1)=factor*scale_(n) (float)"<<std::endl;
	std::cout<<"--nb_scales (-n) : Number of scales (int)"<<std::endl;
	std::cout<<"--extension (-e) : 1 for nifti input image; 0 for mha input image (int)."<<std::endl;
	
	std::cout<<"--window (-w): (optional) Min and max values of the new intensity range of input image (2 int) \n"
	<< "     Convert input image to uint8. Intensities inferior to window_min become 0, intensities superior to window_max become 255; Linear transformation between window_min and window_max"<<std::endl;
	std::cout<<"--core (-c): (optional) nb of CPUs used for RPO computation (int)"<<std::endl;
	std::cout<<"--mask or -m : (optional) path to a mask for the input image (0 for the background and 1 for the foreground). The image type must be uint8."<<std::endl;
	std::cout<<"--debug : (optional) Activation of a debug mode."<<std::endl;
	std::cout<<""<<std::endl;
	std::cout<<"Usage Exemple : ./RORPO_multiscale -i input_image.mha -o result.mha -s 30 -f 1.5 -n 2 -e 0 -m 'Mask_image.mha'"<<std::endl;
	std::cout<<"Usage Exemple : ./RORPO_multiscale --debug --input input_image.nii -output result.nii -s 30 -f 1.5 -n 2 -e 1 --window 100 3250"<<std::endl;
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
	int extension=-1;
	int debug_flag=0;
	int binmask_flag=0;

	std::cout << " " << std::endl;
	std::cout << "------ PARAMETERS -------" << std::endl;
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
           {"extension",required_argument,0,'e'},
           
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
       				    "i:o:s:f:n:e:dm:w:c:",
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

		case 'e':
        	extension=atoi(optarg);
        	std::cout << "extension= " << optarg<<std::endl;
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
	
	if (extension==-1)
	{
		std::cout<<" "<<std::endl;
		std::cout<<" ********* Error with extension ********"<<std::endl;
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
		    std::cout<<" "<<std::endl;
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

		InputType* image;
		int dimz;
		int dimy;
		int dimx;
		int image_size;

		// ------------ Reading Nifti Image -------------------
		if (extension==1){
			nifti_image *nim=NULL;
			nim = nifti_image_read(image_path.c_str(), 1);
			//image=((InputType*)(nim->data));
			
			dimz=nim->nz;
			dimy=nim->ny;
			dimx=nim->nx;
			image_size=dimx*dimy*dimz;
			
			image=new InputType[dimx*dimy*dimz];
			memcpy(image,(nim->data),sizeof(InputType)*dimx*dimy*dimz);
			nifti_image_free(nim);
			
			if (debug_flag){
				std::cout<<""<<std::endl;
				std::cout << "------ INPUT IMAGE -------" << std::endl;
				std::cout<<"NIFTI Image"<<std::endl;
				std::cout<<"dimx="<<dimx<<" ; dimy="<<dimy<<" ; dimz="<<dimz<<std::endl;
			}
		}
		 
		// ------------ Reading Mha Image -------------------
		else if (extension==0){

			vtkSmartPointer<vtkMetaImageReader> reader =
				vtkSmartPointer<vtkMetaImageReader>::New();
			reader->SetFileName(image_path.c_str());
			reader->Update();

			//ImageData
			vtkImageData* metaimage = reader->GetOutput();

			//image=malloc((dimx+dimy+dimz)*sizeof(InputType));
			//image=(InputType*)(metaimage->GetScalarPointer());

			

			dimx=metaimage->GetDimensions()[0];
			dimy=metaimage->GetDimensions()[1];
			dimz=metaimage->GetDimensions()[2];
			image_size=dimx*dimy*dimz;			
			
			image=new InputType[dimx*dimy*dimz];
			memcpy(image,(metaimage->GetScalarPointer()),sizeof(InputType)*dimx*dimy*dimz);
			
			
			if (debug_flag){
				std::cout<<""<<std::endl;
				std::cout << "------ INPUT IMAGE -------" << std::endl;
				std::cout<<"MHA Image"<<std::endl;
				std::cout<<"dimx="<<dimx<<" ; dimy="<<dimy<<" ; dimz="<<dimz<<std::endl;
			}	
		}
		
		else {
			std::cerr<<"Error with extension parameter"<<std::endl;
			return 0;
		}
		char* mask_buffer=new char[dimx*dimy*dimz];

		// ---------- Find min and max value --------
		int min_value=image[0];
		int max_value=image[0];
		
		for (int i=0; i<image_size;++i)
		{
			if (window_min==-1 and window_max==-1 and image[i]<0 ){
				std::cerr<<"Image contain negative values"<<std::endl;
				return 0;
			}
			if (image[i]<min_value)
				min_value=image[i];

			if (image[i]>max_value)
				max_value=image[i];
		
		}

		if (debug_flag){
			std::cout<<"min value: "<<min_value<<std::endl;
			std::cout<<"max value: "<<max_value<<std::endl;
			std::cout<<" "<<std::endl;
		}

			// ---------------------- Mask Image -----------------------------------

		if (mask_path!="NULL") // A mask image is given
		{
			if (extension==1){
			// Reading the mask image
			nifti_image *nim_mask=NULL;
			nim_mask = nifti_image_read(mask_path.c_str(), 1);
			memcpy(mask_buffer,(nim_mask->data),sizeof(char)*dimx*dimy*dimz);
			nifti_image_free(nim_mask);
			}
			else{
				vtkSmartPointer<vtkMetaImageReader> readerMask =
				vtkSmartPointer<vtkMetaImageReader>::New();
				readerMask->SetFileName(mask_path.c_str());
				readerMask->Update();

				//ImageData
				vtkImageData* metaimageMask = readerMask->GetOutput();
				
				image=new InputType[dimx*dimy*dimz];
				memcpy(mask_buffer,(metaimageMask->GetScalarPointer()),sizeof(char)*dimx*dimy*dimz);
			}
		}
		else 
		{
			delete[](mask_buffer);
		}

		// ############################# CHAR IMAGE  ##############################

		// --------------- Convert the InputType input image to FastPixelType and rescale it between window_min and window_max -----------
		if (window_min!=-1 and window_max!=-1)
		{
			if (min_value>window_min)
				window_min=min_value;

			if (max_value<window_max)
				window_max=max_value;    

			if(debug_flag){
				std::cout<<"Dynamic Changement"<<std::endl;
				std::cout<<"New image intensity range :"<<std::endl;
				std::cout<<"["<<window_min << " " << window_max<<"] converted in char [0,255]"<<std::endl;
			}

			FastPixelType* image_modif=new FastPixelType[dimx*dimy*dimz];
	
			// Window the dynamic range of the image between window_min and window_max
			for (int i=0; i<image_size;++i)
			{
				if (image[i]<=window_min){
					image_modif[i]=0; 
				   	}
				else if (image[i]>window_max){
					 image_modif[i]=255;
					 
					 }
				else {
					image_modif[i]=255*((image[i]-window_min)/(window_max-window_min));
					}
			}
			max_value=255;
	
			delete[](image);
	
			if(debug_flag){
				std::cout<<""<<std::endl;
			}
	
			// Run RORPO
			FastPixelType* multiscale=new FastPixelType[dimx*dimy*dimz];
	
			if (mask_path!="NULL"){
				multiscale=RORPO_multiscale<FastPixelType>(image_modif, S_list, nb_scales,nb_core, max_value, dimx, dimy, dimz, debug_flag, mask_buffer);	
				delete[](mask_buffer);
			}
			else {
				multiscale=RORPO_multiscale<FastPixelType>(image_modif, S_list, nb_scales,nb_core, max_value, dimx, dimy, dimz, debug_flag);	
			}
			delete[](image_modif);
			
			if (extension==1){
				// Write the result in nifti
				nifti_image *nim=NULL;
				nim = nifti_image_read(image_path.c_str(), 1);
			 	nim->datatype=DT_UINT8;
				nim->data=multiscale;
				int test=nifti_set_filenames(nim,writing_path.c_str(),0,nim->byteorder);
				nifti_image_write(nim);
			}
			
			else {
				vtkSmartPointer<vtkImageData> vtkimage = vtkSmartPointer<vtkImageData>::New();
				vtkimage->SetExtent(0, dimx-1, 0, dimy-1, 0, dimz-1);
				vtkimage->SetSpacing(1,1,1);
				vtkimage->SetOrigin(0,0,0);
				vtkimage->AllocateScalars(VTK_UNSIGNED_CHAR, 1);
				FastPixelType* usp=static_cast<FastPixelType*>(vtkimage->GetScalarPointer());
				memcpy(usp,multiscale,sizeof(FastPixelType)*dimx*dimy*dimz);
				vtkSmartPointer<vtkMetaImageWriter> writer = vtkSmartPointer<vtkMetaImageWriter>::New();
				writer->SetInputData(vtkimage);
				writer->SetCompression(false);
				writer->SetFileName(writing_path.c_str());
				writer->Write();
			}
			
			delete[](multiscale);
		}
	
	
		// ############################# SHORT IMAGE  ##############################

		else 
		{

			// Run RORPO
			InputType* multiscale;
	
			if (mask_path!="NULL"){
				multiscale=RORPO_multiscale<InputType>(image, S_list, nb_scales,nb_core, max_value, dimx, dimy, dimz, debug_flag, mask_buffer);	
				delete[](mask_buffer);
			}
			else 
				multiscale=RORPO_multiscale<InputType>(image, S_list, nb_scales,nb_core, max_value, dimx, dimy, dimz, debug_flag);	
	
			delete[](image);
			
			if (extension==1){
				// Write the result in nifti
				void *temp; 
				nifti_image *nim=NULL;
				nim = nifti_image_read(image_path.c_str(), 1);
				temp=nim->data;
				nim->data=multiscale;
				int test=nifti_set_filenames(nim,writing_path.c_str(),0,nim->byteorder);
				nifti_image_write(nim);
				nim->data=temp;
				nifti_image_free(nim);

			}
			else {
				vtkSmartPointer<vtkImageData> vtkimage = vtkSmartPointer<vtkImageData>::New();
				vtkimage->SetExtent(0, dimx-1, 0, dimy-1, 0, dimz-1);
				vtkimage->SetSpacing(1,1,1);
				vtkimage->SetOrigin(0,0,0);
				vtkimage->AllocateScalars(VTK_SHORT, 1);
				InputType* usp=static_cast<InputType*>(vtkimage->GetScalarPointer());
				memcpy(usp,multiscale,sizeof(InputType)*dimx*dimy*dimz);
				vtkSmartPointer<vtkMetaImageWriter> writer = vtkSmartPointer<vtkMetaImageWriter>::New();
				writer->SetInputData(vtkimage);
				writer->SetCompression(false);
				writer->SetFileName(writing_path.c_str());
				writer->Write();
			}
			delete[](multiscale);
		}

		return 1;
	}
}//end main

		
