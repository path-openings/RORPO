#pragma once

#include "Image/Image.hpp"
#include "Image/Image_IO_ITK.hpp"

#include "pyRORPO.hpp"

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

        Image3D<PixelType> image = pyarrayToImage3D<PixelType>(imageArray, spacing, origin);

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

        return image3DToPyarray<PixelType>(output);
    }

} // namespace pyRORPO