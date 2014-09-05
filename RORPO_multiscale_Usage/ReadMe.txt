################ RORPO Multiscale ##############

Compute the RORPO multiscale from a .nii or .mha short image.

Parameters : 
--input (-i) : path to the .mha or .nii image. The image type must be short
--output (-o) : path to write the resulting image."
--smin (-s) : Minimum path length (int)"
--factor (-f) : factor for the geometric sequence of scales; scale_(n+1)=factor*scale_(n) (float)
--nb_scales (-n) : Number of scales (int)
--extension (-e) : 1 for nifti input image; 0 for mha input image (int).
	
--window (-w): (optional) Min and max values of the new intensity range of input image (2 int)
			    Convert input image to uint8. Intensities inferior to window_min become 0, intensities superior to window_max become 255; Linear transformation between window_min and window_max
--core (-c): (optional) nb of CPUs used for RPO computation (int)
--mask or -m : (optional) path to a mask for the input image (0 for the background and 1 for the foreground). The image type must be uint8.
--debug : (optional) Activation of a debug mode.

Usage Example : ./RORPO_multiscale -i input_image.mha -o result.mha -s 30 -f 1.5 -n 2 -e 0 -m 'Mask_image.mha'
Usage Example : ./RORPO_multiscale --debug --input input_image.nii -output result.nii -s 30 -f 1.5 -n 2 -e 1 --window 100 3250
