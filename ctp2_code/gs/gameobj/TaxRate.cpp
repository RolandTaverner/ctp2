#include "ctp/c3.h"
#include "gs/gameobj/TaxRate.h"
#include "robot/aibackdoor/civarchive.h"
#include "gs/slic/SlicEngine.h"
#include "gs/database/DB.h"
#include "gs/newdb/GovernmentRecord.h"
#include "gs/gameobj/Player.h"
#include "net/general/network.h"
#include "net/general/net_info.h"
#include "net/general/net_action.h"

extern Player **g_player;











void TaxRate::Serialize(CivArchive &archive)
{
    if (archive.IsStoring()) {
        archive << m_science;
		archive << m_science_before_anarchy;
    } else {
        archive >> m_science;
		archive >> m_science_before_anarchy;
    }
}














uint32 TaxRate_TaxRate_GetVersion(void)
	{
	return (k_TAXRATE_VERSION_MAJOR<<16 | k_TAXRATE_VERSION_MINOR);
	}

void TaxRate::SetTaxRates(double s, sint32 owner)
{
	Assert(0.0 <= s);
    Assert(s <= 1.0);

	if(s > g_theGovernmentDB->Get(g_player[owner]->m_government_type)->GetMaxScienceRate())
		s = g_theGovernmentDB->Get(g_player[owner]->m_government_type)->GetMaxScienceRate();

	double oldscience = m_science;
	m_science = s;
	if(oldscience != m_science) {
		g_slicEngine->RunScienceRateTriggers(owner);
	}

	if(g_network.IsClient() && g_network.IsLocalPlayer(owner)) {
		g_network.SendAction(new NetAction(NET_ACTION_TAX_RATES,
										   (sint32)(s * 100000.),
										   0,
										   0));
	} else if(g_network.IsHost()) {
		g_network.Block(owner);
		g_network.Enqueue(new NetInfo(NET_INFO_CODE_TAX_RATE,
									  owner,
									  (sint32)(s * 100000.),
									  0, 0));
		g_network.Unblock(owner);
	}
}

void TaxRate::InitTaxRates(double s, sint32 owner)
{
	m_science = s;
}
