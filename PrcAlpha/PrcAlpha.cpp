// PrcAlpha.cpp : Defines the entry point for the console application.
//

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


  DWORD  dwGFA = GetFileAttributesA(pszOutput);

  if (dwGFA == INVALID_FILE_ATTRIBUTES) {
	  if (!CreateDirectoryA(pszOutput, NULL)) {
		  fprintf(stderr, "Error: %s does not exist and CreateDirectory failed.\n\n", pszOutput);
		  exit(-1);
	  }
  } else if (!(dwGFA & FILE_ATTRIBUTE_DIRECTORY)) {
	  fprintf(stderr, "Error: %s exists but is not a directory.\n\n", pszOutput);
	  exit(-1);
  }

  try {
  //    extractor.Init( pszInput, pszOutput, &nNum );
	  extractor.Extract(pszInput, pszOutput);
  } catch( PdfError & e ) {
      fprintf( stderr, "Error: An error %i ocurred during processing the pdf file.\n", e.GetError() );
      e.PrintErrorMsg();
      return e.GetError();
  }

  nNum = extractor.GetNumImagesExtracted();

  printf("Extracted %i images successfully from the PDF file.\n", nNum );
  
  return 0;
}
