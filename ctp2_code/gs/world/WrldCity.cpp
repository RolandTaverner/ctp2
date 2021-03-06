//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ source
// Description  : World city handling
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
// - None
//
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - None
//
//----------------------------------------------------------------------------

#include "ctp/c3.h"

#include "gs/utility/Globals.h"
#include "gs/gameobj/XY_Coordinates.h"
#include "gs/world/World.h"
#include "gs/utility/QuadTree.h"
#include "gs/world/Cell.h"
#include "gs/gameobj/installation.h"
#include "gs/gameobj/installationtree.h"
#include "gs/gameobj/Player.h"
#include "gs/world/cellunitlist.h"
#include "gs/utility/UnitDynArr.h"
#include "net/general/network.h"
#include "net/general/net_info.h"
#include "gs/gameobj/TerrImprove.h"

extern QuadTree<Unit> *g_theUnitTree;
extern InstallationQuadTree  *g_theInstallationTree;
extern Player **g_player;

bool World::HasCity(const MapPoint &pos) const
{
	if(pos.IsValid())
		return GetCell(pos)->GetCity().m_id != (0);
	else
		return false;
}

void World::MoveUnitToCitySlot(Unit newCity, const MapPoint &pos)

{
	Assert(FALSE);
	Cell *c = GetCell(pos);

   sint32 r = c->m_unit_army->Del(newCity);

   Assert(r);
   Assert(c->GetCity().m_id == (0));

   c->SetCity(newCity);
}

void World::CityRadiusFunc(const MapPoint &pos)
{

	switch(m_radiusOp) {
		case WORLD_RADIUS_OP_SET_RADIUS:
#if 0

			if(GetCell(pos)->GetOwner() < 0) {
				GetCell(pos)->SetOwner(m_insertCityOwner);
				if(g_network.IsHost()) {
					uint32 packpos = g_network.PackedPos(pos);
					g_network.Enqueue(new NetInfo(NET_INFO_CODE_CELL_OWNER,
												  packpos, m_insertCityOwner));
				}
			} else if(!IsInsideCityRadiusOfPlayerOtherThan(pos, m_insertCityOwner,
														   Unit(m_ignoreCity))) {
				GetCell(pos)->SetOwner((sint8)m_insertCityOwner);
				if(g_network.IsHost()) {
					uint32 packpos = g_network.PackedPos(pos);
					g_network.Enqueue(new NetInfo(NET_INFO_CODE_CELL_OWNER,
												  packpos, m_insertCityOwner));
				}
			}

			m_radiusOp = WORLD_RADIUS_OP_SET_RADIUS;
			GetCell(pos)->m_env |= k_BIT_ENV_CITY_RADIUS;
#endif
			break;
		case WORLD_RADIUS_OP_IRRIGATION_CHECK:
		{
			if(!m_tempIrrigation) {
				Cell *theCell = GetCell(pos);
				if(((theCell->m_env &
					 (k_MASK_ENV_IRRIGATION | k_MASK_ENV_RIV_CUR)) != 0) ||
				   IsWater(pos)) {
					m_tempIrrigation = TRUE;
				} else if(GetCell(pos)->GetNumImprovements()) {
#if 0   // useless code
					sint32 i;
					for(i = theCell->GetNumImprovements() - 1; i >= 0; i--) {
						TERRAIN_IMPROVEMENT type = theCell->AccessImprovement(i).GetType();






					}
#endif
				}
			}

			break;
		}
		case WORLD_RADIUS_OP_CITY_SEARCH:
			if(GetCell(pos)->GetCity().m_id != 0 &&
			   GetCell(pos)->GetCity().m_id != m_ignoreCity &&
			   GetCell(pos)->GetCity().GetOwner() != m_cityOwnerCheck) {
				m_isInsideRadius = TRUE;
			}
			break;
		default:
			Assert(false);
			break;
	}
}

bool World::InsertCity(const MapPoint &pos, Unit u)
{
	g_theUnitTree->Insert(u);

	UnitDynamicArray revealed;
	u.DoVision(revealed);

	Cell *  c   = GetCell(pos);
	if (c->HasCity()) {
		return false;
	} else {
		c->SetCity(u);

		if(!g_network.IsClient() || g_network.ReadyToStart()) {
			PLAYER_INDEX owner = u.GetOwner();
			m_radiusOp = WORLD_RADIUS_OP_SET_RADIUS;
			m_ignoreCity = u.m_id;
			m_insertCityOwner = owner;

			c->SetOwner(owner);
			if(g_network.IsHost()) {
				uint32 packpos = g_network.PackedPos(pos);
				g_network.Enqueue(new NetInfo(NET_INFO_CODE_CELL_OWNER,
											  packpos, owner));
			}

			CityRadiusIterator(pos, this);

		}

		DPRINTF(k_DBG_GAMESTATE, ("World: Inserted city 0x%x at (%d,%d)\n",
			    (uint32)u, pos.x, pos.y));
		return true;
	}
}

Unit World::GetCity(const MapPoint &pos)

{
	return GetCell(pos)->GetCity();
}

void World::CityXOff(const MapPoint &pos, sint32 &startx, sint32 &endx)

{
    startx = pos.x - 2;
    endx = pos.x + 2;
}

void World::CityYOff(sint32 x, const MapPoint &pos, sint32 &starty, sint32 &endy)

{
    int c;

    c = pos.x - x;

    switch (c) {
    case 2:
        starty = pos.y+1;
        endy = pos.y+3;
        break;
    case 1:
        starty = pos.y-1;
        endy = pos.y+3;
        break;
    case 0:
        starty = pos.y-2;
        endy = pos.y+2;
        break;
    case -1:
        starty = pos.y-3;
        endy = pos.y+1;
        break;
    case -2:
        starty = pos.y-3;
        endy = pos.y-1;
        break;
    default:
        Assert(FALSE);
    }

}

extern void WhackScreen();

void World::SetColor(const MapPoint &pos, sint32 c)
{

#ifdef _DEBUG
#ifdef CELL_COLOR
    GetCell(pos.x,pos.y)->m_color = c;

#endif
#endif
}

sint32 World::GetFoodProduced(const MapPoint &pos) const
{
	return GetCell(pos)->GetFoodProduced();
}

sint32 World::GetShieldsProduced(const MapPoint &pos) const
{
	return GetCell(pos)->GetShieldsProduced();
}

void World::GamestateDebug()

{
    sint32 x, y;

	for (x=0; x<m_size.x; x++) {
		for (y=0; y<m_size.y; y++) {
#ifdef CELL_COLOR


#endif
		}
	}
}

#ifdef CELL_COLOR
















#endif
