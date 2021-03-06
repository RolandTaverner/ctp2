#ifdef HAVE_PRAGMA_ONCE
#pragma once
#endif
#ifndef _TERRIMPROVE_H_
#define _TERRIMPROVE_H_

#include "gs/gameobj/ID.h"
#include "gs/gameobj/TerrImproveData.h"

template <class T> class DynamicArray;

class TerrainImprovement : public ID {
public:
	TerrainImprovement () : ID() { return; } ;
	TerrainImprovement (sint32 val) : ID (val) { return; };
	TerrainImprovement (uint32 val) : ID (val) { return; };

	TerrainImprovementData *operator -> () const { return AccessData(); }

	bool IsValid();

	void KillImprovement();
	void Kill() { KillImprovement(); }
	void RemoveAllReferences();

	//const TerrainImprovementRecord * GetDBRec() const;  //added by E although the game uses installations?
	const TerrainImprovementData *GetData() const;
	TerrainImprovementData *AccessData() const;

	sint32 GetOwner() const { return GetData()->GetOwner(); }
	sint32 GetType() const { return GetData()->GetType(); }
    sint32 GetInstType() const { return 0; }
	MapPoint GetLocation() const { return GetData()->GetLocation(); }
	sint32 GetCompletion() const { return GetData()->GetCompletion(); }
	sint32 PercentComplete() const { return GetData()->PercentComplete(); }
//	sint32 GetBonusProductionExport() const { return GetData()->GetBonusProductionExport(); } //EMOD 4-5-2006
	sint32 GetMaterialCost() const { return GetData()->GetMaterialCost(); }
	bool IsBuilding() const { return GetData()->IsBuilding(); }

	BOOL Complete(void) { return AccessData()->Complete(); }
	void AddTurn();
	void StartBuilding() { AccessData()->StartBuilding(); }

};

#endif
