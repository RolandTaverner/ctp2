#include "ctp/c3.h"
#include "gs/gameobj/Unit.h"
#include "gs/gameobj/Player.h"

#include "gs/gameobj/DiplomaticRequestPool.h"
#include "gs/gameobj/MessagePool.h"
#include "gs/database/StrDB.h"
#include "net/general/network.h"
#include "net/general/net_action.h"
#include "robot/aibackdoor/civarchive.h"
#include "gs/utility/DiplDynArr.h"
#include "gs/utility/MsgDynArr.h"
#include "gs/utility/TurnCnt.h"
#include "gs/utility/Globals.h"
#include "gs/fileio/gamefile.h"

extern	StringDB	*g_theStringDB ;

extern	Player	**g_player ;
extern TurnCount *g_turn;

#include "gs/gameobj/Diplomacy_Log.h"
extern Diplomacy_Log *g_theDiplomacyLog;








DiplomaticRequestPool::DiplomaticRequestPool() : ObjPool(k_BIT_GAME_OBJ_TYPE_DIPLOMATIC_REQUEST)
	{
	}









DiplomaticRequestPool::DiplomaticRequestPool(CivArchive &archive) : ObjPool(k_BIT_GAME_OBJ_TYPE_DIPLOMATIC_REQUEST)
	{
	Serialize(archive) ;
	}












void DiplomaticRequestPool::Serialize(CivArchive &archive)
{
	DiplomaticRequestData	*newData ;

	sint32	i,
			count = 0 ;

    CHECKSERIALIZE

#define DIPREQPOOL_MAGIC 0xEEFFAADD
	if (archive.IsStoring())
		{
		}
	else
	{
		if(g_saveFileVersion < 55)
		{
			archive.TestMagic(DIPREQPOOL_MAGIC) ;
			archive>>count;
			for (i=0; i<count; i++)
			{
				newData = new DiplomaticRequestData(archive) ;
				Insert(newData) ;
			}

		}

	}
}


















DiplomaticRequest DiplomaticRequestPool::Create(PLAYER_INDEX owner, PLAYER_INDEX recipient, REQUEST_TYPE request)
	{
	DiplomaticRequestData* newData;

#ifdef _DEBUG
    if (g_theDiplomacyLog) {
        g_theDiplomacyLog->LogRegard(owner, recipient);
        g_theDiplomacyLog->LogRegard(recipient, owner);
    }
#endif // _DEBUG

	DiplomaticRequest newRequest(NewKey(k_BIT_GAME_OBJ_TYPE_DIPLOMATIC_REQUEST));

	newData = new DiplomaticRequestData(newRequest, owner, recipient, request) ;
	Insert(newData) ;

	g_player[owner]->AddDiplomaticRequest(newRequest) ;
	g_player[owner]->IncrementSentRequests(recipient);

	g_player[recipient]->AddDiplomaticRequest(newRequest) ;

#ifdef _DEBUG
    if (g_theDiplomacyLog) {
        g_theDiplomacyLog->LogRequestCreated(owner, recipient, request);
    }
#endif // _DEBUG

#if 0
	if(g_network.IsClient())
		{
		Assert(owner == g_network.GetPlayerIndex());
		if(owner == g_network.GetPlayerIndex())
			{
			g_network.AddCreatedObject(newData);
			g_network.SendAction(new NetAction(NET_ACTION_CREATE_DIP_REQUEST,
											   (uint32)owner,
											   (uint32)recipient,
											   (uint32)request,
											   (uint32)newRequest));
			}
		}
#endif
	return (newRequest) ;
	}





DiplomaticRequestData *DiplomaticRequestPool::CreateData()
{
	DiplomaticRequest newRequest(NewKey(k_BIT_GAME_OBJ_TYPE_DIPLOMATIC_REQUEST));
	DiplomaticRequestData *newData;
	newData = new DiplomaticRequestData(newRequest);
	Insert(newData);
	return newData;
}












void DiplomaticRequestPool::EndTurn(const PLAYER_INDEX sender)
	{
	sint32	i ;

	MessageDynamicArray	msgExpired;

	for(i = 0; i < k_OBJ_POOL_TABLE_SIZE; i++)
		{
		if(m_table[i])
			{
			EndTurn((DiplomaticRequestData*)m_table[i], sender, msgExpired);
			}

		}

	for(i = msgExpired.Num() - 1; i >= 0; i--)
		{
		if(g_theMessagePool->IsValid(msgExpired[i]))
			msgExpired[i].Kill();
		}
	}

void DiplomaticRequestPool::EndTurn(DiplomaticRequestData *top,
									const PLAYER_INDEX sender,
									MessageDynamicArray &msgExpired)
{
	if(top->GetLesser())
		EndTurn(top->GetLesser(), sender, msgExpired);
	if(top->GetGreater())
		EndTurn(top->GetGreater(), sender, msgExpired);

}
