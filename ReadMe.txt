################ libRORPO ###############

---------- File PO.hpp ----------

PO_3D : 
Compute the Path Opening operator in one orientation. The 7 orientations are defined in the function createNeighbourhood.

	Image : Input image
	L : Path length
	new_dimz : z dimension of input image
	new_dimy : y dimension of input image
	new_dimx : x dimension of input image
	orientations : defined the orientation used. Choices are [0,0,1] ; [1,0,0] ; [0,1,0] ; [1,1,1] ; [-1,1,1] ; [1,1,-1] ; [-1,1,-1]
	Outputbuffer : Result of the Path Opening


PO_3D_border :
Same function as PO_3D but use this one to compute just one path opening with one orientation.


---------- File Get_RPO_Orientations.hpp ----------

Get_RPO_Orientations :
Compute the 7 orientations of the Robust Path Opening and return them.

	input_buffer : input image
	image_dilat : dilation by a ball (radius 3 pixels) of input image
	L : Path length
	dimz : z dimension of input image
	dimy : y dimension of input image
	dimx : x dimension of input image
	res1 : resulting Robust Path Opening in the first orientation
	res2 : resulting Robust Path Opening in the first orientation
	res3 : resulting Robust Path Opening in the first orientation
	res4 : resulting Robust Path Opening in the first orientation
	res5 : resulting Robust Path Opening in the first orientation
	res6 : resulting Robust Path Opening in the first orientation
	res7 : resulting Robust Path Opening in the first orientation
	nb_core : number of cores used to compute the Path Opening (choose between 1 and 7)
	

---------- File RORPO.hpp ----------
Compute the Ranking Orientations Responses of Path Operators (RORPO)

	input_buffer : input image
	image_dilat : dilation by a ball (radius 3 pixels) of input image
	L : Path length
	nb_core : number of cores used to compute the Path Opening (choose between 1 and 7)
	dimx : x dimension of input image
	dimy : y dimension of input image
	dimz : z dimension of input image
	
	
---------- File RORPO_multiscale.hpp ----------
Compute the multiscale RORPO

	image_modif : input image
	S_list : vector containing the different path length (scales)
	nb_scales : number of scales
	nb_core : number of cores used to compute the Path Opening (choose between 1 and 7)
	max_value : maximum value of the input image
	dimx : x dimension of input image
	dimy : y dimension of input image
	dimz : z dimension of input image
	debug_flag : 1 (activated) or 0 (desactivated)
	mask_buffer : optional mask image
	


	

