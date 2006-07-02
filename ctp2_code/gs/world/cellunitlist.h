//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ header
// Description  : Handling of a list of Units in the same cell
// Id           : $Id$
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
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Microsoft extensions removed.
// - MovementTypeLand added, as suggested by NelsonAndBronte.
// - Option added to report sync errors only once.
//
//----------------------------------------------------------------------------

#if defined(HAVE_PRAGMA_ONCE)
#pragma once
#endif

#ifndef __CELL_UNIT_LIST_H__
#define __CELL_UNIT_LIST_H__

//----------------------------------------------------------------------------
//
// Library imports
//
//----------------------------------------------------------------------------

// none

//----------------------------------------------------------------------------
//
// Exported names
//
//----------------------------------------------------------------------------

class CellUnitList;

#define k_CULF_IGNORES_ZOC          0x01
#define k_CULF_CANT_CAPTURE_CITY    0x02
#define k_CULF_CAN_SPACE_LAUNCH     0x04
#define k_CULF_CAN_SPACE_LAND       0x08
#define k_CULF_IN_SPACE             0x10
#define k_CULF_EXECUTED_THIS_TURN   0x80

//----------------------------------------------------------------------------
//
// Project imports
//
//----------------------------------------------------------------------------

#include "Unit.h"
#include "gstypes.h"

template <class T> class DynamicArray;

class CivArchive;

//----------------------------------------------------------------------------
//
// Declarations
//
//----------------------------------------------------------------------------

class CellUnitList
{
public:
	Unit m_array[k_MAX_ARMY_SIZE];

	uint32 m_moveIntersection;
	uint8 m_flags;

	
	sint32 m_nElements;

	CellUnitList() {m_nElements = 0;}
	CellUnitList(const sint32 size) {Assert(size == k_MAX_ARMY_SIZE); m_nElements = 0;}
	CellUnitList(const DynamicArray<Unit> &copyme);
	void Serialize(CivArchive &archive);

	virtual sint32 Insert(Unit id);
	sint32 Del(const Unit &delme);
	sint32 DelIndex(const sint32 index);

	Unit &operator [] (const sint32 i) {
		Assert(0 <= i);
		Assert(i < m_nElements);
		return m_array[i];
	}

	const Unit & operator [] (const sint32 i) const {
		Assert(0 <= i);
		Assert(i < m_nElements);
		return m_array[i];
	}

	const Unit &Get(const sint32 i) const {
		Assert(0 <= i);
		Assert(i < m_nElements);
		return m_array[i];
	}

	Unit &Access(const sint32 i) {
		Assert(0 <= i);
		Assert(i < m_nElements);
		return m_array[i];
	}
	
	void Clear() { m_nElements = 0; }

	const sint32 Num() const { if(!this) return 0; else return m_nElements; }
	void KillList(CAUSE_REMOVE_ARMY cause, PLAYER_INDEX killedBy);
	sint32 IsPresent(const Unit &u);

	bool CanEnter(const MapPoint &pos) const;
	bool HasWormholeProbe() const;
	virtual void GetPos(MapPoint &pos) const;
	virtual PLAYER_INDEX GetOwner() const;
	bool IsAtLeastOneMoveLand() const;
	bool IsAtLeastOneMoveWater() const;
	bool IsAtLeastOneMoveShallowWater() const;
	bool IsAtLeastOneMoveAir() const;
	bool IsAtLeastOneMoveSpace() const;
	bool IsAtLeastOneMoveMountain() const;
	bool IsEnemy(PLAYER_INDEX player) const;
	bool IsEnemy(Unit defender) const;
	bool IsEnemy(CellUnitList &defender) const;
	bool CanAtLeastOneCaptureCity() const;
	bool CanBeExpelled(); 

	bool IsVisible(PLAYER_INDEX player) const;
	bool GetTopVisibleUnitOfMoveType
	(
		PLAYER_INDEX const	looker,
		uint32 const		moveType,
		sint32 &			maxi,
		bool &				isResyncReported
	) const;
	Unit GetTopVisibleUnit
	(
		PLAYER_INDEX const	looker
	) const;
	
	bool CanBeSued() const;
	bool ExertsZOC() const;

    void ForceVisibleThisTurn(const PLAYER_INDEX to_me);
	double GetHPModifier();
	void DoVictoryEnslavement(sint32 origOwner);

	bool CanMoveIntoCell(const MapPoint &pos,
						 bool &zocViolation,
						 bool ignoreZoc,
						 bool &alliedCity);
	bool IsMovePointsEnough(const double cost) const;
	bool IsMovePointsEnough(const MapPoint &pos);
    bool GetMovementTypeAir() const; 
	bool GetMovementTypeLand() const; 
	bool CanBeCargoPodded() const;
	bool CanSpaceLand() const;
	bool CanSpaceLaunch() const;
	bool IsIgnoresZOC() const;

	void UpdateMoveIntersection();

	
	void ComputeStrength(double & attack, 
						 double & defense, 
						 double & ranged, 
						 sint16 & defend_unit_count, 
						 sint16 & ranged_unit_count,
						 double & land_bombard,
						 double & water_bombard,
						 double & air_bombard) const;

	
	double GetAverageHealthPercentage();
};

#endif
