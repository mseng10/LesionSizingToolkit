/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkHessian3DTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkHessianRecursiveGaussianImageFilter.h"
#include "itkSymmetricEigenAnalysisImageFilter.h" 
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

int main( int argc, char * argv [] )
{

  if( argc < 3 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " inputImage outputImageEigenValue1 [sigma]";
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 3;

  typedef signed short    InputPixelType;

  typedef itk::Image< InputPixelType,  Dimension >   InputImageType;

  typedef itk::ImageFileReader< InputImageType >     ReaderType;


  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName( argv[1] );

  try 
    {
    reader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  typedef itk::HessianRecursiveGaussianImageFilter< InputImageType >   HessianFilterType;

  typedef HessianFilterType::OutputImageType  HessianImageType;

  typedef  itk::FixedArray< double, Dimension >  EigenValueArrayType;
  typedef  itk::Image< EigenValueArrayType, Dimension > EigenValueImageType;

  typedef itk::SymmetricEigenAnalysisImageFilter< 
    HessianImageType, EigenValueImageType >     EigenAnalysisFilterType;


  HessianFilterType::Pointer hessianFilter = HessianFilterType::New();
  EigenAnalysisFilterType::Pointer eigenFilter = EigenAnalysisFilterType::New();

  hessianFilter->SetInput( reader->GetOutput() );
  eigenFilter->SetInput( hessianFilter->GetOutput() );

  typedef itk::ImageFileWriter< EigenValueImageType >    WriterType;
  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName( argv[2] );
  writer->SetInput( eigenFilter->GetOutput() );
  writer->UseCompressionOn();


  try 
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
