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

#ifndef _IMAGE_EXTRACTOR_H_
#define _IMAGE_EXTRACTOR_H_

using namespace PoDoFo;

#ifndef MAX_PATH
#define MAX_PATH 512
#endif // MAX_PATH

/** This class uses the PoDoFo lib to parse a PDF file and to write all 
*   in  ordered by page number 
**/

class ImageExtractor {
 public:
    ImageExtractor();
    virtual ~ImageExtractor();

    void Init(const char* pszInputFile, const char* pszOutput);
	void Extract();

    /**
     * \returns the number of succesfully extracted images
     */
    inline int GetNumImagesExtracted() const;

 private:

	 /** Opens file to extract the image to
	 *  \param sExt - extension
	 *  \param nPage page number
	 *  \param nCount image number in the page
	 */

	 ofstream* OpenFStream(string sExt, unsigned int nPage, unsigned int nCount);

    /** Extracts the image form the given PdfObject
     *  which has to be an XObject with Subtype "Image"
     *  \param pObject a handle to a PDF object
     *  \param bJpeg if true extract as a jpeg, otherwise create a ppm
	 *  \param nPage page number
	 *  \param nCount image number in the page
     */
	 void ExtractImage( PoDoFo::PdfObject* pObject, bool bDecode, string sExt, unsigned int nPage, unsigned int nCount);

    /** This function checks wether a file with the 
     *  given filename does exist.
     *  \returns true if the file exists otherwise false
     */
    bool    FileExists( const char* pszFilename );

 private:
    string   m_sOutputDirectory;
	string   m_sInputFile;
	string   m_sFName;
	unsigned int m_nSuccess;
    unsigned int m_nCount;

};

inline int ImageExtractor::GetNumImagesExtracted() const
{
    return m_nSuccess;
}

#endif // _IMAGE_EXTRACTOR_H_
