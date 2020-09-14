#pragma once

#include "pyRORPO.hpp"

#include "Image/Image.hpp"

namespace pyRORPO
{
    template<typename PixelType>
    inline Image3D<PixelType> pyarrayToImage3D(py::array_t<PixelType> imageInput,
            float spacingX,
            float spacingY,
            float spacingZ,
            double originX,
            double originY,
            double originZ)
    {
        auto bufImage = imageInput.request();

        auto image = Image3D<PixelType>(
            bufImage.shape[2], 
            bufImage.shape[1], 
            bufImage.shape[0], 
            spacingX,
            spacingY,
            spacingZ,
            originX,
            originY,
            originZ
        );

        image.add_data_from_pointer((PixelType*) bufImage.ptr);

        return image;
    }

    template<typename PixelType>
    inline Image3D<PixelType> image3DToPyarray(Image3D<PixelType> image)
    {
        py::array_t<PixelType> result = py::array_t<PixelType>({image.dimX(), image.dimY(), image.dimZ()});
        
        py::buffer_info buf3 = result.request();

        PixelType* ptr = (PixelType*) buf3.ptr;

        memcpy(ptr, image.get_pointer(), image.size() * sizeof(PixelType));

        result.resize({image.dimX(), image.dimY(), image.dimZ()});

        return result;
    }
}