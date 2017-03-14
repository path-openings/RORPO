/* Copyright (C) 2014 Odyssee Merveille
odyssee.merveille@gmail.com

    This software is a computer program whose purpose is to compute RORPO.
    This software is governed by the CeCILL-B license under French law and
    abiding by the rules of distribution of free software.  You can  use,
    modify and/ or redistribute the software under the terms of the CeCILL-B
    license as circulated by CEA, CNRS and INRIA at the following URL
    "http://www.cecill.info".

    As a counterpart to the access to the source code and  rights to copy,
    modify and redistribute granted by the license, users are provided only
    with a limited warranty  and the software's author,  the holder of the
    economic rights,  and the successive licensors  have only  limited
    liability.

    In this respect, the user's attention is drawn to the risks associated
    with loading,  using,  modifying and/or developing or reproducing the
    software by the user in light of its specific status of free software,
    that may mean  that it is complicated to manipulate,  and  that  also
    therefore means  that it is reserved for developers  and  experienced
    professionals having in-depth computer knowledge. Users are therefore
    encouraged to load and test the software's suitability as regards their
    requirements in conditions enabling the security of their systems and/or
    data to be ensured and,  more generally, to use and operate it in the
    same conditions as regards security.

    The fact that you are presently reading this means that you have had
    knowledge of the CeCILL-B license and that you accept its terms.
*/

#ifndef Images_IO_VTK_INCLUDED
#define Images_IO_VTK_INCLUDED

#include <string>
#include <vector>
#include <vtkImageData.h>
#include <vtkMetaImageReader.h>
#include <vtkSmartPointer.h>
#include "vtkImageWriter.h"
#include "vtkImageImport.h"
#include "vtkImageExport.h"
#include "vtkMetaImageWriter.h"

#include "Image.hpp"
#include <typeinfo>


// ############################# MHA Image ##############################

template<typename PixelType>
Image3D<PixelType> Read_Mha_Image(std::string image_path){
	
	//Reader
	vtkSmartPointer<vtkMetaImageReader> reader =
	vtkSmartPointer<vtkMetaImageReader>::New();
	reader->SetFileName(image_path.c_str());
	reader->Update();

	//ImageData
	vtkImageData* metaimage = reader->GetOutput();
	
    Image3D<PixelType> I(metaimage->GetDimensions()[0],metaimage->GetDimensions()[1],metaimage->GetDimensions()[1]);
	I.Add_data_from_pointer((PixelType*)(metaimage->GetScalarPointer()));
	
	return I;
	
}


template<typename PixelType>
void Write_Mha_Image(Image3D<PixelType> I, std::string writing_path){
		
		vtkSmartPointer<vtkImageData> vtkimage = vtkSmartPointer<vtkImageData>::New();
        vtkimage->SetExtent(0, I.Dimx()-1, 0, I.Dimy()-1, 0, I.Dimz()-1);
		vtkimage->SetSpacing(1,1,1);
		vtkimage->SetOrigin(0,0,0);
		
		
		if (typeid(PixelType)==typeid(short)){
			vtkimage->AllocateScalars(VTK_SHORT, 1);
		}
		else if (typeid(PixelType)==typeid(unsigned short)){
			vtkimage->AllocateScalars(VTK_UNSIGNED_SHORT, 1);
		}
		else if (typeid(PixelType)==typeid(unsigned char)){
			vtkimage->AllocateScalars(VTK_UNSIGNED_CHAR, 1);
		}
		else if (typeid(PixelType)==typeid(int32_t)){
			vtkimage->AllocateScalars(VTK_INT, 1);
		}
		else if (typeid(PixelType)==typeid(float)){
			vtkimage->AllocateScalars(VTK_FLOAT, 1);
		}
		else {
			std::cerr<<"Write Image : Pixel Type not supported"<<std::endl;
		}
		
		PixelType* usp=static_cast<PixelType*>(vtkimage->GetScalarPointer());
		vtkSmartPointer<vtkMetaImageWriter> writer = vtkSmartPointer<vtkMetaImageWriter>::New();
		writer->SetCompression(false);		
		writer->SetInputData(vtkimage);
        memcpy(usp,I.GetPointer(),sizeof(PixelType)*I.Dimx()*I.Dimy()*I.Dimz());
		writer->SetFileName(writing_path.c_str());
		writer->Write();
	
#endif // Images_IO_VTK_INCLUDED
	
