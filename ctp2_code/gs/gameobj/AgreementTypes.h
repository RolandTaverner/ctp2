#pragma once
#ifndef __AGREEMENT_TYPES_H__
#define __AGREEMENT_TYPES_H__

enum AGREEMENT_TYPE
{
	AGREEMENT_TYPE_DEMAND_ADVANCE,
	AGREEMENT_TYPE_DEMAND_CITY,
	AGREEMENT_TYPE_DEMAND_MAP,
	AGREEMENT_TYPE_DEMAND_GOLD,
	AGREEMENT_TYPE_DEMAND_STOP_TRADE,		
	AGREEMENT_TYPE_DEMAND_LEAVE_OUR_LANDS,	
	AGREEMENT_TYPE_REDUCE_POLLUTION,		
	
	AGREEMENT_TYPE_CEASE_FIRE,				
	AGREEMENT_TYPE_PACT_CAPTURE_CITY,		
	AGREEMENT_TYPE_PACT_END_POLLUTION,		
	AGREEMENT_TYPE_DEMAND_ATTACK_ENEMY,
	AGREEMENT_TYPE_OFFER_ADVANCE,
	AGREEMENT_TYPE_OFFER_CITY,
	AGREEMENT_TYPE_OFFER_MAP,
	AGREEMENT_TYPE_OFFER_GOLD,
	AGREEMENT_TYPE_EXCHANGE_ADVANCE,
	AGREEMENT_TYPE_EXCHANGE_CITY,
	AGREEMENT_TYPE_EXCHANGE_MAP,
	
	AGREEMENT_TYPE_NO_PIRACY,
	
	AGREEMENT_TYPE_NULL,
	AGREEMENT_TYPE_MAX
};

#endif
