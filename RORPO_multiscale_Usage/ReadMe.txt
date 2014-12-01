################ RORPO Multiscale Usage ##############

Compute the RORPO multiscale for blood vessels from a .nii image.

Accepted type : int8, uint8, int16, uint16, int32, uint32, float
An isotropic image resolution is required (cubic voxels). 

Parameters : 
--input (-i) : path to .nii image (string).
--output (-o) : path to write the resulting image (string).
--smin (-s) : Minimum path length (int)
--factor (-f) : factor for the geometric sequence of scales; scale_(n+1)=factor*scale_(n) (float)
--nb_scales (-n) : Number of scales (int)

--window (-w): (optional) Min and max values of the new intensity range of input image (2 int)
			    Convert input image to uint8. Intensities inferior to window_min become 0, intensities superior to window_max become 255; Linear transformation between window_min and window_max
--core (-c): (optional) nb of CPUs used for RPO computation (int)
--mask or -m : (optional) path to a mask for the input image (0 for the background and 1 for the foreground). The image type must be uint8.
--debug : (optional) Activation of a debug mode.

Usage Example : ./RORPO_multiscale --debug --input input_image.nii --output result.nii --smin 30 --factor 1.5 --nb_scales 2 --window 100 3250
Usage Example : ./RORPO_multiscale --debug -i input_image.nii -o result.nii -s 30 -f 1.5 -n 2 -w 100 3250
