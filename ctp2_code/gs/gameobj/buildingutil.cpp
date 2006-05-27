//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ source
// Description  : Building data handling
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
// _DEBUG
// - Generate debug version when set.
//
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Fixed buildingutil_GetOffenseBonusAir and buildingutil_GetOffenseBonusWater
// - Fix by NelsonAndBronte on 04-11-2003
// - EMOD GetGoldPerCity added to work like GoldPerPop  2-15-2006
// - EMOD GetGoldPerUnit added to work like GoldPerPop  2-24-2006
// - EMOD GetGoldPerUnitReadiness added to work like GoldPerPop 2-24-2006
// - buildingutil_IsObsolete added by E so buildings can be obsolete ike wonders 4-28-2006
// - buildingutil_GetEmbassiesEverywhereEvenAtWar
// - buildingutil_GetIncreaseHP
//
//----------------------------------------------------------------------------


#include "c3.h"
#include "c3errors.h"

#include "BuildingRecord.h"
#include "StrDB.h"
#include "ErrMsg.h"
#include "Unit.h"
#include "civarchive.h"

#include "WonderTracker.h"
#include "WonderRecord.h"
#include "player.h"

#include "wonderutil.h"

#include "FeatTracker.h"

extern StringDB *g_theStringDB; 
extern sint32 g_parse_improve_abort;

extern Player **g_player;

#define shiftbit(i) uint64(uint64(0x01) << uint64(i))		









#define k_MAX_BUILDING_FLAGS 64





#define FOREACH_BUILT(recordFunc) \
const BuildingRecord *rec; \
sint32 i; \
uint64 cur_built; \
for (i=0,cur_built=built_improvements;cur_built!=0;cur_built>>=1,i++) \
	if ((cur_built&1) && \
		(rec = g_theBuildingDB->Get(i))->recordFunc())



void buildingutil_Initialize()
{
}

sint32 buildingutil_GetProductionCost(const sint32 building_type)
{
    const BuildingRecord* rec = g_theBuildingDB->Get(building_type);
	Assert(rec);
	if(rec == NULL)
		return 0;

    return rec->GetProductionCost();
}

sint32 buildingutil_GetTotalUpkeep(const uint64 built_improvements,
										   sint32 wonderLevel)
{
    //sint32 owner = 0;
	sint32 upkeep = 0;
	for(sint32 i = 0; i < g_theBuildingDB->NumRecords(); i++) {
		if(built_improvements & shiftbit(i)) {
			if(g_theBuildingDB->Get(i)->GetUpkeep() > wonderLevel) {
				upkeep += g_theBuildingDB->Get(i)->GetUpkeep();

//EMOD added new Upkeep calculations (3-13-2006) out commented because AI wouldnt do science????
//				upkeep += g_theBuildingDB->Get(i)->GetUpkeepPerUnitWagesReadiness * g_player[m_owner]->GetNumUnits() * g_player[owner]->m_readiness->GetSupportModifier(g_player[owner]->GetGovernmentType()) * g_player[owner]->GetWagesPerPerson();
//				upkeep += g_theBuildingDB->Get(i)->GetUpkeepPerCity * g_player[m_owner]->GetNumCities();
//				upkeep += g_theBuildingDB->Get(i)->GetUpkeepPerCitySq * g_player[owner]->GetNumCities() *  g_player[owner]->GetNumCities();
//				upkeep += g_theBuildingDB->Get(i)->GetUpkeepPerUnit * g_player[owner]->GetNumUnits();
//end EMOD
			}
		}
	}
		return upkeep;
}

sint32 buildingutil_GetBlgUpkeep(const sint32 building_type)
{
    return g_theBuildingDB->Get(building_type)->GetUpkeep();
}

sint32 buildingutil_GetCheapestBuilding(const uint64 built_improvements, 
												sint32 wonderLevel)
{
	sint32 lowcost = 0x7fffffff;
	sint32 low = -1;

	for(sint32 i = 0; i < g_theBuildingDB->NumRecords(); i++) {
		if(built_improvements & shiftbit(i)) {
			if(g_theBuildingDB->Get(i)->GetUpkeep() > wonderLevel && g_theBuildingDB->Get(i)->GetUpkeep() < lowcost) {
				lowcost = g_theBuildingDB->Get(i)->GetUpkeep();
				low = i;
// EMOD add new upkeep calucations?

			}
		}
	}
	return low;
}

sint32 buildingutil_GetDesignatesCapitol(const uint64 built_improvements)

{
	FOREACH_BUILT(GetCapitol) {
		return TRUE;
	}
    return FALSE;  
}

void buildingutil_GetDefendersBonus(const uint64 built_improvements, 
     double &bonus)

{
    bonus = 0.0;
    FOREACH_BUILT(GetDefendersPercent) {
		double b;
		rec->GetDefendersPercent(b);
		bonus += b;
	}
}

void buildingutil_GetHappinessIncrement(const uint64 built_improvements, 
     sint32 &bonus, sint32 owner)

{



	double wonderCathedralIncrease = 0;
	if(g_player[owner]) {
		wonderCathedralIncrease = double(wonderutil_GetIncreaseCathedrals(
			g_player[owner]->m_builtWonders)) / 100.0;
	}

    bonus = 0;
	FOREACH_BUILT(GetHappyInc) {
		sint32 inc;
		rec->GetHappyInc(inc);
		if(rec->GetIsReligious())
			inc -= sint32(double(inc) * 0); 

		if(rec->GetCathedral()) {
			inc += sint32(double(inc * wonderCathedralIncrease));
		}

		bonus += inc;
	}
}

void buildingutil_GetDoubleTelevangelism(uint64 built_improvements, sint32 &is_dbl_tel)

{
    is_dbl_tel = FALSE; 
	FOREACH_BUILT(GetDoubleTelevangelists) {
		is_dbl_tel = TRUE; 
		return;
	}
}


sint32 buildingutil_GetNoUnhappyPeople(const uint64 built_improvements)

{
	FOREACH_BUILT(GetNoUnhappyPeople) {
		return TRUE;
	}

    return FALSE; 
}


double buildingutil_GetLowerCrime(const uint64 built_improvements)
{
	double crimeMod = 0.0;
	FOREACH_BUILT(GetLowerCrime) {
		double lc;
		rec->GetLowerCrime(lc);
		crimeMod += lc;
	}
	return crimeMod;
}

double buildingutil_GetPreventConversion(const uint64 built_improvements)
{
	double theoMod = 0.0;
	FOREACH_BUILT(GetPreventConversion) {
		double mod;
		rec->GetPreventConversion(mod);
		theoMod += mod;
	}
	return theoMod;
}

double buildingutil_GetPreventSlavery(const uint64 built_improvements)
{
	double best = 0;
	FOREACH_BUILT(GetPreventSlavery) {
		double cur;
		if(rec->GetPreventSlavery(cur) && cur > best)
			best = cur;
	}
	return best;
}

double buildingutil_GetLowerPeaceMovement(const uint64 built_improvements)
{
	double peaceMod = 0;

	FOREACH_BUILT(GetLowerPeaceMovement) {
		double mod;
		rec->GetLowerPeaceMovement(mod);
		peaceMod += mod;
	}
	return peaceMod;
}

sint32 buildingutil_GetGoldPerCitizen(const uint64 built_improvements)
{
	sint32 goldMod = 0;
	FOREACH_BUILT(GetGoldPerCitizen) {
		sint32 mod;
		rec->GetGoldPerCitizen(mod);
		goldMod += mod;
	}
	return goldMod;
}

sint32 buildingutil_GetGoldPerCity(const uint64 built_improvements) //EMOD
{
	sint32 goldMod = 0;
	FOREACH_BUILT(GetGoldPerCity) {
		sint32 mod;
		rec->GetGoldPerCity(mod);
		goldMod += mod;
	}
	return goldMod;
}

sint32 buildingutil_GetGoldPerUnit(const uint64 built_improvements) //EMOD
{
	sint32 goldMod = 0;
	FOREACH_BUILT(GetGoldPerUnit) {
		sint32 mod;
		rec->GetGoldPerUnit(mod);
		goldMod += mod;
	}
	return goldMod;
}

sint32 buildingutil_GetGoldPerUnitReadiness(const uint64 built_improvements) //EMOD
{
	sint32 goldMod = 0;
	FOREACH_BUILT(GetGoldPerUnitReadiness) {
		sint32 mod;
		rec->GetGoldPerUnitReadiness(mod);
		goldMod += mod;
	}
	return goldMod;
}

sint32 buildingutil_GetGoldPerUnitSupport(const uint64 built_improvements) //EMOD
{
	sint32 goldMod = 0;
	FOREACH_BUILT(GetGoldPerUnitSupport) {
		sint32 mod;
		rec->GetGoldPerUnitSupport(mod);
		goldMod += mod;
	}
	return goldMod;
}

sint32 buildingutil_GetUpkeepPerCity(const uint64 built_improvements) //EMOD
{
	sint32 goldMod = 0;
	FOREACH_BUILT(GetUpkeepPerCity) {
		sint32 mod;
		rec->GetUpkeepPerCity(mod);
		goldMod += mod;
	}
	return goldMod;
}

sint32 buildingutil_GetUpkeepPerUnit(const uint64 built_improvements) //EMOD
{
	sint32 goldMod = 0;
	FOREACH_BUILT(GetUpkeepPerUnit) {
		sint32 mod;
		rec->GetUpkeepPerUnit(mod);
		goldMod += mod;
	}
	return goldMod;
}

sint32 buildingutil_GetUpkeepPerUnitWagesReadiness(const uint64 built_improvements) //EMOD
{
	sint32 goldMod = 0;
	FOREACH_BUILT(GetUpkeepPerUnitWagesReadiness) {
		sint32 mod;
		rec->GetUpkeepPerUnitWagesReadiness(mod);
		goldMod += mod;
	}
	return goldMod;
}

BOOL buildingutil_GetProtectFromNukes(const uint64 built_improvements)
{
	FOREACH_BUILT(GetProtectFromNukes) {
		return TRUE;
	}
	return FALSE;
}

double buildingutil_GetProtectFromBioAgents(const uint64 built_improvements)
{
	double best = 0;
	FOREACH_BUILT(GetProtectFromBioAgents) {
		double cur;
		if(rec->GetProtectFromBioAgents(cur) && cur > best)
			best = cur;
	}
	return best;
}

double buildingutil_GetProtectFromNanoVirus(const uint64 built_improvements)
{
	double best = 0;
	FOREACH_BUILT(GetProtectFromNanoVirus) {
		double cur;
		if(rec->GetProtectFromNanoVirus(cur) && cur > best)
			best = cur;
	}
	return best;
}

BOOL buildingutil_GetTelevision(const uint64 built_improvements)
{
	FOREACH_BUILT(GetTelevision) {
		return TRUE;
	}
	return FALSE;
}


BOOL buildingutil_GetCityWalls(const uint64 built_improvements)
{
	FOREACH_BUILT(GetCityWalls) {
		return TRUE;
	}
	return FALSE;
}

double buildingutil_GetCityWallsDefense(const uint64 built_improvements)
{
	double val=0.0, temp=0.0;
	FOREACH_BUILT(GetCityWalls) 
	{
		if(g_theBuildingDB->Get(i)->GetDefendersPercent(temp))
		{
			val+=temp;
		}
	}
	return val;
}

BOOL buildingutil_GetAirport(const uint64 built_improvements)
{
	FOREACH_BUILT(GetAirport) {
		return TRUE;
	}
	return FALSE;
}

BOOL buildingutil_HaveFoodVat(const uint64 built_improvements, 
									  double &food_to_pollution_coef)
{
	BOOL atLeastOne = FALSE;
	food_to_pollution_coef = 0;

	FOREACH_BUILT(GetFoodVat) {
		atLeastOne = TRUE;
		double coef;
		rec->GetFoodVat(coef);
		food_to_pollution_coef += coef;
	}
	return atLeastOne;
}

BOOL buildingutil_NoRushBuyPenalty(const uint64 built_improvements)
{
	FOREACH_BUILT(GetNoRushBuyPenalty) {
		return TRUE;
	}
	return FALSE;
}

BOOL buildingutil_IsCathedral(const uint64 built_improvements)
{
	FOREACH_BUILT(GetCathedral) {
		return TRUE;
	}
	return FALSE;
}

BOOL buildingutil_IsBrokerage(const uint64 built_improvements)
{
	FOREACH_BUILT(GetBrokerage) {
		return TRUE;
	}
	return FALSE;
}

BOOL buildingutil_IsNuclearPlant(const uint64 built_improvements)
{
	FOREACH_BUILT(GetNuclearPlant) {
		return TRUE;
	}
	return FALSE;
}

double buildingutil_GetIncreaseSciencePerPop(const uint64 built_improvements)
{
	double sci = 0;
	FOREACH_BUILT(GetSciencePerPop) {
		double s;
		rec->GetSciencePerPop(s);
		sci += s;
	}
	return sci;
}


BOOL buildingutil_GetForceField(const uint64 built_improvements)
{
	FOREACH_BUILT(GetForceField) {
		return TRUE;
	}
	return FALSE;
}

BOOL buildingutil_GetProductionPercent(const uint64 built_improvements,
									   double &percent)
{
	percent = 0;
	FOREACH_BUILT(GetProductionPercent) {
		double p;
		rec->GetProductionPercent(p);
		percent += p;
	}
	return percent > 0;
}

BOOL buildingutil_GetFoodPercent(const uint64 built_improvements,
									   double &percent)
{
	percent = 0;
	FOREACH_BUILT(GetFoodPercent) {
		double p;
		rec->GetFoodPercent(p);
		percent += p;
	}
	return percent > 0;
}

BOOL buildingutil_GetCommercePercent(const uint64 built_improvements,
									   double &percent, sint32 owner)
{
	double wonderBrokerageIncrease = 0;
	if(g_player[owner]) {
		wonderBrokerageIncrease = double(wonderutil_GetIncreaseBrokerages(
			g_player[owner]->m_builtWonders)) / 100.0;
	}

	percent = 0;
	FOREACH_BUILT(GetCommercePercent) {
		double p;
		rec->GetCommercePercent(p);

		if(rec->GetBrokerage()) {
			p += double(p * wonderBrokerageIncrease);
		}

		percent += p;
	}
	return percent > 0;
}

BOOL buildingutil_GetSciencePercent(const uint64 built_improvements,
									   double &percent)
{
	percent = 0;
	FOREACH_BUILT(GetSciencePercent) {
		double p;
		rec->GetSciencePercent(p);
		percent += p;
	}
	return percent > 0;
}

BOOL buildingutil_GetRaiseOvercrowdingLevel(const uint64 built_improvements,
											sint32 &level)
{
	level = 0;
	FOREACH_BUILT(GetRaiseOvercrowdingLevel) {
		sint32 l;
		rec->GetRaiseOvercrowdingLevel(l);
		level += l;
	}

	return level > 0;
}

BOOL buildingutil_GetRaiseMaxPopulation(const uint64 built_improvements,
										sint32 &level)
{
	level = 0;
	FOREACH_BUILT(GetRaiseMaxPopulation) {
		sint32 l;
		rec->GetRaiseMaxPopulation(l);
		level += l;
	}
	return level > 0;
}

sint32 buildingutil_GetStarvationProtection(const uint64 built_improvements)
{
	sint32 amt = 0;
	FOREACH_BUILT(GetStarvationProtection) {
		sint32 l;
		rec->GetStarvationProtection(l);
		amt += l;
	}
	return amt;
}

double buildingutil_GetOffenseBonusLand(const uint64 built_improvements)
{
	double best = 0;
	FOREACH_BUILT(GetOffenseBonusLand) {
		double cur;
		if(rec->GetOffenseBonusLand(cur) && cur > best)
			best = cur;
	}
	return best;
}

double buildingutil_GetOffenseBonusWater(const uint64 built_improvements)
{
	double best = 0;
	FOREACH_BUILT(GetOffenseBonusWater) {
		double cur;
		if(rec->GetOffenseBonusWater(cur) && cur > best)
	//	Original code:
	//	if(rec->GetOffenseBonusLand(cur) && cur > best)
	//	Fix by NelsonAndBronte on 04-11-2003
			best = cur;
	}
	return best;
}

double buildingutil_GetOffenseBonusAir(const uint64 built_improvements)
{
	double best = 0;
	FOREACH_BUILT(GetOffenseBonusAir) {
//	Original code:
//	FOREACH_BUILT(GetOffenseBonusWater) {
		double cur;
		if(rec->GetOffenseBonusAir(cur) && cur > best)
	//	Original code:
	//	if(rec->GetOffenseBonusLand(cur) && cur > best)
	//	Fix by NelsonAndBronte on 04-11-2003
			best = cur;
	}
	return best;
}

BOOL buildingutil_IsObsolete(sint32 building_type)
{
    const BuildingRecord* rec = g_theBuildingDB->Get(building_type);
	sint32 nObsolete = rec->GetNumObsoleteAdvance();
	if(nObsolete <= 0)
		return FALSE;

	for(sint32 p = 0; p < k_MAX_PLAYERS; p++) {
		if(!g_player[p]) continue;

		for(sint32 o = 0; o < nObsolete; o++) {
			if(g_player[p]->HasAdvance(rec->GetObsoleteAdvanceIndex(o)))
				return TRUE;
		}
	}
	return FALSE;
}

BOOL buildingutil_GetEmbassiesEverywhereEvenAtWar(const uint64 built_improvements)
{
	FOREACH_BUILT(GetEmbassiesEverywhereEvenAtWar) {
		return TRUE;
	}
	return FALSE;
}

sint32 buildingutil_GetIncreaseHP(const uint64 built_improvements)
{
		sint32 amt = 0;
	FOREACH_BUILT(GetIncreaseHP) {
		sint32 l;
		rec->GetIncreaseHP(l);
		amt += l;
	}
	return amt;
}