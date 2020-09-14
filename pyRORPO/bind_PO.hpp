#pragma once

#include "Image/Image.hpp"
#include "Image/Image_IO_ITK.hpp"

#include "pyRORPO.hpp"

#include "bindingUtils.hpp"
#include "RORPO/PO.hpp"

#define PO_BINDING(x) \
    m.def("PO", &PO_binding<x>, "PO usage", \
        py::arg("image"), \
        py::arg("scale"), \
        py::arg("index_image"), \
        py::arg("orientations"), \
        py::arg("b") \
    ); \

namespace pyRORPO
{
    template<typename PixelType>
    py::array_t<PixelType> PO_binding(py::array_t<PixelType> imageArray,
                    int scale,
                    std::vector<IndexType> &index_image,
                    std::vector<int> &orientations,
                    std::vector<bool> b)
    {

        // ---------------------------- Load image data ----------------------------

        Image3D<PixelType> image = pyarrayToImage3D<PixelType>( imageArray, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0 );


        auto output = Image3D<PixelType>( image.dimX(), image.dimY(), image.dimZ() );

        PO_3D<PixelType, PixelType>(image, scale, index_image, orientations, output, b);

        py::array_t<PixelType> result = py::array_t<PixelType>({output.dimZ(), output.dimY(), output.dimX()});
        py::buffer_info buf3 = result.request();
        PixelType* ptr = (PixelType*) buf3.ptr;
        memcpy(ptr, output.get_pointer(), output.size() * sizeof(PixelType));
        result.resize({output.dimZ(), output.dimY(), output.dimX()});

        return result;
    }

} // namespace pyRORPO