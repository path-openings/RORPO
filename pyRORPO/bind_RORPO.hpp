#pragma once

#include "Image/Image.hpp"
#include "Image/Image_IO_ITK.hpp"

#include "pyRORPO.hpp"

#include "bindingUtils.hpp"
#include "RORPO/RORPO.hpp"

#define RORPO_BINDING(x) \
    m.def("RORPO", &RORPO_binding<x>, "rorpo usage", \
        py::arg("image"), \
        py::arg("spacing"), \
        py::arg("origin"), \
        py::arg("scale"), \
        py::arg("nbCores") = 1, \
        py::arg("dilationSize") = 3, \
        py::arg("verbose") = false, \
        py::arg("maskPath") = "" \
    ); \

#define RORPO_FROMFILE_BINDING() \
    m.def("RORPO_fromFile", &RORPO_binding<uint>, "rorpo usage from file", \
        py::arg("image"), \
        py::arg("spacing"), \
        py::arg("origin"), \
        py::arg("scale"), \
        py::arg("nbCores") = 1, \
        py::arg("dilationSize") = 3, \
        py::arg("verbose") = false, \
        py::arg("maskPath") = "" \
    ); \

namespace pyRORPO
{
    template<typename PixelType>
    py::array_t<PixelType> RORPO_binding(py::array_t<PixelType> imageArray,
                    std::vector<float> spacing,
                    std::vector<double> origin,
                    int scale,
                    int nbCores = 1,
                    int dilationSize = 2,
                    int verbose = false,
                    std::string maskPath = "")
    {
        std::vector<int> window(3);
        window[2] = 0;

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

        // -------------------------- mask Image -----------------------------------

        Image3D<PixelType> mask;

        if (!maskPath.empty()) // A mask image is given
        {
            mask = Read_Itk_Image<PixelType>(maskPath);

            if (mask.dimX() != image.dimX() || mask.dimY() != image.dimY() || mask.dimZ() != image.dimZ()){
                std::cerr<<"Size of the mask image (dimx= "<<mask.dimX()
                        <<" dimy= "<<mask.dimY()<<" dimz="<<mask.dimZ()
                    << ") is different from size of the input image"<<std::endl;
                exit(1);
            }
        }

        // ---------------------------- Run RORPO ----------------------------------

        Image3D<PixelType> output = RORPO<PixelType, PixelType>(
            image,
            scale,
            nbCores,
            dilationSize,
            //verbose,
            mask
        );

        py::array_t<PixelType> result = py::array_t<PixelType>({output.dimZ(), output.dimY(), output.dimX()});
        
        py::buffer_info buf3 = result.request();

        PixelType* ptr = (PixelType*) buf3.ptr;

        memcpy(ptr, output.get_pointer(), output.size() * sizeof(PixelType));

        result.resize({output.dimZ(), output.dimY(), output.dimX()});

        return result;
    }

    /*
    int RORPO_fromFile_binding(std::string imagePath,
                    std::string output,
                    int scale,
                    int nbCores = 1,
                    int dilationSize = 2,
                    int verbose = false,
                    std::string maskPath = "",
                    bool dicom = false)
    {
        std::vector<int> window(3);
        window[2] = 0;

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
                Image3D<unsigned char> output = RORPO_binding<unsigned char>(image,
                                                        { 1.0, 1.0, 1.0 },
                                                        { 0.0, 0.0, 0.0 },
                                                        scale,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        maskPath);
                Write_Itk_Image<unsigned char>(image, outputPath);
                break;
            }
            case uint(itk::ImageIOBase::CHAR):
            {
                Image3D<char> image = dicom?Read_Itk_Image_Series<char>(imagePath):Read_Itk_Image<char>(imagePath);
                Image3D output = RORPO_binding<char>(image,
                                                        { 1.0, 1.0, 1.0 },
                                                        { 0.0, 0.0, 0.0 }
                                                        scale,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        maskPath);
                Write_Itk_Image<char>(image, outputPath);
                break;
            }
            case uint(itk::ImageIOBase::USHORT):
            {
                Image3D<unsigned short> image = dicom?Read_Itk_Image_Series<unsigned short>(imagePath):Read_Itk_Image<unsigned short>(imagePath);
                Image3D output = RORPO_binding<unsigned short>(image,
                                                        { 1.0, 1.0, 1.0 },
                                                        { 0.0, 0.0, 0.0 }
                                                        scale,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        maskPath);
                Write_Itk_Image<unsigned short>(image, outputPath);
                break;
            }
            case uint(itk::ImageIOBase::SHORT):
            {
                Image3D<short> image = dicom?Read_Itk_Image_Series<short>(imagePath):Read_Itk_Image<short>(imagePath);
                Image3D output = RORPO_binding<short>(image,
                                                        { 1.0, 1.0, 1.0 },
                                                        { 0.0, 0.0, 0.0 }
                                                        scale,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        maskPath);
                Write_Itk_Image<short>(image, outputPath);
                break;
            }
            case uint(itk::ImageIOBase::UINT):
            {
                Image3D<unsigned int> image = dicom?Read_Itk_Image_Series<unsigned int>(imagePath):Read_Itk_Image<unsigned int>(imagePath);
                Image3D output = RORPO_binding<unsigned int>(image,
                                                        { 1.0, 1.0, 1.0 },
                                                        { 0.0, 0.0, 0.0 }
                                                        scale,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        maskPath);
                Write_Itk_Image<unsigned int>(image, outputPath);
                break;
            }
            case uint(itk::ImageIOBase::INT):
            {
                Image3D<int> image = dicom?Read_Itk_Image_Series<int>(imagePath):Read_Itk_Image<int>(imagePath);
                Image3D output = RORPO_binding<int>(image,
                                                        { 1.0, 1.0, 1.0 },
                                                        { 0.0, 0.0, 0.0 }
                                                        scale,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        maskPath);
                Write_Itk_Image<int>(image, outputPath);
                break;
            }
            case uint(itk::ImageIOBase::ULONG):
            {
                Image3D<unsigned long> image = dicom?Read_Itk_Image_Series<unsigned long>(imagePath):Read_Itk_Image<unsigned long>(imagePath);
                Image3D output = RORPO_binding<unsigned long>(image,
                                                        { 1.0, 1.0, 1.0 },
                                                        { 0.0, 0.0, 0.0 }
                                                        scale,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        maskPath);
                Write_Itk_Image<unsigned long>(image, outputPath);
                break;
            }
            case uint(itk::ImageIOBase::LONG):
            {
                Image3D<long> image = dicom?Read_Itk_Image_Series<long>(imagePath):Read_Itk_Image<long>(imagePath);
                Image3D output = RORPO_binding<long>(image,
                                                        { 1.0, 1.0, 1.0 },
                                                        { 0.0, 0.0, 0.0 }
                                                        scale,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        maskPath);
                Write_Itk_Image<long>(image, outputPath);
                break;
            }
            case uint(itk::ImageIOBase::ULONGLONG):
            {
                Image3D<unsigned long long> image = dicom?Read_Itk_Image_Series<unsigned long long>(imagePath):Read_Itk_Image<unsigned long long>(imagePath);
                Image3D output = RORPO_binding<unsigned long long>(image,
                                                        { 1.0, 1.0, 1.0 },
                                                        { 0.0, 0.0, 0.0 }
                                                        scale,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        maskPath);
                Write_Itk_Image<unsigned long long>(image, outputPath);
                break;
            }
            case uint(itk::ImageIOBase::LONGLONG):
            {
                Image3D<long long> image = dicom?Read_Itk_Image_Series<long long>(imagePath):Read_Itk_Image<long long>(imagePath);
                Image3D output = RORPO_binding<long long>(image,
                                                        { 1.0, 1.0, 1.0 },
                                                        { 0.0, 0.0, 0.0 }
                                                        scale,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        maskPath);
                Write_Itk_Image<long long>(image, outputPath);
                break;
            }
            case uint(itk::ImageIOBase::FLOAT):
            {
                Image3D<float> image = dicom?Read_Itk_Image_Series<float>(imagePath):Read_Itk_Image<float>(imagePath);
                Image3D output = RORPO_binding<float>(image,
                                                        { 1.0, 1.0, 1.0 },
                                                        { 0.0, 0.0, 0.0 }
                                                        scale,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        maskPath);
                Write_Itk_Image<float>(image, outputPath);
                break;
            }
            case uint(itk::ImageIOBase::DOUBLE):
            {
                Image3D<double> image = dicom?Read_Itk_Image_Series<double>(imagePath):Read_Itk_Image<double>(imagePath);
                Image3D output = RORPO_binding<double>(image,
                                                        { 1.0, 1.0, 1.0 },
                                                        { 0.0, 0.0, 0.0 }
                                                        scale,
                                                        nbCores,
                                                        dilationSize,
                                                        verbose,
                                                        maskPath);
                Write_Itk_Image<double>(image, outputPath);
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
    */
} // namespace pyRORPO