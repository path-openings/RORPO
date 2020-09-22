#pragma once

#include "Image/Image.hpp"
#include "Image/Image_IO_ITK.hpp"

#include "pyRORPO.hpp"

#include "RORPO/RORPO.hpp"

#include <optional>

#define RORPO_BINDING(x) \
    m.def("RORPO", &RORPO_binding<x>, "rorpo usage", \
        py::arg("image"), \
        py::arg("scale"), \
        py::arg("spacing") = py::none(), \
        py::arg("origin") = py::none(), \
        py::arg("nbCores") = 1, \
        py::arg("dilationSize") = 2, \
        py::arg("verbose") = false, \
        py::arg("mask") = py::none() \
    ); \

namespace pyRORPO
{
    template<typename PixelType>
    py::array_t<PixelType> RORPO_binding(py::array_t<PixelType> imageArray,
                    int scale,
                    std::optional<std::vector<float>> spacingOpt,
                    std::optional<std::vector<double>> originOpt,
                    int nbCores = 1,
                    int dilationSize = 2,
                    int verbose = false,
                    std::optional<py::array_t<PixelType>> maskArray = py::none())
    {
        std::vector<int> window(3);
        window[2] = 0;

        std::vector<float> spacing = spacingOpt ? *spacingOpt : std::vector<float>{1.0, 1.0, 1.0};
        std::vector<double> origin = originOpt ? *originOpt : std::vector<double>{1.0, 1.0, 1.0};

        // ---------------------------- Load image data ----------------------------

        Image3D<PixelType> image = pyarrayToImage3D<PixelType>(imageArray, spacing, origin);

        // -------------------------- mask Image -----------------------------------

        Image3D<PixelType> mask;

        if (maskArray)
            mask = pyarrayToImage3D<PixelType>(*maskArray, spacing, origin);

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