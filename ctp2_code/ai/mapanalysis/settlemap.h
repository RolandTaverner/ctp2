//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ header file
// Description  : declarations for the SettleMap class
//
//----------------------------------------------------------------------------
//
// Disclaimer
//
// THIS FILE IS NOT GENERATED OR SUPPORTED BY ACTIVISION.
//
// This material has been developed at apolyton.net by the Apolyton CtP2 
// Source Code Project. Contact the authors at ctp2source@apolyton.net.
//
//----------------------------------------------------------------------------
//
// Compiler flags
// 
// _MSC_VER		
// - Compiler version (for the Microsoft C++ compiler only)
//
// Note: For the blocks with _MSC_VER preprocessor directives, the following
//       is implied: the (_MSC_VER) preprocessor directive lines and the blocks 
//       between #else and #endif are modified Apolyton code. The blocks that
//       are active for _MSC_VER value 1200 are the original Activision code.
//
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Marked MS version specific code.
//
//----------------------------------------------------------------------------

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif

#ifndef __SETTLE_MAP_H__
#define __SETTLE_MAP_H__

#include "c3.h"
#include "c3debugstl.h"

#include "Unit.h"
#include "CivArchive.h"
#include "Bit_Table.h"
#include "MapPoint.h"
#include "MapGrid.h"

#if defined(_MSC_VER) && (_MSC_VER < 1300)
#include <list-fixed>
#else
#include <list>
#endif

const sint16 k_minimum_settle_city_size = 2;


const sint32 k_targets_per_continent = 25;

class SettleMap {
public:

	static SettleMap s_settleMap;

	
	
	
	void Cleanup();

	
	void Initialize();

	
	void HandleCityGrowth(const Unit & city);

	struct SettleTarget
	{
		SettleTarget() { m_value = 0.0;	}
		bool operator<(const SettleTarget & rval) const { return ( m_value < rval.m_value ); }
		bool operator>(const SettleTarget & rval) const { return ( m_value > rval.m_value ); }
		double m_value;
		MapPoint m_pos;
	};

#ifdef _DEBUG
	typedef std::list<SettleTarget, dbgallocator< SettleTarget > > SettleTargetList;
#else
	typedef std::list<SettleTarget > SettleTargetList;
#endif

	
	void GetSettleTargets(const PLAYER_INDEX &player, 
						  const bool & settle_water,
						  SettleMap::SettleTargetList & targets) const;
	
	
	bool CanSettlePos(const MapPoint & rc_pos) const;

	
	void SetCanSettlePos(const MapPoint & rc_pos, const bool can_settle);

	
	double GetValue(const MapPoint & rc_pos) const;

private:	
	
	SettleMap();

	
	double ComputeSettleValue(const MapPoint & pos) const;

    MapGrid<double> m_settleValues;
	Bit_Table m_invalidCells;
	
	
};

#endif 




