//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C/C++ header
// Description  : Combat events
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
// - Fixed a memory leak concerning g_theCurrentBattle. (7-Nov-2007 Martin G�hmann)
//
//----------------------------------------------------------------------------

#include "ctp/c3.h"
#include "gs/gameobj/combatevent.h"
#include "gs/gameobj/Events.h"
#include "gs/gameobj/CTP2Combat.h"
#include "gs/events/GameEventUser.h"
#include "gs/gameobj/Army.h"
#include "gs/world/MapPoint.h"
#include "gs/world/World.h"
#include "gs/world/Cell.h"
#include "gs/world/cellunitlist.h"
#include "gs/gameobj/ArmyData.h"
#include "gfx/spritesys/director.h"
#include "ui/interface/battleviewwindow.h"
#include "ui/aui_ctp2/c3ui.h"
#include "ui/aui_common/aui_button.h"
#include "gs/utility/Globals.h"
extern C3UI *g_c3ui;

STDEHANDLER(RunCombatEvent)
{
	Army army;
	MapPoint pos;
	sint32 attacker, defender;

	if(!args->GetArmy(0, army))
		return(GEV_HD_Continue);
	if(!args->GetPos(0, pos))
		return(GEV_HD_Continue);
	if(!args->GetPlayer(0, attacker))
		return(GEV_HD_Continue);
	if(!args->GetPlayer(1, defender))
		return(GEV_HD_Continue);
	if(!g_theCurrentBattle)
		return(GEV_HD_Continue);

	bool const  isDoneAtStart   = g_theCurrentBattle->IsDone();
	bool const  playAnimations  =
	    isDoneAtStart ? false : g_theCurrentBattle->ResolveOneRound();

	if (isDoneAtStart)
	{
		// No action: probably a place holder for the final user click.
	}
	else if (g_theCurrentBattle->IsDone())
	{
		g_gevManager->AddEvent(GEV_INSERT_AfterCurrent,
		                       GEV_BattleAftermath,
		                       GEA_Army,     army,
		                       GEA_MapPoint, pos,
		                       GEA_Unit,     army[0],
		                       GEA_Unit,     g_theWorld->GetCell(pos)->AccessUnit(0),
		                       GEA_Player,   attacker,
		                       GEA_Player,   defender,
		                       GEA_Int,      1,
		                       GEA_End
		                      );
		g_theCurrentBattle->KillUnits(GEV_INSERT_AfterCurrent);
	}
	else
	{
		g_gevManager->AddEvent(GEV_INSERT_AfterCurrent,
		                       GEV_RunCombat,
		                       GEA_Army,     army,
		                       GEA_MapPoint, pos,
		                       GEA_Player,   attacker,
		                       GEA_Player,   defender,
		                       GEA_End
		                      );
	}

	return (playAnimations && g_theCurrentBattle->GetBattle())
	       ? GEV_HD_NeedUserInput
	       : GEV_HD_Continue;
}

STDEHANDLER(StartCombatEvent)
{
	Army a;
	MapPoint p;
	if(!args->GetArmy(0, a)) return GEV_HD_Continue;
	if(!args->GetPos(0, p)) return GEV_HD_Continue;

	if (g_theCurrentBattle)
	{
		// Close previous screen - if still open
		if (g_battleViewWindow && g_c3ui->GetWindow(g_battleViewWindow->Id()))
		{
			battleview_ExitButtonActionCallback(NULL, AUI_BUTTON_ACTION_EXECUTE, 0, NULL);
		}
		g_theCurrentBattle->ClearBattle();
		delete g_theCurrentBattle;
		g_theCurrentBattle = NULL;
	}

	CellUnitList defender;
	g_theWorld->GetCell(p)->GetArmy(defender);

	Assert(a.Num() > 0);
	Assert(defender.Num() > 0);
	if (a.Num() > 0 && defender.Num() > 0 &&
	    a.GetOwner() != defender.GetOwner()
	   )
	{
		g_theCurrentBattle = new CTP2Combat(k_COMBAT_WIDTH, k_COMBAT_HEIGHT, *a.AccessData(), defender);
	}

	return GEV_HD_Continue;
}

void combatevent_Initialize()
{
	g_gevManager->AddCallback(GEV_RunCombat, GEV_PRI_Primary, &s_RunCombatEvent);
	g_gevManager->AddCallback(GEV_StartCombat, GEV_PRI_Primary, &s_StartCombatEvent);
}

void combatevent_Cleanup()
{
	allocated::clear(g_theCurrentBattle);
}
