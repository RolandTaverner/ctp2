//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ source
// Description  : Player Happiness
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
// - Replaced old const database by new one. (5-Aug-2007 Martin G�hmann)
//
//----------------------------------------------------------------------------

#include "ctp/c3.h"
#include "gs/utility/Globals.h"

#include "gs/gameobj/PlayHap.h"
#include "gs/gameobj/Player.h"
#include "gs/gameobj/XY_Coordinates.h"
#include "gs/world/World.h"
#include "gs/newdb/ConstRecord.h"
#include "ui/aui_common/aui.h"
#include "ui/aui_common/aui_surface.h"
#include "ui/aui_utils/primitives.h"
#include "ui/aui_common/aui_window.h"
#include "ui/interface/debugwindow.h"
#include "gs/gameobj/Unit.h"
#include "robot/aibackdoor/dynarr.h"
#include "gs/utility/UnitDynArr.h"
#include "gs/world/Cell.h"
#include "gs/gameobj/installationtree.h"

#include "gs/newdb/GovernmentRecord.h"
#include "gs/gameobj/terrainutil.h"

#include "robot/aibackdoor/civarchive.h"
#include "net/general/network.h"
#include "gs/newdb/GovernmentRecord.h"

#include "gs/gameobj/Army.h"

extern DebugWindow *g_debugWindow;

PlayerHappiness::PlayerHappiness()

{
    m_pad = 0;

    m_workday_per_person = g_theConstDB->Get(0)->GetBaseWorkday();
    m_unitless_workday = 0;

    m_rations_per_person = g_theConstDB->Get(0)->GetBaseRations();
    m_unitless_rations = 0;

    m_wages_per_person = g_theConstDB->Get(0)->GetBaseWages();
    m_unitless_wages = 0;

    m_raw_overseas_defeat = 0.0;
    m_raw_home_defeat =0.0;

    m_overseas_defeat = 0.0;
    m_home_defeat = 0.0;

    m_dist_overseas = 0.0;
    m_overseas = 0.0;

	m_warDiscontent = 0;
}

void PlayerHappiness::CalcPeaceMovement(Player *p,
    DynamicArray<Army> &all_armies, UnitDynamicArray &all_cities)

{
    m_raw_overseas_defeat =  ((1.0 - p->GetOverseasDefeatDecay()) * m_raw_overseas_defeat + p->GetOverseasDefeatCoef() * p->GetUnitsLostOverseas());
    m_raw_home_defeat = ((1.0 - p->GetHomeDefeatDecay()) * m_raw_home_defeat + p->GetHomeDefeatCoef() * p->GetUnitsLostHome());

    m_overseas_defeat = m_raw_overseas_defeat;
    m_home_defeat = m_raw_home_defeat;

    Unit nearest;
    sint32 n, m, i, j;
    MapPoint apos, cpos, diff;
    double distance;
    double d;
    double radius = p->GetAtHomeRadius();

    n = all_armies.Num();
    m = all_cities.Num();
    m_dist_overseas = 0;
    for (i=0; i<n; i++) {

        all_armies[i].GetPos(apos);


        if (p->GetNearestCity(apos, nearest, distance) == FALSE) {
            m_overseas = 0.0;
            m_overseas_defeat = 0.0;
            m_home_defeat = 0.0;
            return;
        }

        if (distance <= radius) {
            continue;
        }

        d = distance - radius;

        Assert (0 < d);
        m = all_armies[i].Num();
        for (j=0; j<m; j++) {
             if (!all_armies[i][j].IsContentOverseas()) {
                  m_dist_overseas += double(d);
             }
        }
    }

    m_overseas = p->GetOverseasCoef() * m_dist_overseas;

	sint32 militaryUnitsOutOfCities = 0;
	n = all_armies.Num();
	for(i = 0; i < n; i++) {
		MapPoint pos;
		all_armies[i].GetPos(pos);
		Cell *cell = g_theWorld->GetCell(pos);
		if(cell->GetCity().m_id != (0))
			continue;
		if (terrainutil_HasFort(pos) && cell->GetOwner() == p->m_owner)
			continue;

		for(j = 0; j < all_armies[i].Num(); j++) {
			if(all_armies[i][j].GetAttack() > 0) {
				militaryUnitsOutOfCities++;
			}
		}
	}

	if(p->IsRobot() &&
	   !(g_network.IsClient() && g_network.IsLocalPlayer(p->m_owner))) {
		militaryUnitsOutOfCities = 0;
	}

	m_warDiscontent = 0;
	const GovernmentRecord *grec = g_theGovernmentDB->Get(p->m_government_type);
	if(militaryUnitsOutOfCities > grec->GetWarDiscontentMaxUnits()) {
		sint32 diff = militaryUnitsOutOfCities - grec->GetWarDiscontentMaxUnits();
		double total = diff * grec->GetWarDiscontentPerUnit();
		double perCity;
		if(all_cities.Num() < 1)
			perCity = 0;
		else
			perCity = total / all_cities.Num();

		m_warDiscontent = perCity;
	}
}

void PlayerHappiness::SetWorkdayLevel (sint32 h)
{
    m_workday_per_person = - h * g_theConstDB->Get(0)->GetUnitWorkday() + g_theConstDB->Get(0)->GetBaseWorkday();
    m_unitless_workday = h;
}

double PlayerHappiness::GetWorkdayPerPerson ()
{   return m_workday_per_person;
}

sint32 PlayerHappiness::GetUnitlessWorkday ()
{
    return m_unitless_workday;
}

void PlayerHappiness::SetWagesLevel (sint32 w)
{
    m_wages_per_person = w * g_theConstDB->Get(0)->GetUnitWages() + g_theConstDB->Get(0)->GetBaseWages();
    m_unitless_wages = w;
}

double PlayerHappiness::GetWagesPerPerson ()
{
	return m_wages_per_person;
}

sint32 PlayerHappiness::GetUnitlessWages ()
{
	return m_unitless_wages;
}

void PlayerHappiness::SetRationsLevel (sint32 r)
{
    m_rations_per_person = r * g_theConstDB->Get(0)->GetUnitRations() + g_theConstDB->Get(0)->GetBaseRations();
    m_unitless_rations = r;
}

double PlayerHappiness::GetRationsPerPerson () const
{   return m_rations_per_person;
}

sint32 PlayerHappiness::GetUnitlessRations ()
{
    return m_unitless_rations;
}

void PlayerHappiness::DisplayWWR()
{
	char str[80];

	sprintf(str, "work    %d work per person:%f  ",  m_unitless_workday, m_workday_per_person);
	g_debugWindow->AddText(str);
	sprintf(str, "wage    %d wage per person:%f  ",  m_unitless_wages, m_wages_per_person);
	g_debugWindow->AddText(str);

	sprintf(str, "rations %d  rations per person:%f ", m_unitless_rations, m_rations_per_person * g_theConstDB->Get(0)->GetBaseRations());
	g_debugWindow->AddText(str);


















}

void PlayerHappiness::Serialize (CivArchive &archive)

{
    CHECKSERIALIZE

    if (archive.IsStoring()) {
		archive.Store((uint8 *)this, sizeof(PlayerHappiness)) ;
    } else {
		archive.Load((uint8 *)this, sizeof(PlayerHappiness)) ;
    }
}
