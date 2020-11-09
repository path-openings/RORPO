*************
Documentation
*************

RPO
===

.. py:function:: pyRORPO.RPO(image, scale, spacing=None, origin=None, nbCores=1, dilationSize=3, verbose=False, mask=None)

	Compute the 7 orientations of the Robust Path Opening.

	:param numpy.ndarray image: Input image
	:param int scale: Path length smaller than the real curvilinear structure length
	:param List[float] spacing: The "Spacing" is the size of a pixel in physical space along each dimension
	:param List[float] origin: The origin of the image - the geometric coordinates of the index (0,0,0)
	:param int nbCores: Number of CPUs used for RPO computation
	:param int dilationSize: Size of the dilation for the noise robustness step.
	:param bool verbose: Activation of a verbose mode
	:param numpy.ndarray mask: Path to a mask image (0 for the background and 1 for the foreground)

	:return: 7 Robust Path Opening responses of 7 orientations .
	:rtype: numpy.ndarray

.. code:: python

	import pyRORPO
	import nibabel

	image = nibabel.load(PATH_TO_IMAGE)
	input_image = image.get_fdata()

	responses = pyRORPO.RPO(input_image, scale=40, spacing=None, origin=None, nbCores=8, dilationSize=3, verbose=True, mask=None)


RORPO
=====

.. py:function:: pyRORPO.RORPO(image, scale, spacing=None, origin=None, nbCores=1, dilationSize=3, verbose=False, mask=None)

	Compute the 7 orientations of the Robust Path Opening.

	:param numpy.ndarray image: Input image
	:param int scale: Path length smaller than the real curvilinear structure length
	:param List[float] spacing: The "Spacing" is the size of a pixel in physical space along each dimension
	:param List[float] origin: The origin of the image - the geometric coordinates of the index (0,0,0)
	:param int nbCores: Number of CPUs used for RPO computation
	:param int dilationSize: Size of the dilation for the noise robustness step.
	:param bool verbose: Activation of a verbose mode
	:param numpy.ndarray mask: Path to a mask image (0 for the background and 1 for the foreground)

	:return: 7 Robust Path Opening responses of 7 orientations .
	:rtype: numpy.ndarray

.. code:: python

	import pyRORPO


RORPO


RORPO_multiscale
================

RORPO_multiscale