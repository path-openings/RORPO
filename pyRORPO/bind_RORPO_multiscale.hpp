#pragma once
#include "Image/Image.hpp"
#include "Image/Image_IO_ITK.hpp"

#include "pyRORPO.hpp"

#include "bindingUtils.hpp"
#include "RORPO/RORPO_multiscale.hpp"

#define RORPO_MULTISCALE_BINDING(x) \
    m.def("RORPO_multiscale", &RORPO_multiscale_binding<x>, "RORPO multiscale", \
        py::arg("image") , \
        py::arg("spacing"), \
        py::arg("origin"), \
        py::arg("scaleMin"), \
        py::arg("factor"), \
        py::arg("nbScale"), \
        py::arg("nbCores") = 1, \
        py::arg("dilationSize") = 3 , \
        py::arg("verbose") = false, \
        py::arg("maskPath") = "" \
    ); \

#define RORPO_MULTISCALE_FROMFILE_BINDING(func) \
    m.def("RORPO_multiscale_fromFile", &func, "rorpo usage", \
        py::arg("inputpath"), \
        py::arg("outputPath"), \
        py::arg("scaleMin"), \
        py::arg("factor"), \
        py::arg("nbScales"), \
        py::arg("nbCores") = 1, \
        py::arg("dilationSize") = 3, \
        py::arg("maskPath") = "", \
        py::arg("verbose") = false, \
        py::arg("dicom") = false \
    ); \

namespace pyRORPO
{
    template<typename PixelType>
    Image3D<PixelType> RORPO_multiscale_usage2(Image3D<PixelType>& image,
                    std::vector<int>& scaleList,
                    std::vector<int>& window,
                    int nbCores,
                    int dilationSize,
                    int verbose,
                    std::string maskPath)
    {
        unsigned int dimz = image.dimZ();
        unsigned int dimy = image.dimY();
        unsigned int dimx= image.dimX();

        float  spacingX = image.spacingX();
        float  spacingY = image.spacingY();

        float  spacingZ = image.spacingZ();

        if (verbose){
            std::cout << "dimensions: [" << dimx << ", " << dimy << ", " << dimz << "]" << std::endl;
            std::cout << "spacing: [" << spacingX << ", " << spacingY << ", " << spacingZ << "]" << std::endl;
        }

        // ------------------ Compute input image intensity range ------------------

        std::pair<PixelType,PixelType> minmax = image.min_max_value();

        if (verbose){
            std::cout<< "Image intensity range: "<< (int)minmax.first << ", "
                    << (int)minmax.second << std::endl;
            std::cout<<std::endl;
        }

        // -------------------------- mask Image -----------------------------------

        Image3D<PixelType> mask;

        if (!maskPath.empty()) // A mask image is given
        {
            mask = Read_Itk_Image<PixelType>(maskPath);

            if (mask.dimX() != dimx || mask.dimY() != dimy || mask.dimZ() != dimz){
                std::cerr<<"Size of the mask image (dimx= "<<mask.dimX()
                        <<" dimy= "<<mask.dimY()<<" dimz="<<mask.dimZ()
                    << ") is different from size of the input image"<<std::endl;
                exit(1);
            }
        }

        Image3D<PixelType> res = RORPO_multiscale<PixelType, PixelType>(image, scaleList, nbCores, dilationSize, verbose, mask);
        return res;

    } // RORPO_multiscale_usage


    template<typename PixelType>
    py::array_t<PixelType> RORPO_multiscale_binding(py::array_t<PixelType> imageArray,
                    std::vector<float> spacing,
                    std::vector<double> origin,
                    float scaleMin,
                    float factor,
                    int nbScales,
                    int nbCores = 1,
                    int dilationSize = 2,
                    int verbose = false,
                    std::string maskPath = "")
    {
        std::vector<int> window(3);
        window[2] = 0;

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

        // ---------------------------- Load image data ----------------------------

        Image3D<PixelType> image = pyarrayToImage3D<PixelType>(
            imageArray,
            spacing[0],
            spacing[1],
            spacing[2],
            origin[0],
            origin[1],
            origin[2]
        );

        // ------------------------- Run RORPO_multiscale -------------------------

        Image3D<PixelType> output = RORPO_multiscale_usage2<PixelType>(
            image,
            scaleList,
            window,
            nbCores,
            dilationSize,
            verbose,
            maskPath
        );

        py::array_t<PixelType> result = py::array_t<PixelType>({output.dimZ(), output.dimY(), output.dimX()});
        
        py::buffer_info buf3 = result.request();

        PixelType* ptr = (PixelType*) buf3.ptr;

        memcpy(ptr, output.get_pointer(), output.size() * sizeof(PixelType));

        result.resize({output.dimZ(), output.dimY(), output.dimX()});

        return result;
    }



    // --------- From file path -----------------------------------------------

    template<typename PixelType>
    int RORPO_multiscale_usage(Image3D<PixelType>& image,
                    std::string outputPath,
                    std::vector<int>& scaleList,
                    std::vector<int>& window,
                    int nbCores,
                    int dilationSize,
                    int verbose,
                    std::string maskPath)
    {
        unsigned int dimz = image.dimZ();
        unsigned int dimy = image.dimY();
        unsigned int dimx= image.dimX();

        float  spacingX = image.spacingX();
        float  spacingY = image.spacingY();

        float  spacingZ = image.spacingZ();

        if (verbose){
            std::cout << "dimensions: [" << dimx << ", " << dimy << ", " << dimz << "]" << std::endl;
            std::cout << "spacing: [" << spacingX << ", " << spacingY << ", " << spacingZ << "]" << std::endl;
        }

        // ------------------ Compute input image intensity range ------------------

        std::pair<PixelType,PixelType> minmax = image.min_max_value();

        if (verbose){
            std::cout<< "Image intensity range: "<< (int)minmax.first << ", "
                    << (int)minmax.second << std::endl;
            std::cout<<std::endl;
        }

        // -------------------------- mask Image -----------------------------------

        Image3D<uint8_t> mask;

        if (!maskPath.empty()) // A mask image is given
        {
            mask = Read_Itk_Image<uint8_t>(maskPath);

            if (mask.dimX() != dimx || mask.dimY() != dimy || mask.dimZ() != dimz){
                std::cerr<<"Size of the mask image (dimx= "<<mask.dimX()
                        <<" dimy= "<<mask.dimY()<<" dimz="<<mask.dimZ()
                    << ") is different from size of the input image"<<std::endl;
                return 1;
            }
        }

        // #################### Convert input image to char #######################

        if (window[2] == 1 || typeid(PixelType) == typeid(float) ||
                typeid(PixelType) == typeid(double))
        {
            if (minmax.first > (PixelType) window[0])
                window[0] = minmax.first;

            if (minmax.second < (PixelType) window[1])
                window[1] = minmax.second;

            if(verbose){
                std::cout<<"Convert image intensity range from: [";
                std::cout<<minmax.first<<", "<<minmax.second<<"] to [";
                std::cout<<window[0]<<", "<<window[1]<<"]"<<std::endl;
            }

            image.window_dynamic(window[0], window[1]);

            if(verbose)
                std::cout << "Convert image to uint8" << std::endl;

            minmax.first = 0;
            minmax.second = 255;
            Image3D<uint8_t> imageChar = image.copy_image_2_uchar();

            // Run RORPO multiscale
            Image3D<uint8_t> multiscale =
                    RORPO_multiscale<uint8_t, uint8_t>(imageChar,
                                                    scaleList,
                                                    nbCores,
                                                    dilationSize,
                                                    verbose,
                                                    mask);

            // Write the result to nifti image
            Write_Itk_Image<uint8_t>( multiscale, outputPath );
        }

        // ################## Keep input image in PixelType ########################

        else {

            // ------------------------ Negative intensities -----------------------

            if (minmax.first < 0)
            {
                image -= minmax.first;

                if(verbose){
                    std::cout << "Convert image intensity range from [";
                    std::cout << (int)minmax.first << ", " << (int)minmax.second << "] to [";
                    std::cout << "0" << ", " << (int)minmax.second - (int)minmax.first << "]"
                                << std::endl;
                }
            }

            // Run RORPO multiscale
            Image3D<PixelType> multiscale =
                    RORPO_multiscale<PixelType, uint8_t>(image,
                                                        scaleList,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        mask);

            // getting min and max from multiscale image
            PixelType min = 255;
            PixelType max = 0;
    
            for( PixelType &value:multiscale.get_data() )
            {
                if( value > max)
                    max = value;
                if( value < min)
                    min = value;
            }


            // normalize output
            Image3D<float> multiscale_normalized(multiscale.dimX(),
                                                multiscale.dimY(),
                                                multiscale.dimZ(),
                                                multiscale.spacingX(),
                                                multiscale.spacingY(),
                                                multiscale.spacingZ(),
                                                multiscale.originX(),
                                                multiscale.originY(),
                                                multiscale.originZ()
                                                );


            for(unsigned int i=0; i<multiscale.size();i++)
            {
                multiscale_normalized.get_data()[i] = (multiscale.get_data()[i])/(float)(max); //
            }

            if(verbose)
            {
                std::cout<<"converting output image intensity :"<<(int)min<<"-"<<(int)max<<" to [0;1]"<<std::endl;
            }

            // Write the result to nifti image
            //Write_Itk_Image<float>(multiscale_normalized, outputPath);
            Write_Itk_Image<PixelType>(multiscale, outputPath);
        }

        return 0;
    } // RORPO_multiscale_usage

    inline
    int RORPO_multiscale_binding_fromPath(std::string input,
                std::string output,
                float scaleMin,
                float factor,
                int nbScales,
                int nbCores = 1,
                int dilationSize = 2,
                std::string maskPath = "",
                bool verbose = false,
                bool dicom = false)
    {
        std::string imagePath = input;
        std::string outputPath = output;

        std::vector<int> window(3);
        window[2] = 0;
        
        if(verbose)
            std::cout<<"dilation size:"<<dilationSize<<std::endl;

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

        // -------------------------- Read ITK Image -----------------------------
        Image3DMetadata imageMetadata = Read_Itk_Metadata(imagePath);

        // ---------------- Find image type and run RORPO multiscale ---------------
        int error;
        if (verbose){
            std::cout<<" "<<std::endl;
            std::cout << "------ INPUT IMAGE -------" << std::endl;
            std::cout << "Input image type: " << imageMetadata.pixelTypeString << std::endl;
        }

        if ( imageMetadata.nbDimensions != 3 ) {
            std::cout << "Error: input image dimension is " << imageMetadata.nbDimensions << " but should be 3 " << std::endl;
            return 1;
        }

        switch (imageMetadata.pixelType){
            case uint(itk::ImageIOBase::UCHAR):
            {
                Image3D<unsigned char> image = dicom?Read_Itk_Image_Series<unsigned char>(imagePath):Read_Itk_Image<unsigned char>(imagePath);
                error = RORPO_multiscale_usage<unsigned char>(image,
                                                        outputPath,
                                                        scaleList,
                                                        window,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        maskPath);
                break;
            }
            case uint(itk::ImageIOBase::CHAR):
            {
                Image3D<char> image = dicom?Read_Itk_Image_Series<char>(imagePath):Read_Itk_Image<char>(imagePath);
                error = RORPO_multiscale_usage<char>(image,
                                                        outputPath,
                                                        scaleList,
                                                        window,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        maskPath);
                break;
            }
            case uint(itk::ImageIOBase::USHORT):
            {
                Image3D<unsigned short> image = dicom?Read_Itk_Image_Series<unsigned short>(imagePath):Read_Itk_Image<unsigned short>(imagePath);
                error = RORPO_multiscale_usage<unsigned short>(image,
                                                        outputPath,
                                                        scaleList,
                                                        window,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        maskPath);
                break;
            }
            case uint(itk::ImageIOBase::SHORT):
            {
                Image3D<short> image = dicom?Read_Itk_Image_Series<short>(imagePath):Read_Itk_Image<short>(imagePath);
                error = RORPO_multiscale_usage<short>(image,
                                                        outputPath,
                                                        scaleList,
                                                        window,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        maskPath);
                break;
            }
            case uint(itk::ImageIOBase::UINT):
            {
                Image3D<unsigned int> image = dicom?Read_Itk_Image_Series<unsigned int>(imagePath):Read_Itk_Image<unsigned int>(imagePath);
                error = RORPO_multiscale_usage<unsigned int>(image,
                                                        outputPath,
                                                        scaleList,
                                                        window,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        maskPath);
                break;
            }
            case uint(itk::ImageIOBase::INT):
            {
                Image3D<int> image = dicom?Read_Itk_Image_Series<int>(imagePath):Read_Itk_Image<int>(imagePath);
                error = RORPO_multiscale_usage<int>(image,
                                                        outputPath,
                                                        scaleList,
                                                        window,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        maskPath);
                break;
            }
            case uint(itk::ImageIOBase::ULONG):
            {
                Image3D<unsigned long> image = dicom?Read_Itk_Image_Series<unsigned long>(imagePath):Read_Itk_Image<unsigned long>(imagePath);
                error = RORPO_multiscale_usage<unsigned long>(image,
                                                        outputPath,
                                                        scaleList,
                                                        window,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        maskPath);
                break;
            }
            case uint(itk::ImageIOBase::LONG):
            {
                Image3D<long> image = dicom?Read_Itk_Image_Series<long>(imagePath):Read_Itk_Image<long>(imagePath);
                error = RORPO_multiscale_usage<long>(image,
                                                        outputPath,
                                                        scaleList,
                                                        window,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        maskPath);
                break;
            }
            case uint(itk::ImageIOBase::ULONGLONG):
            {
                Image3D<unsigned long long> image = dicom?Read_Itk_Image_Series<unsigned long long>(imagePath):Read_Itk_Image<unsigned long long>(imagePath);
                error = RORPO_multiscale_usage<unsigned long long>(image,
                                                        outputPath,
                                                        scaleList,
                                                        window,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        maskPath);
                break;
            }
            case uint(itk::ImageIOBase::LONGLONG):
            {
                Image3D<long long> image = dicom?Read_Itk_Image_Series<long long>(imagePath):Read_Itk_Image<long long>(imagePath);
                error = RORPO_multiscale_usage<long long>(image,
                                                        outputPath,
                                                        scaleList,
                                                        window,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        maskPath);
                break;
            }
            case uint(itk::ImageIOBase::FLOAT):
            {
                Image3D<float> image = dicom?Read_Itk_Image_Series<float>(imagePath):Read_Itk_Image<float>(imagePath);
                error = RORPO_multiscale_usage<float>(image,
                                                        outputPath,
                                                        scaleList,
                                                        window,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        maskPath);
                break;
            }
            case uint(itk::ImageIOBase::DOUBLE):
            {
                Image3D<double> image = dicom?Read_Itk_Image_Series<double>(imagePath):Read_Itk_Image<double>(imagePath);
                error = RORPO_multiscale_usage<double>(image,
                                                        outputPath,
                                                        scaleList,
                                                        window,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        maskPath);
                break;
            }
            case uint(itk::ImageIOBase::UNKNOWNCOMPONENTTYPE):
            default:
                error = 1;
                std::cout << "Error: pixel type unknown." << std::endl;
                break;
        }
        return error;
    }
} // namespace pyRORPO