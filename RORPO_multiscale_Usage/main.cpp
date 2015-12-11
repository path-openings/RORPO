#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>
#include <getopt.h>
#include <typeinfo>
#include <sstream>

#include "docopt.h"
#include "Image/Image.hpp"
#include "Image/Image_IO_nifti.hpp"
#include "RORPO/RORPO_multiscale.hpp"


// Split a string
std::vector<std::string> split(std::string str, char delimiter) {
  std::vector<std::string> internal;
  std::stringstream ss(str); // Turn the string into a stream.
  std::string tok;

  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }
  return internal;
}


template<typename PixelType>
int RORPO_multiscale_usage(Image3D<PixelType> image,
                std::string outputPath,
                std::vector<int> scaleList,
                std::vector<int> window,
                int nbCores,
                int verbose,
                std::string maskPath)
{
    int dimz = image.Dimz();
    int dimy = image.Dimy();
    int dimx= image.Dimx();
	
    if (verbose){
		std::cout<<"NIFTI Image"<<std::endl;
        std::cout<<"dimx= "<<dimx<<"; dimy= "<<dimy<<"; dimz= "<<dimz<<std::endl;
	}

    // ------------------ Compute input image intensity range ------------------

    std::vector<PixelType> minmax = image.min_max_value();

    if (verbose){
        std::cout<< "Image intensity range: "<< (float)minmax[0]<<", "
                 << (float)minmax[1] << std::endl;
        std::cout<<std::endl;
	}

    // -------------------------- mask Image -----------------------------------
		
    Image3D<unsigned char> mask;

    if (!maskPath.empty()) // A mask image is given
	{
        mask = read_3D_nifti_image<uint8_t>(maskPath);

        if (mask.Dimx() != dimx || mask.Dimy() != dimy || mask.Dimz() != dimz){
            std::cerr<<"Size of the mask image (dimx= "<<mask.Dimx()
                    <<" dimy= "<<mask.Dimy()<<" dimz="<<mask.Dimz()
                   << ") is different from size of the input image"<<std::endl;
            return 1;
        }
    }

    // #################### Convert input image to char #######################

    if (window[2] == 1 || typeid(PixelType) == typeid(float) ||
            typeid(PixelType) == typeid(double)){

        if (window[2] == 1) { // window the intensity range to [0,255]
            if (minmax[0] > (PixelType) window[0])
                window[0] = minmax[0];

            if (minmax[1] < (PixelType) window[1])
                window[1] = minmax[1];

            image.window_dynamic(window[0], window[1]);

            if(verbose){
                std::cout<<"Convert image intensity range from: [";
                std::cout<<window[0]<<", "<<window[1]<<"] to [";
                std::cout<<"0"<<", "<<"255"<<"]"<<std::endl;
            }
        }

        else{ // convert the full intensity range to [0,255]
            image.window_dynamic(minmax[0], minmax[1]);
            if(verbose){
                std::cout<<"Convert image intensity range from: [";
                std::cout<<minmax[0]<<", "<<minmax[1]<<"] to [";
                std::cout<<"0"<<", "<<"255"<<"]"<<std::endl;
            }
        }

        minmax = image.min_max_value();
        Image3D<uint8_t>imageChar= image.copy_image_2_uchar();

        if(verbose)
            std::cout<<"Convert image to uint8"<<std::endl;

        // Run RORPO multiscale
        Image3D<uint8_t> multiscale=
                RORPO_multiscale<uint8_t, uint8_t>(imageChar,
                                                   scaleList,
                                                   nbCores,
                                                   verbose,
                                                   mask);

        // Write the result to nifti image
        write_3D_nifti_image<uint8_t>(multiscale, outputPath);
    }

    // ################## Keep input image in PixelType ########################

    else {

        // ------------------------ Negative intensities -----------------------

        if (minmax[0] < 0){
            image - minmax[0];

            if(verbose){
                std::cout<<"Convert image intensity range from [";
                std::cout<<minmax[0]<<", "<<minmax[1]<<"] to [";
                std::cout<<"0"<<", "<<minmax[1] - minmax[0]<<"]"<<std::endl;
            }
            minmax = image.min_max_value();
        }

        // Run RORPO multiscale
        Image3D<PixelType> multiscale=
                RORPO_multiscale<PixelType, uint8_t>(image,
                                                              scaleList,
                                                              nbCores,
                                                              verbose,
                                                              mask);

        // Write the result to nifti image
        write_3D_nifti_image<PixelType>(multiscale, outputPath);

    }

    return 0;
} // RORPO_multiscale_usage


// Parse command line with docopt
static const char USAGE[] =
R"(RORPO_multiscale_usage.

    USAGE:
    RORPO_multiscale_usage <imagePath> <outputPath> <scaleMin> <factor> <nbScales> [--window=min,max] [--core=nbCores] [--mask=maskPath] [--verbose]

    Options:
         --core=<nbCores>  Number of CPUs used for RPO computation
         --window=min,max     Convert intensity range [min, max] of the intput \
                              image to [0,255] and convert to uint8 image\
                              (strongly decrease computation time).
         --mask=maskPath      Path to a mask for the input image \
                              (0 for the background; not 0 for the foreground).\
                              mask image type must be uint8.
         --verbose            Activation of a verbose mode.
        )";


int main(int argc, char **argv)
{

    // -------------- Parse arguments and initialize parameters ----------------
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
                                                  {argv + 1, argv + argc},
                                                  true,
                                                  "RORPO_multiscale_usage 2.0");

    std::cout<<" "<<std::endl;
    std::cout<<"Parameters: "<<std::endl;
    for(auto const& arg : args) {
        std::cout << arg.first << ": " << arg.second << std::endl;
    }

    std::string imagePath = args["<imagePath>"].asString();
    std::string outputPath = args["<outputPath>"].asString();
    float scaleMin = std::stoi(args["<scaleMin>"].asString());
    float factor = std::stof(args["<factor>"].asString());
    int nbScales = std::stoi(args["<nbScales>"].asString());
    std::vector<int> window(3);
    int nbCores = 1;
    std::string maskPath;
    bool verbose = args["--verbose"].asBool();

    if (args["--mask"])
        maskPath = args["--mask"].asString();

    if (args["--core"])
        nbCores = std::stoi(args["--core"].asString());

    if (args["--window"]){
        std::vector<std::string> windowVector =
                split(args["--window"].asString(),',');

        window[0] = std::stoi(windowVector[0]);
        window[1] = std::stoi(windowVector[1]);
        window[2] = 1; // --window used
    }
    else
        window[2] = 0; // --window not used


    // -------------------------- Scales computation ---------------------------

    std::vector<int> scaleList(nbScales);
    scaleList[0] = scaleMin;

    for (int i = 1; i < nbScales; ++i)
        scaleList[i] = int(scaleMin * pow(factor, i));

    if (verbose){
        std::cout<<"Scales : ";
        std::cout<<scaleList[0];
        for (int i = 1; i < nbScales; ++i)
            std::cout<<','<<scaleList[i];
    }

    // -------------------------- Read Nifti Image -----------------------------
    nifti_image *nim = NULL;
    nim = nifti_image_read(imagePath.c_str(), 1);

    // ---------------- Find image type and run RORPO multiscale ---------------
    int error;
    if (verbose){
        std::cout<<" "<<std::endl;
        std::cout << "------ INPUT IMAGE -------" << std::endl;
    }

    switch(nim->datatype){
        case 2: { // uint8
            if (verbose)
                std::cout<<"Input image type: uint8"<<std::endl;

            Image3D<uint8_t> image = read_3D_nifti_image<uint8_t>(nim);
            nifti_image_free(nim);

            error = RORPO_multiscale_usage<uint8_t>(image,
                                                    outputPath,
                                                    scaleList,
                                                    window,
                                                    nbCores,
                                                    verbose,
                                                    maskPath);
            break;
    }

        case 4: { // uint16
            if (verbose)
                std::cout<<"Input image type: uint16 "<<std::endl;

            Image3D<u_int16_t> image = read_3D_nifti_image<u_int16_t>(nim);
            nifti_image_free(nim);

            error = RORPO_multiscale_usage<u_int16_t>(image,
                                                      outputPath,
                                                      scaleList,
                                                      window,
                                                      nbCores,
                                                      verbose,
                                                      maskPath);
            break;
        }

        case 8: { // uint32
            if (verbose)
                std::cout<<"Input image type: int32 "<<std::endl;

            Image3D<int32_t> image = read_3D_nifti_image<int32_t>(nim);
            nifti_image_free(nim);

            error = RORPO_multiscale_usage<int32_t>(image,
                                                    outputPath,
                                                    scaleList,
                                                    window,
                                                    nbCores,
                                                    verbose,
                                                    maskPath);
            break;
        }

        case 16: { // float
            if (verbose)
                std::cout<<"Input image type: float "<<std::endl;

            Image3D<float_t> image = read_3D_nifti_image<float_t>(nim);
            nifti_image_free(nim);

            error = RORPO_multiscale_usage<float_t>(image,
                                                    outputPath,
                                                    scaleList,
                                                    window,
                                                    nbCores,
                                                    verbose,
                                                    maskPath);
            break;
        }
        case 256: { // int8
            if (verbose)
                std::cout<<"Input image type: int8 "<<std::endl;

            Image3D<int8_t> image = read_3D_nifti_image<int8_t>(nim);
            nifti_image_free(nim);

            error = RORPO_multiscale_usage<int8_t>(image,
                                                   outputPath,
                                                   scaleList,
                                                   window,
                                                   nbCores,
                                                   verbose,
                                                   maskPath);
            break;
        }
        case 512: { // int16
            if (verbose)
                std::cout<<"Input image type: int16 "<<std::endl;

            Image3D<int16_t> image = read_3D_nifti_image<int16_t>(nim);
            nifti_image_free(nim);

            error = RORPO_multiscale_usage<int16_t>(image,
                                                    outputPath,
                                                    scaleList,
                                                    window,
                                                    nbCores,
                                                    verbose,
                                                    maskPath);
            break;
        }
        case 768: { // uint32
            if (verbose)
                std::cout<<"Input image type: uint32 "<<std::endl;

            Image3D<uint32_t> image = read_3D_nifti_image<uint32_t>(nim);
            nifti_image_free(nim);

            error = RORPO_multiscale_usage<uint32_t>(image,
                                                     outputPath,
                                                     scaleList,
                                                     window,
                                                     nbCores,
                                                     verbose,
                                                     maskPath);
            break;
        }
        default : {
            std::cerr<<"Input image type not supported "<<std::endl;
            error = 1;
            break;
        }
    } //end switch

    return error;

}//end main

