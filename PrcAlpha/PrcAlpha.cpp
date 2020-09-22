/***************************************************************************
*   Copyright (C) 2020 by Maxim Samsonov                                   *
*   maxim@samsonov.net                                                     *
****************************************************************************/

#include "stdafx.h"

#include "ImageExtractor.h"

void print_help()
{
  printf("Usage: podofoimgextract [inputfile] [outputdirectory]\n");
  printf("       [outputdirectory] shall exist");
  printf("\nPoDoFo Version: %s\n\n", PODOFO_VERSION_STRING);
}

int main( int argc, char* argv[] )
{
  char*    pszInput;
  char*    pszOutput;
  int      nNum     = 0;

  ImageExtractor extractor;

  if( argc != 3 )
  {
    print_help();
    exit( -1 );
  }

  pszInput  = argv[1];
  pszOutput = argv[2];

  try {
      extractor.Init( pszOutput );
	  extractor.Extract(pszInput);
  } catch( PdfError & e ) {
      fprintf( stderr, "Error: An error %i ocurred during processing the pdf file.\n", e.GetError() );
      e.PrintErrorMsg();
      return e.GetError();
  }

  nNum = extractor.GetNumImagesExtracted();

  printf("Extracted %i images successfully from the PDF file.\n", nNum );
  
  return 0;
}
