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
  \file    GPUMemMan.h
  \author    Jens Krueger
        SCI Institute
        University of Utah
  \date    August 2008
*/

#pragma once

#ifndef GPUMEMMAN_H
#define GPUMEMMAN_H

#include <deque>
#include <string>
#include "../AbstrRenderer.h"
#include "../GLTexture1D.h"
#include "../GLTexture2D.h"
#include "../../IO/VolumeDataset.h"
#include "../../IO/TransferFunction1D.h"
#include "../../IO/TransferFunction2D.h"

typedef std::deque< AbstrRenderer* > AbstrRendererList;
typedef AbstrRendererList::iterator AbstrRendererListIter;

// volume datasets
struct VolDataListElem {
  VolDataListElem(VolumeDataset* _pVolumeDataset, AbstrRenderer* pUser) :
    pVolumeDataset(_pVolumeDataset) 
  {
    qpUser.push_back(pUser);
  }

  VolumeDataset*  pVolumeDataset;
  AbstrRendererList qpUser;
};
typedef std::deque<VolDataListElem> VolDataList;
typedef VolDataList::iterator VolDataListIter;

// simple textures
struct SimpleTextureListElem {
  SimpleTextureListElem(unsigned int _iAccessCounter, GLTexture2D* _pTexture, std::string _strFilename) :
    iAccessCounter(_iAccessCounter), 
    pTexture(_pTexture), 
    strFilename(_strFilename)
  {}

  unsigned int  iAccessCounter;
  GLTexture2D*  pTexture;
  std::string    strFilename;
};
typedef std::deque<SimpleTextureListElem> SimpleTextureList;
typedef SimpleTextureList::iterator SimpleTextureListIter;

// 1D transfer functions
struct Trans1DListElem {
  Trans1DListElem(TransferFunction1D* _pTransferFunction1D, GLTexture1D* _pTexture, AbstrRenderer* pUser) :
    pTransferFunction1D(_pTransferFunction1D),
    pTexture(_pTexture)
  {
    qpUser.push_back(pUser);
  }

  TransferFunction1D*  pTransferFunction1D;
  GLTexture1D*    pTexture;
  AbstrRendererList  qpUser;
};
typedef std::deque<Trans1DListElem> Trans1DList;
typedef Trans1DList::iterator Trans1DListIter;

// 2D transfer functions
struct Trans2DListElem {
  Trans2DListElem(TransferFunction2D* _pTransferFunction2D, GLTexture2D* _pTexture, AbstrRenderer* pUser) :
    pTransferFunction2D(_pTransferFunction2D),
    pTexture(_pTexture)
  {
    qpUser.push_back(pUser);
  }

  TransferFunction2D*  pTransferFunction2D;
  GLTexture2D*    pTexture;
  AbstrRendererList  qpUser;
};
typedef std::deque<Trans2DListElem> Trans2DList;
typedef Trans2DList::iterator Trans2DListIter;

class MasterController;

class GPUMemMan {
  public:
    GPUMemMan(MasterController* masterController);
    virtual ~GPUMemMan();

    VolumeDataset* LoadDataset(const std::string& strFilename, AbstrRenderer* requester);
    void FreeDataset(VolumeDataset* pVolumeDataset, AbstrRenderer* requester);

    void Changed1DTrans(AbstrRenderer* requester, TransferFunction1D* pTransferFunction1D);
    void GetEmpty1DTrans(size_t iSize, AbstrRenderer* requester, TransferFunction1D** transferFunc, GLTexture1D** tex);
    void Get1DTransFromFile(const std::string& strFilename, AbstrRenderer* requester, TransferFunction1D** transferFunc, GLTexture1D** tex);
    GLTexture1D* Access1DTrans(TransferFunction1D* transferFunc, AbstrRenderer* requester);
    void Free1DTrans(TransferFunction1D* transferFunc, AbstrRenderer* requester);

    void Changed2DTrans(AbstrRenderer* requester, TransferFunction2D* pTransferFunction2D);
    void GetEmpty2DTrans(const VECTOR2<size_t>& iSize, AbstrRenderer* requester, TransferFunction2D** transferFunc, GLTexture2D** tex);
    void Get2DTransFromFile(const std::string& strFilename, AbstrRenderer* requester, TransferFunction2D** transferFunc, GLTexture2D** tex);
    GLTexture2D* Access2DTrans(TransferFunction2D* transferFunc, AbstrRenderer* requester);
    void Free2DTrans(TransferFunction2D* transferFunc, AbstrRenderer* requester);

    GLTexture2D* Load2DTextureFromFile(const std::string& strFilename);
    void FreeTexture(GLTexture2D* pTexture);

  private:
    VolDataList      m_vpVolumeDatasets;
    SimpleTextureList   m_vpSimpleTextures;
    Trans1DList      m_vpTrans1DList;
    Trans2DList      m_vpTrans2DList;
    MasterController*  m_MasterController;
};

#endif // GPUMEMMAN_H