#ifndef SORTING_HPP_INCLUDED
#define SORTING_HPP_INCLUDED

template<typename PixelType>
static inline void sort7_sorting_network_simple_swap(PixelType ** d){

#define mini(x, y) (x<y?x:y)
#define maxi(x, y) (x<y?y:x)
//#define SWAP(x,y) { const int a = min(d[x], d[y]); const int b = max(d[x], d[y]); d[x] = a; d[y] = b;}
#define SWAP(x,y) { const int a = mini(*d[x], *d[y]); const int b = maxi(*d[x], *d[y]); *d[x] = a; *d[y] = b;}
    SWAP(1, 2);
    SWAP(3, 4);
    SWAP(5, 6);
    SWAP(0, 2);
    SWAP(4, 6);
    SWAP(3, 5);
    SWAP(2, 6);
    SWAP(1, 5);
    SWAP(0, 4);
    SWAP(2, 5);
    SWAP(0, 3);
    SWAP(2, 4);
    SWAP(1, 3);
    SWAP(0, 1);
    SWAP(2, 3);
    SWAP(4, 5);
#undef SWAP
#undef min
#undef max
}


template<typename PixelType>
static void sorting(PixelType* I1, PixelType* I2, PixelType* I3, PixelType* I4, PixelType* I5, PixelType* I6, PixelType* I7, int N)
{
    PixelType* d[7];
    d[0] = I1;
    d[1] = I2;
    d[2] = I3;
    d[3] = I4;
    d[4] = I5;
    d[5] = I6;
    d[6] = I7;

    for (int i=0; i<N; i++) {
        sort7_sorting_network_simple_swap(d);
        for (int j=0;j<7;j++)
            d[j]++;
    }
}

#ifdef _TEST_SORT_
int main(int argc, char **argv) {
   for (int i=0; i<10; i++){
        PixelType d[7], *b[7];
        for (int j=0;j<7; j++) {d[j]=rand()%256;b[j]=d+j;}
        sort7_sorting_network_simple_swap(b);
        for (int j=0; j<7; j++) std::cout<< d[j] << "--" ;
        std::cout << std::endl;
    }
    int i;
    std::cin >> i;
}
#endif // _TEST_SORT_
#endif // SORTING_HPP_INCLUDED
