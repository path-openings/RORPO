#ifndef DIFF_BUFFER_INCLUDED
#define DIFF_BUFFER_INCLUDED


template<typename PixelType>
PixelType* diff_buffer(PixelType* I1, PixelType* I2, int image_size)
{
    PixelType* res= new PixelType[image_size];
    for(int i=0; i<image_size; i++)
    {
        *(res+i)=*(I1+i)-*(I2+i);
    }
    return res;
}

#endif // DIFF_BUFFER_INCLUDED
