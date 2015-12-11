#ifndef Image_IO_NIFTI_INCLUDED
#define Image_IO_NIFTI_INCLUDED

#include <string>
#include <vector>

#include "Image.hpp"
#include <typeinfo>

#include "nifti/nifti1_io.h"

// ############################# Nifti Image ##############################

template<typename PixelType>
Image3D<PixelType> read_3D_nifti_image(std::string imagePath){
	
    nifti_image *nim = NULL;
    nim = nifti_image_read(imagePath.c_str(), 1);
    Image3D<PixelType> image(nim->nx, nim->ny, nim->nz);
    image.add_data_from_pointer((PixelType*)(nim->data));
    nifti_image_free(nim);
	
    return image;
}

template<typename PixelType>
Image3D<PixelType> read_3D_nifti_image(nifti_image* nim){

    Image3D<PixelType> image(nim->nx, nim->ny, nim->nz);
    image.add_data_from_pointer((PixelType*)(nim->data));

    return image;
}

template<typename PixelType>
Image2D<PixelType> read_2D_nifti_image(std::string image_path){

    nifti_image *nim = NULL;
    nim = nifti_image_read(image_path.c_str(), 1);
    Image2D<PixelType> I(nim->nx, nim->ny);
    I.add_data_from_pointer((PixelType*)(nim->data));

    return I;

}

template<typename PixelType>
int write_3D_nifti_image(Image3D<PixelType> image, std::string outputPath){

    int t[8] = {3, image.Dimx(), image.Dimy(), image.Dimz(), 0,0,0,0};
    nifti_image *nim_write;

    if (typeid(PixelType) == typeid(int16_t)){
        nim_write = nifti_make_new_nim(t, 4, 1);
    }
    else if (typeid(PixelType) == typeid(u_int16_t)){
        nim_write = nifti_make_new_nim(t, 512, 1);
    }
    else if (typeid(PixelType) == typeid(u_int8_t)){
        nim_write = nifti_make_new_nim(t, 2, 1);
    }
    else if (typeid(PixelType) == typeid(int8_t)){
            nim_write = nifti_make_new_nim(t, 256, 1);
    }
    else if (typeid(PixelType) == typeid(int32_t)){
        nim_write = nifti_make_new_nim(t, 8, 1);
    }
    else if (typeid(PixelType) == typeid(float)){
        nim_write = nifti_make_new_nim(t, 16, 1);
    }

    else {
        std::cerr<<"Error write nifti image: Image type not supported"
                <<std::endl;
        return 1;
    }
    nim_write->data = image.get_pointer();
    int error = nifti_set_filenames(nim_write, outputPath.c_str(), 0, nim_write->byteorder);
    if (error == 0){
        nifti_image_write(nim_write);
    }
    return error;
}


template<typename PixelType>
int write_2D_nifti_image(Image2D<PixelType> image, std::string outputPath){

    int t[8] = {2, image.Dimy(), image.Dimx(),0,0,0,0,0};
    nifti_image *nim_write;

    if (typeid(PixelType) == typeid(int16_t)){
        nim_write = nifti_make_new_nim(t, 4, 1);
    }
    if (typeid(PixelType) == typeid(u_int16_t)){
        nim_write = nifti_make_new_nim(t, 512, 1);
    }
    if (typeid(PixelType) == typeid(u_int8_t)){
        nim_write = nifti_make_new_nim(t, 2, 1);
    }
    if (typeid(PixelType) == typeid(int8_t)){
            nim_write = nifti_make_new_nim(t, 256, 1);
    }
    if (typeid(PixelType) == typeid(int32_t)){
        nim_write = nifti_make_new_nim(t, 8, 1);
    }
    if (typeid(PixelType) == typeid(float)){
        nim_write = nifti_make_new_nim(t, 16, 1);
    }

    nim_write->data = image.GetPointer();
    int error = nifti_set_filenames(nim_write, outputPath.c_str(), 0, nim_write->byteorder);
    if (error == 0){
        nifti_image_write(nim_write);
    }
    return error;

}

#endif // Image_IO_NIFTI_INCLUDED
	
