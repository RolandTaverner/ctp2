#pragma once

#ifndef __PLASMA_GEN_1_H__
#define __PLASMA_GEN_1_H__

#include "IMapGen.h"

class FaultGenerator : public IMapGenerator
{
private:
	ULONG m_refCount;

public:
	FaultGenerator() { m_refCount = 0; }

	
	STDMETHODIMP QueryInterface(REFIID, void **obj);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	STDMETHOD(Generate) (sint8 *map, sint32 width, sint32 height,
						 IC3Rand *randgen, 
						 const double *settings, sint32 numSettings);
};

#endif
