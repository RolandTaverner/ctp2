//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ source
// Description  : Turn count handler
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
// - Relaxed assert
// - Moved needs refueling check to Unit.cpp to remove code duplication.
//   - April 24th 2005 Martin G�hmann
// - Replaced old difficulty database by new one. (April 29th 2006 Martin G�hmann)
// - Replaced old const database by new one. (5-Aug-2007 Martin G�hmann)
// - Fixed PBEM BeginTurn event execution. (27-Oct-2007 Martin G�hmann)
// - PollutionBeginTurn is now triggered from PlayerBeginTurn if executed
//   so that flood events make players invalid after all the player events. (29-Oct-2007 Martin G�hmann)
//
//----------------------------------------------------------------------------

#include "ctp/c3.h"

#include "gs/utility/newturncount.h"
#include "gs/events/GameEventManager.h"

#include "ctp/ctp2_utils/c3errors.h"

#include "gfx/spritesys/director.h"

#include "gs/slic/SlicObject.h"
#include "ui/aui_ctp2/SelItem.h"
#include "gs/gameobj/Player.h"
#include "gs/world/World.h"
#include "gs/gameobj/Barbarians.h"
#include "robot/pathing/A_Star_Heuristic_Cost.h"
#include "gs/slic/SlicSegment.h"
#include "gs/slic/SlicEngine.h"
#include "gs/database/profileDB.h"
#include "gs/newdb/DifficultyRecord.h"
#include "gs/gameobj/Diffcly.h"
#include "gfx/tilesys/tiledmap.h"

#include "gs/gameobj/pollution.h"

#include "net/general/network.h"
#include "net/general/net_action.h"
#include "net/general/net_info.h"
#include "net/general/net_rand.h"
#include "net/general/net_ready.h"

#include "gs/newdb/ConstRecord.h"
#include "gs/gameobj/Score.h"
#include "gs/gameobj/GameOver.h"
#include "gs/gameobj/GameSettings.h"
#include "ui/interface/messagemodal.h"
#include "gs/gameobj/Unit.h"
#include "gs/gameobj/UnitData.h"
#include "gs/newdb/UnitRecord.h"
#include "gs/utility/UnitDynArr.h"
#include "gs/gameobj/Readiness.h"
#include "gs/gameobj/buildingutil.h"

#include "gs/utility/TurnCnt.h"

#include "ui/aui_ctp2/radarmap.h"

#include "ui/interface/controlpanelwindow.h"
#include "gs/gameobj/CriticalMessagesPrefs.h"
#include "gs/gameobj/Gold.h"
#include "ui/interface/radarwindow.h"
#include "ui/interface/screenutils.h"

extern SelectedItem             *g_selected_item;
extern World                    *g_world;
extern Director                 *g_director;
extern Player                   **g_player;

extern Pollution                *g_thePollution;


extern ProfileDB                *g_theProfileDB;

extern MessageModal             *g_modalMessage;

sint32 NewTurnCount::sm_the_stop_player = 1;

bool NewTurnCount::m_sentGameAlmostOverMessage=false;
bool NewTurnCount::m_sentGameOverMessage=false;

NewTurnCount::NewTurnCount()
{
	m_sentGameAlmostOverMessage=false;
	m_sentGameOverMessage=false;
}

sint32 NewTurnCount::GetStopPlayer()
{
	return sm_the_stop_player;
}

void NewTurnCount::SetStopPlayer(const sint32 &player_index)
{
	sm_the_stop_player = player_index;
}

void NewTurnCount::StartNextPlayer(bool stop)
{
	DPRINTF(1, ("NewTurnCount::StartNextPlayer(%d), curPlayer: %d\n", stop, g_selected_item->GetCurPlayer()));

	static bool warned=false;
	if (!VerifyEndTurn(warned))
	{
		warned=true;
		return;
	}
	warned=false;

	PLAYER_INDEX current_player = g_selected_item->GetCurPlayer();

	if(g_network.IsClient()) {
		g_network.SendAction(new NetAction(NET_ACTION_END_TURN));
		return;
	}

	g_player[current_player]->EndTurn();

#if 0

	if((g_player[current_player]->IsHuman()
	|| (g_player[current_player]->IsNetwork()
	&&  g_network.IsLocalPlayer(current_player)))
	&&  g_selected_item->GetCurPlayer() == g_selected_item->GetVisiblePlayer()
	){
		g_player[current_player]->m_endingTurn = TRUE;
		g_player[current_player]->ProcessUnitOrders();
		g_player[current_player]->m_endingTurn = FALSE;
	}
#endif

	extern BOOL g_aPlayerIsDead;
	if(!g_network.IsClient() && g_aPlayerIsDead) {
		Player::RemoveDeadPlayers();
	}

	NewTurnCount::ChooseNextActivePlayer();
	PLAYER_INDEX next_player = g_selected_item->GetCurPlayer();
	sint32 next_round = g_player[next_player]->GetCurRound() + 1;

	if(g_turn->IsHotSeat() || g_turn->IsEmail())
	{
		if(!g_player[g_selected_item->GetCurPlayer()]->IsRobot())
		{
			stop = true;
		}

		g_director->NextPlayer();

		g_director->AddCopyVision();

		g_tiledMap->InvalidateMix();
		g_tiledMap->InvalidateMap();
		g_tiledMap->Refresh();
		g_radarMap->Update();
		g_turn->InformMessages();

		g_controlPanel->Hide();
		radarwindow_Hide();
		close_AllScreens();
	}

	if (stop ||
		(g_network.IsActive() &&
		 (g_network.IsClient() || !g_player[next_player]->IsRobot())))
	{
		NewTurnCount::SetStopPlayer(next_player);
		g_director->NextPlayer();
	}

	if(g_network.IsHost() && GetStopPlayer() == next_player)
	{
		if(g_player[next_player]->IsRobot())
		{
			SetStopPlayer(g_selected_item->GetVisiblePlayer());
		}
	}

	g_controlPanel->UpdatePlayerEndProgress(current_player);

	sint32 oldVis = g_selected_item->GetVisiblePlayer();
	g_selected_item->SetPlayerOnScreen(NewTurnCount::GetStopPlayer());

	if(oldVis != g_selected_item->GetVisiblePlayer())
	{
		g_tiledMap->CopyVision();
	}

	if (next_player == 0)
	{
		NewTurnCount::StartNewYear();
	}

	if(g_network.IsHost())
	{
		g_network.QueuePacketToAll(new NetInfo(NET_INFO_CODE_BEGIN_TURN, next_player));
	}

	if((g_turn->IsHotSeat() || g_turn->IsEmail())
	&& !g_player[g_selected_item->GetCurPlayer()]->IsRobot()
	){
		g_turn->SendNextPlayerMessage();

		if(g_turn->IsEmail())
		{
			return;
		}
	}

	if(!g_network.IsHost() || g_network.IsLocalPlayer(next_player))
	{
		g_gevManager->AddEvent(GEV_INSERT_Tail, GEV_BeginTurn,
		                       GEA_Player,      next_player,
		                       GEA_Int,         next_round,
		                       GEA_End);
	}
	else
	{
		g_director->NextPlayer();
		g_thePollution->BeginTurn();
	}
}

void NewTurnCount::ChooseNextActivePlayer()
{
	sint32 count = 0;

	do {
		g_selected_item->NextPlayer();
		g_director->NextPlayer();
		count++;
	} while( g_player[g_selected_item->GetCurPlayer()] == NULL );
}

void NewTurnCount::StartNewYear()
{

	g_theWorld->A_star_heuristic->Update();

	Barbarians::BeginYear();





	g_slicEngine->RunYearlyTriggers();

	g_thePollution->EndRound();

	RunNewYearMessages();
}

void NewTurnCount::ClientStartNewYear()
{

	g_theWorld->A_star_heuristic->Update();

	RunNewYearMessages();
}

sint32 NewTurnCount::GetCurrentYear(sint32 player)
{
	PLAYER_INDEX current_player = g_selected_item->GetCurPlayer();
	if(player >= 0 && player < k_MAX_PLAYERS)
		current_player = player;

	Assert(g_player != NULL);
	Assert(g_player[current_player] != NULL);
	if(!g_player || !g_player[current_player]) return 0;

	sint32 round = g_player[current_player]->GetCurRound();

	return diffutil_GetYearFromTurn(g_theGameSettings->GetDifficulty(), round);
}

sint32 NewTurnCount::GetCurrentRound()
{
	PLAYER_INDEX current_player = g_selected_item->GetCurPlayer();
	Assert(g_player != NULL);
	if(!g_player || !g_player[current_player]) return 0;

	return g_player[current_player]->GetCurRound();
}

void NewTurnCount::RunNewYearMessages(void)
{

	if (GetCurrentYear() >= g_theConstDB->Get(0)->GetEndOfGameYearEarlyWarning())
	{
		if(!m_sentGameAlmostOverMessage)
		{
			m_sentGameAlmostOverMessage = TRUE;

			SendMsgEndOfGameEarlyWarning() ;
		}
	}

	if(GetCurrentYear() >= g_theConstDB->Get(0)->GetEndOfGameYear())
	{
		if(!m_sentGameOverMessage)
		{
			sint32 i;
			sint32 highScore = -1;
			sint32 highPlayer = 1;

			for(i = 1; i < k_MAX_PLAYERS; i++)
			{
				if(g_player[i])
				{
					if(g_player[i]->m_score->GetTotalScore() > highScore)
					{
						highScore = g_player[i]->m_score->GetTotalScore();
						highPlayer = i;
					}
				}
			}
			if(g_network.IsHost())
			{
				g_network.Enqueue(new NetInfo(NET_INFO_CODE_GAME_OVER_OUT_OF_TIME,
				                              highPlayer));
			}

			for(i = 0; i < k_MAX_PLAYERS; i++)
			{
				if(g_player[i])
				{
					if(i == highPlayer)
					{
						g_player[i]->GameOver(GAME_OVER_WON_OUT_OF_TIME, -1);
					}
					else
					{
						g_player[i]->GameOver(GAME_OVER_LOST_OUT_OF_TIME, -1);
					}
				}
			}
			m_sentGameOverMessage = true;
		}
		else
		{
			g_theGameSettings->SetKeepScore(FALSE);
		}
	}
}

void NewTurnCount::SendMsgEndOfGameEarlyWarning()
{
	SendMsgToAllPlayers("73EndOfGameTimeIsRunningOut") ;
	if(g_network.IsHost())
	{
		g_network.Enqueue(new NetInfo(NET_INFO_CODE_TIMES_ALMOST_UP));
	}
}

void NewTurnCount::SendMsgToAllPlayers(MBCHAR *s)
{
	sint32	i ;

	SlicObject *so = new SlicObject(s) ;

	for(i=0; i<k_MAX_PLAYERS; i++)
	{
		if ((g_player[i]) && (!g_player[i]->IsDead()))
			so->AddRecipient(i) ;

	}

	g_slicEngine->Execute(so) ;
}

BOOL NewTurnCount::VerifyEndTurn(BOOL force)
{
	Player *player = g_player[g_selected_item->GetCurPlayer()];

	if (!player->IsHuman())
	{
		return(TRUE);
	}

	if(g_network.IsActive() && (g_network.IsSpeedStyle() || g_network.IsTimedStyle())) {
		return TRUE;
	}

	if (g_modalMessage && !force)
		return FALSE;

	if(g_theCriticalMessagesPrefs->IsEnabled("16IAOutOfFuel")) {
		if (g_slicEngine->GetSegment("16IAOutOfFuel")->TestLastShown(player->m_owner, 1)) {
			int i;
			int n = player->GetAllUnitList()->Num();
			for (i=0; i<n; i++) {
				Unit *unit = &(player->GetAllUnitList()->Access(i));
				if (!(unit->GetMovementTypeAir()) && !(unit->GetMovementTypeSpace()))
					continue;
				if (!(unit->GetDBRec()->GetNoFuelThenCrash()))
					continue;
				if(unit->AccessData()->CheckForRefuel())
					continue;
				if (unit->NeedsRefueling()) {
					SlicObject *so = new SlicObject("16IAOutOfFuel") ;
					so->AddRecipient(player->m_owner) ;
					so->AddCivilisation(player->m_owner) ;
					g_slicEngine->Execute(so) ;
					return(FALSE);
				}
			}
		}
	}

	if(g_theCriticalMessagesPrefs->IsEnabled("23IACityWillStarve")) {
		if (g_slicEngine->GetSegment("23IACityWillStarve")->TestLastShown(player->m_owner, 1)) {
			int i;
			int n = player->GetAllCitiesList()->Num();
			for (i=0; i<n; i++) {
				double tmp;
				Unit *unit = &(player->GetAllCitiesList()->Access(i));
				if (!(unit->IsCity()))
					continue;
				if (buildingutil_HaveFoodVat(unit->GetImprovements(), tmp, unit->GetOwner()))
					continue;
				CityData *city = unit->GetData()->GetCityData();
//				double fudge = (double)(g_theConstDB->StarvationWarningFudgeFactor()) / 100.0;
				if ((city->GetProducedFood() < city->GetConsumedFood()) &&
					(city->GetStarvationTurns() == 0)) {
					SlicObject *so = new SlicObject("23IACityWillStarve") ;
					so->AddRecipient(player->m_owner) ;
					so->AddCity(*unit) ;
					g_slicEngine->Execute(so) ;
					return(FALSE);
				}
			}
		}
	}

	if(g_theCriticalMessagesPrefs->IsEnabled("21IACannotAffordMaintenance")) {
		if (g_slicEngine->GetSegment("21IACannotAffordMaintenance")->TestLastShown(player->m_owner, 1)) {
			if (player->m_gold->BankruptcyImminent() &&
				(player->CalcTotalBuildingUpkeep() > 0)) {
				SlicObject *so = new SlicObject("21IACannotAffordMaintenance") ;
				so->AddRecipient(player->m_owner) ;
				so->AddCivilisation(player->m_owner) ;
				g_slicEngine->Execute(so) ;
				return(FALSE);
			}
		}
	}

	if(g_theCriticalMessagesPrefs->IsEnabled("22IACannotAffordSupport")) {
		if (g_slicEngine->GetSegment("22IACannotAffordSupport")->TestLastShown(player->m_owner, 1)) {
			int i;
			int n = player->GetAllCitiesList()->Num();
			double prod_total = 0.0;
			double fudge = (double)(g_theConstDB->Get(0)->GetSupportWarningFudgeFactor()) / 100.0;
			for (i=0; i<n; i++) {
				Unit *unit = &(player->GetAllCitiesList()->Access(i));
				if (!(unit->IsCity()))
					continue;
				CityData *city = unit->GetData()->GetCityData();
				prod_total += city->ProjectMilitaryContribution();
			}
			prod_total *= fudge;
			if (!(player->m_first_city) &&
				(prod_total < player->m_readiness->GetCost())) {
				SlicObject *so = new SlicObject("22IACannotAffordSupport") ;
				so->AddRecipient(player->m_owner) ;
				so->AddCivilisation(player->m_owner) ;
				g_slicEngine->Execute(so) ;
				return(FALSE);
			}
		}
	}

	return(TRUE);
}
