/***************************************************************************
*   Copyright (C) 2020 by Maxim Samsonov                                   *
*   maxim@samsonov.net                                                     *
****************************************************************************/


/***************************************************************************
*   Copyright (C) 2005 by Dominik Seichter                                *
*   domseichter@web.de                                                    *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#include "stdafx.h"
#include "ImageExtractor.h"

#ifdef _MSC_VER
#define snprintf _snprintf_s
#endif

ImageExtractor::ImageExtractor()
    : m_pszOutputDirectory( NULL ), m_nSuccess( 0 ), m_nCount( 0 )
{

}

ImageExtractor::~ImageExtractor()
{
}

void ImageExtractor::Init(const char* pszOutput)
{
	if (!pszOutput)
	{
		PODOFO_RAISE_ERROR(ePdfError_InvalidHandle);
	}

	m_pszOutputDirectory = const_cast<char*>(pszOutput);


	DWORD  dwGFA = GetFileAttributesA(pszOutput);

	if (dwGFA == INVALID_FILE_ATTRIBUTES) {
		if (!CreateDirectoryA(pszOutput, NULL)) {
			cerr << "Error: " << pszOutput << " does not exist and CreateDirectory failed." << endl << endl;
			exit(-1);
		}
	}
	else if (!(dwGFA & FILE_ATTRIBUTE_DIRECTORY)) {
		cerr << "Error: " << pszOutput << " exists but is not a directory." << endl << endl;
		exit(-1);
	}

}


void ImageExtractor::Extract(const char* pszInput)
{

	PdfMemDocument document(pszInput);
	int nPages = document.GetPageCount();

	const string kwdDo("Do");
	const PdfName nameXObject("XObject");

	for (int i = 0; i < nPages; i++) {

		cout << "Page " << setw(6) << (i + 1) << endl << flush;

		PdfPage* pPage = document.GetPage(i);
		PdfContentsTokenizer tokenizer(pPage);

		EPdfContentsType type;
		const char * kwText;
		PdfVariant var;
		bool bReadToken;

		PdfObject* pObj;
		PdfObject* pObjType;
		PdfObject* pObjSubType;
		PdfObject* pFilter;

		string theLastNameReadable("undefined");
		PdfVariant theLastName;
// --------------------------------------------------------------------------------------------------------------
// https://www.adobe.com/content/dam/acom/en/devnet/pdf/pdfs/PDF32000_2008.pdf 
// p. 201 and beyond
// We are looking for <XObject name> /Do  sequences
// --------------------------------------------------------------------------------------------------------------
		while ((bReadToken = tokenizer.ReadNext(type, kwText, var)))
		{
			switch (type) {
				case ePdfContentsType_Keyword: /* The token is a PDF keyword. */
					if (kwdDo == kwText) {
// Debug output			cout << "Do (XObject:\"" << setw(6) << theLastNameReadable << "\")" << endl;
						pObj = pPage->GetFromResources(nameXObject, theLastName.GetName());
						if (pObj && pObj->IsDictionary()) {
// Debug output				cout << "Dictionary ...";
							pObjType = pObj->GetDictionary().GetKey(PdfName::KeyType);
							pObjSubType = pObj->GetDictionary().GetKey(PdfName::KeySubtype);

							if ((pObjType && pObjType->IsName() && (pObjType->GetName().GetName() == "XObject")) ||
								(pObjSubType && pObjSubType->IsName() && (pObjSubType->GetName().GetName() == "Image")))
							{
// Debug output				    cout << " " << pObjType->GetName().GetName() << " and " << pObjSubType->GetName().GetName() << " ...";

								pFilter = pObj->GetDictionary().GetKey(PdfName::KeyFilter);
								if (pFilter && pFilter->IsArray() && pFilter->GetArray().GetSize() == 1 &&
									pFilter->GetArray()[0].IsName() && (pFilter->GetArray()[0].GetName().GetName() == "DCTDecode"))
									pFilter = &pFilter->GetArray()[0];


								if (pFilter && pFilter->IsName() && (pFilter->GetName().GetName() == "DCTDecode"))
								{
									// The only filter is JPEG -> create a JPEG file
									ExtractImage(pObj, true);
								}
								else
								{
									ExtractImage(pObj, false);
								}

								document.FreeObjectMemory(pObj);
							}
						}
					}
					break;
				case ePdfContentsType_Variant: /* The token is a PDF variant. A variant is usually a parameter to a keyword */
				// Save the last Name token that can be a parameter for the following Do command
					if (var.IsName()) {
						theLastName = var;
						theLastName.ToString(theLastNameReadable);
// Debug output			cout << setw(6) << theLastNameReadable << "\" ... ";
					}
					break;
				case ePdfContentsType_ImageData: /* The "token" is raw inline image data found between ID and EI tags (see PDF ref section 4.8.6) */
					cout << "Raw inline image data that we do not support !!!" << endl;
					break;
			};

		}
	}
	cout << "Done!" << endl;
}



void ImageExtractor::ExtractImage( PdfObject* pObject, bool bJpeg )
{
    FILE*       hFile        = NULL;
    const char* pszExtension = bJpeg ? "jpg" : "ppm"; 

    // Do not overwrite existing files:
    do {
        snprintf( m_szBuffer, MAX_PATH, "%s/pdfimage_%04i.%s", m_pszOutputDirectory, m_nCount++, pszExtension );
    } while( FileExists( m_szBuffer ) );

    hFile = fopen( m_szBuffer, "wb" );  
    if( !hFile )
    {
        PODOFO_RAISE_ERROR( ePdfError_InvalidHandle );
    }

    printf("-> Writing image object %s to the file: %s\n", pObject->Reference().ToString().c_str(), m_szBuffer);

    if( bJpeg ) 
    {
        PdfMemStream* pStream = dynamic_cast<PdfMemStream*>(pObject->GetStream());
        fwrite( pStream->Get(), pStream->GetLength(), sizeof(char), hFile );
    }
    else
    {
        //long lBitsPerComponent = pObject->GetDictionary().GetKey( PdfName("BitsPerComponent" ) )->GetNumber();
        // TODO: Handle colorspaces

        // Create a ppm image
        const char* pszPpmHeader = "P6\n# Image extracted by PoDoFo\n%" PDF_FORMAT_INT64 " %" PDF_FORMAT_INT64 "\n%li\n";
        
        
        fprintf( hFile, pszPpmHeader, 
                 pObject->GetDictionary().GetKey( PdfName("Width" ) )->GetNumber(),
                 pObject->GetDictionary().GetKey( PdfName("Height" ) )->GetNumber(),
                 255 );
                 
        char*    pBuffer;
        pdf_long lLen;
        pObject->GetStream()->GetFilteredCopy( &pBuffer, &lLen );
        fwrite( pBuffer, lLen, sizeof(char), hFile );
        free( pBuffer );
    }

    fclose( hFile );

    ++m_nSuccess;
}

bool ImageExtractor::FileExists( const char* pszFilename )
{
    bool result = true;
    
    // if there is an error, it's probably because the file doesn't yet exist
    struct stat	stBuf;
    if ( stat( pszFilename, &stBuf ) == -1 )	result = false;

    return result;
}
