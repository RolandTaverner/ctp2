#include "ctp/c3.h"

#include "gs/gameobj/Sci.h"
#include "robot/aibackdoor/civarchive.h"
#include "ai/ctpai.h"
#include "gs/gameobj/Player.h"
#include "ai/diplomacy/AgreementMatrix.h"
#include "gs/gameobj/Unit.h"
#include "gs/gameobj/UnitData.h"
#include "gs/gameobj/UnitPool.h"

Science::Science()

{
	m_level = 0;
}











void Science::Serialize(CivArchive &archive)
{
	if (archive.IsStoring())
		archive<<m_level ;
	else
		archive>>m_level ;

}














uint32 Sci_Science_GetVersion(void)
	{
	return (k_SCIENCE_VERSION_MAJOR<<16 | k_SCIENCE_VERSION_MINOR) ;
	}


sint32 Science::ComputeScienceFromResearchPacts(const sint32 playerId)
{
	sint32 total_pact_science = 0;
	for (PLAYER_INDEX foreignerId = 1; foreignerId < CtpAi::s_maxPlayers; foreignerId++)
	{
		if ((foreignerId != playerId) && g_player[foreignerId])
		{
			total_pact_science += ComputeScienceFromResearchPact(playerId, foreignerId);
		}
	}
	return total_pact_science;
}

sint32 Science::ComputeScienceFromResearchPact(const sint32 playerId, const sint32 foreignerId)
{
	Assert(g_player[foreignerId] != NULL);
	if (g_player[foreignerId] == NULL)
		return 0;

	ai::Agreement research_pact =
		AgreementMatrix::s_agreements.GetAgreement(playerId, foreignerId, PROPOSAL_TREATY_RESEARCH_PACT);
	sint32 research_pact_science = 0;

	if (research_pact.start != -1 && research_pact.end == -1)
	{
		Unit city;
		sint32 num_cities = g_player[foreignerId]->m_all_cities->Num();
		for (sint32 i = 0; i < num_cities; i++)
		{
			city = g_player[foreignerId]->m_all_cities->Access(i);
			Assert( g_theUnitPool->IsValid(city) );
			Assert( city->GetCityData() != NULL );

			research_pact_science += city.CD()->GetScience();
		}

		research_pact_science = research_pact_science * 105 / 100;
	}
	return research_pact_science;
}
