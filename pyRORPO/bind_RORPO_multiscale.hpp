#pragma once
#include "Image/Image.hpp"
#include "Image/Image_IO_ITK.hpp"

#include "pyRORPO.hpp"

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

        Image3D<PixelType> image = pyarrayToImage3D<PixelType>(imageArray, spacing, origin);

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

        return image3DToPyarray<PixelType>(output);
    }

} // namespace pyRORPO