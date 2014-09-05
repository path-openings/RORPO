#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <omp.h>
#include <cstdlib>

#include "Get_RPO_Orientations.hpp"

#include "geodilat_modif.hpp"
#include "diff_buffer.hpp"
#include "min_modif.hpp"
#include "max_modif.hpp"

#include "sorting.hpp"



template<typename T>
T* RORPO(T* input_buffer, T* image_dilat, int L, int nb_core, int dimx, int dimy, int dimz)
{

	// ######################################### RORPO classique #########################################
	
	
	int image_size=dimx*dimy*dimz;
   // Buffer for results
    
	T *res0=new T[dimx*dimy*dimz];
	T *res1=new T[dimx*dimy*dimz];
	T *res2=new T[dimx*dimy*dimz];
	T *res3=new T[dimx*dimy*dimz];
	T *res4=new T[dimx*dimy*dimz];
	T *res5=new T[dimx*dimy*dimz];
	T *res6=new T[dimx*dimy*dimz];

	
	// Compute RPO 
	Get_RPO_Orientations<T>(input_buffer, image_dilat, L, dimz, dimy, dimx, res0, res1, res2, res3, res4, res5, res6, nb_core);


    // allocate memory for sorting results
    T* RPOt0= new T[image_size];
    T* RPOt1= new T[image_size];
    T* RPOt2= new T[image_size];
    T* RPOt3= new T[image_size];
    T* RPOt4= new T[image_size];
    T* RPOt5= new T[image_size];
    T* RPOt6= new T[image_size];


    memcpy(&RPOt0[0], &res0[0],image_size*sizeof(T));
    memcpy(&RPOt1[0], &res1[0],image_size*sizeof(T));
    memcpy(&RPOt2[0], &res2[0],image_size*sizeof(T));
    memcpy(&RPOt3[0], &res3[0],image_size*sizeof(T));
    memcpy(&RPOt4[0], &res4[0],image_size*sizeof(T));
    memcpy(&RPOt5[0], &res5[0],image_size*sizeof(T));
    memcpy(&RPOt6[0], &res6[0],image_size*sizeof(T));

    sorting(RPOt0,RPOt1,RPOt2,RPOt3,RPOt4,RPOt5,RPOt6, image_size);

    T *RPO_RPO=diff_buffer(RPOt6,RPOt3,image_size);

    // ######################################### Limit Orientations Treatment #########################################

	 // ------------------------- Computation of Imin ----------------------------

	 // ---- Imin limit case 4 orientations ----
	 //horizontal + vertical + diag1 + diag4
	 T* Imin4_1=min_alloc(res0,res1,dimx,dimy,dimz);
     min_crush(Imin4_1,res3,dimx,dimy,dimz);
     min_crush(Imin4_1,res6,dimx,dimy,dimz);

     //horizontal + vertical + diag2 + diag3
     T* Imin4_2=min_alloc(res0,res1,dimx,dimy,dimz);
     min_crush(Imin4_2,res4,dimx,dimy,dimz);
     min_crush(Imin4_2,res5,dimx,dimy,dimz);

     //horizontal + profondeur + diag2+ diag4
     T* Imin4_3=min_alloc(res0,res2,dimx,dimy,dimz);
     min_crush(Imin4_3,res4,dimx,dimy,dimz);
     min_crush(Imin4_3,res6,dimx,dimy,dimz);

     //horizontal + profondeur + diag1+ diag3
	 T* Imin4_4=min_alloc(res0,res2,dimx,dimy,dimz);
     min_crush(Imin4_4,res3,dimx,dimy,dimz);
     min_crush(Imin4_4,res5,dimx,dimy,dimz);

     //vertical + profondeur + diag1+ diag2
     T* Imin4_5=min_alloc(res1,res2,dimx,dimy,dimz);
     min_crush(Imin4_5,res3,dimx,dimy,dimz);
     min_crush(Imin4_5,res4,dimx,dimy,dimz);

     //vertical + profondeur + diag3+ diag4
     T* Imin4_6=min_alloc(res1,res2,dimx,dimy,dimz);
     min_crush(Imin4_6,res5,dimx,dimy,dimz);
     min_crush(Imin4_6,res6,dimx,dimy,dimz);

     // Final
     T* Imin4=max_alloc(Imin4_1,Imin4_2,dimx,dimy,dimz);
     max_crush(Imin4,Imin4_3,dimx,dimy,dimz);
     max_crush(Imin4,Imin4_4,dimx,dimy,dimz);
     max_crush(Imin4,Imin4_5,dimx,dimy,dimz);
     max_crush(Imin4,Imin4_6,dimx,dimy,dimz);

	 // ---- Imin limit case 5 orientations ----
    T* Imin5=min_alloc(res3,res4,dimx,dimy,dimz);
    min_crush(Imin5,res5,dimx,dimy,dimz);
    min_crush(Imin5,res6,dimx,dimy,dimz);

    // ------------------------- Computation of Imin2 ----------------------------
    //geodesic reconstruction of RPO6 in RPO4
    T* RPO6=geodilat_modif(RPOt1,RPOt3,18,-1,dimx,dimy,dimz);

    //geodesic reconstruction of RPO5 in RPO4
    T* RPO5=geodilat_modif(RPOt2,RPOt3,18,-1,dimx,dimy,dimz);

    // ---- Imin2 limit case 4 orientations ----
    T* Imin2_4=min_alloc(Imin4,RPO5,dimx,dimy,dimz);

    // ---- Imin2 limit case 5 orientations ----
    T* Imin2_5=min_alloc(Imin5,RPO6,dimx,dimy,dimz);

    // ------------------------- Final Result --------------------------------

    T* diff_Imin4=diff_buffer(Imin4,Imin2_4,image_size);
    T* diff_Imin5=diff_buffer(Imin5,Imin2_5,image_size);

    T* RPO_RPO_limit_orientations=max_alloc(RPO_RPO,diff_Imin4,dimx,dimy,dimz);
    max_crush(RPO_RPO_limit_orientations,diff_Imin5,dimx,dimy,dimz);
    
    
    /*nifti_image *nim=NULL;
    nim = nifti_image_read("/medit/data/Images/Images_synthetiques/Niveaux_gris/Plan_Spiral_3D/short/plan_spiral_blob_short_5_5_0.9.nii", 1);
    NewPT* image=((NewPT*)(nim->data));
   
 	std::ostringstream write;
	write<< "/medit/home/odyssee/Documents/VIVABRAIN/Programme/Code_RORPO/RORPO_dilat/test/multiscale_"<<L<<".nii";
   
    std::cout<<"Write"<<std::endl;
	nim->data=RPO_RPO_limit_orientations;
	int test=nifti_set_filenames(nim,write.str().c_str(),0,nim->byteorder);
	nifti_image_write(nim);
*/
   // Free memory before ending
   free(Imin2_4);
   free(Imin2_5);
   free(Imin4);
   free(Imin5);
   free(Imin4_1);
   free(Imin4_2);
   free(Imin4_3);
   free(Imin4_4);
   free(Imin4_5);
   free(Imin4_6);
   delete[] diff_Imin4;
   delete[] diff_Imin5;
   free(RPO5);
   free(RPO6);
   delete[] RPO_RPO;

   delete[](res0);
   delete[](res1);
   delete[](res2);
   delete[](res3);
   delete[](res4);
   delete[](res5);
   delete[](res6);

   delete[](RPOt0);
   delete[](RPOt1);
   delete[](RPOt2);
   delete[](RPOt3);
   delete[](RPOt4);
   delete[](RPOt5);
   delete[](RPOt6);

   return RPO_RPO_limit_orientations;
   
}
