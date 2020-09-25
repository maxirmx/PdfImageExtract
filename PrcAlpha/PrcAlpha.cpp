/******************************************************************************
Copyright (C) 2020 by Maxim Samsonov
maxim@samsonov.net

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom
the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

*******************************************************************************/

#include "stdafx.h"

#include "ImageExtractor.h"

void print_help()
{
  cout << "Usage: podofoimgextract [inputfile] [outputdirectory]" << endl <<
          "       [outputdirectory] shall exist" << endl  << 
          "PoDoFo Version: " << PODOFO_VERSION_STRING << endl << endl;
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
      extractor.Init( pszOutput, pszInput);
	  extractor.Extract();
  } catch( PdfError & e ) {
	  cerr << "Error: An error " << e.GetError() << " ocurred during processing the pdf file." << endl; 
      e.PrintErrorMsg();
      return e.GetError();
  }

  nNum = extractor.GetNumImagesExtracted();

  cout << "Extracted " << nNum << " images successfully from the PDF file.";
  
  return 0;
}
