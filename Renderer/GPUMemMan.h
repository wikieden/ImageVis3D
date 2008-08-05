/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2008 Scientific Computing and Imaging Institute,
   University of Utah.

   
   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/

/**
	\file		GPUMemMan.h
	\author		Jens Krueger
				SCI Institute
				University of Utah
	\version	1.0
	\date		August 2008
*/


#pragma once

#ifndef GPUMEMMAN_H
#define GPUMEMMAN_H

#ifdef WIN32
	#include <windows.h>
#endif

#if defined(macintosh) || (defined(__MACH__) && defined(__APPLE__))
	#include <GL/GL>
#else
	#include <GL/GL.h>
#endif


// TODO: realize include GL for other platforms

#include <deque>
#include <string>
#include "../IO/VolumeDataset.h"

typedef std::pair<unsigned int, VolumeDataset*> VolDataListElem;
typedef std::deque<VolDataListElem> VolDataList;
typedef VolDataList::iterator VolDataListIter;

struct SimpleTextureListElem {
	SimpleTextureListElem(unsigned int _iAccessCounter, GLuint _iGLID, std::string _strFilename) :
		iAccessCounter(_iAccessCounter), 
		iGLID(_iGLID), 
		strFilename(_strFilename)
	{}

	unsigned int	iAccessCounter;
	GLuint			iGLID;
	std::string		strFilename;
};
typedef std::deque<SimpleTextureListElem> SimpleTextureList;
typedef SimpleTextureList::iterator SimpleTextureListIter;

class MasterController;

class GPUMemMan {
	public:
		GPUMemMan(MasterController* masterController);
		virtual ~GPUMemMan();

		VolumeDataset* LoadDataset(const std::string& strFilename);
		void FreeDataset(VolumeDataset* pVolumeDataset);

		GLuint Load2DTextureFromFile(const std::string& strFilename);
		void FreeTexture(GLuint iTexture);

	private:
		VolDataList			m_vpVolumeDatasets;
		SimpleTextureList   m_vpSimpleTextures;
		MasterController*	m_MasterController;
};

#endif // GPUMEMMAN_H