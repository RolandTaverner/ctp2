











#include "c3.h"

#if 0
#include "c3errors.h"
#include "civarchive.h"
#include "Token.h"

#include "StrDB.h"
#include "AdvDB.h"
#include "TerrRec.h"
#include "DB.h"
#include "GovRec.h"
#include "UVRecord.h"

#include "Token.h"
enum TOKEN_UV_POLLUTION
	{
	TOKEN_OZONE_TRIGGER = TOKEN_MAX + 1,
	TOKEN_UV_NORTH_POLAR_PROB,
	TOKEN_UV_UPPER_NORTH_MERIDIAN_PROB,
	TOKEN_UV_LOWER_NORTH_MERIDIAN_PROB,
	TOKEN_UV_EQUATORIAL_PROB,
	TOKEN_UV_UPPER_SOUTH_MERIDIAN_PROB,
	TOKEN_UV_LOWER_SOUTH_MERIDIAN_PROB,
	TOKEN_UV_SOUTH_POLAR_PROB,
	TOKEN_OZONE_DEAD_TILES,
	TOKEN_OZONE_DEAD_RESOURCES,
	TOKEN_UV_DEAD_TILE_DESERT,
	TOKEN_UV_DEAD_TILE_FORESTS,
	TOKEN_UV_DEAD_TILE_GRASSLANDS,
	TOKEN_UV_DEAD_TILE_HILLS,
	TOKEN_UV_DEAD_TILE_MOUNTAINS,
	TOKEN_UV_DEAD_TILE_PLAINS,
	TOKEN_UV_DEAD_TILE_SWAMPS,
	TOKEN_UV_DEAD_TILE_TUNDRA,
	TOKEN_UV_DEAD_TILE_CONTINENTAL_SHELF,
	TOKEN_UV_DEAD_RESOURCE_PALM_OIL,
	TOKEN_UV_DEAD_RESOURCE_OIL,
	TOKEN_UV_DEAD_RESOURCE_PHEASANT,
	TOKEN_UV_DEAD_RESOURCE_SILK,
	TOKEN_UV_DEAD_RESOURCE_SPICE,
	TOKEN_UV_DEAD_RESOURCE_TUSKS,
	TOKEN_UV_DEAD_RESOURCE_BLUBBER,
	TOKEN_UV_DEAD_RESOURCE_ALIEN_CORPSES,
	TOKEN_UV_DEAD_RESOURCE_CLAY,
	TOKEN_UV_DEAD_RESOURCE_TOBACCO,
	TOKEN_UV_DEAD_RESOURCE_COTTON,
	TOKEN_UV_DEAD_RESOURCE_COAL,
	TOKEN_UV_DEAD_RESOURCE_GRAPES,
	TOKEN_UV_DEAD_RESOURCE_GRANITE,
	TOKEN_UV_DEAD_RESOURCE_BANANAS,
	TOKEN_UV_DEAD_RESOURCE_JADE,
	TOKEN_UV_DEAD_RESOURCE_HARDWOOD,
	TOKEN_UV_DEAD_RESOURCE_IRON,
	TOKEN_UV_DEAD_RESOURCE_GOLD,
	TOKEN_UV_DEAD_RESOURCE_SILVER,
	TOKEN_UV_DEAD_RESOURCE_BUFFALO,
	TOKEN_UV_DEAD_RESOURCE_WHEAT,
	TOKEN_UV_DEAD_RESOURCE_CORN,
	TOKEN_UV_DEAD_RESOURCE_PEAT,
	TOKEN_UV_DEAD_RESOURCE_MANGROVE,
	TOKEN_UV_DEAD_RESOURCE_MINK,
	TOKEN_UV_DEAD_RESOURCE_CARIBOU,
	TOKEN_UV_DEAD_RESOURCE_FISH,
	TOKEN_UV_DEAD_RESOURCE_WHALES,
	TOKEN_UV_DEAD_RESOURCE_CRABS,
	TOKEN_UV_DEAD_RESOURCE_DIAMONDS,
	TOKEN_UV_DEAD_RESOURCE_MAGNESIUM_NODULE,
	TOKEN_UV_DEAD_RESOURCE_PHOSPHORUS,
	TOKEN_UV_DEAD_RESOURCE_PLANKTON_UPWELLING,
	TOKEN_UV_DEAD_RESOURCE_GIANT_CLAM,
	TOKEN_UV_DEAD_RESOURCE_GIANT_SQUID,

    TOKEN_UV_POLLUTION_MAX_VAL
	} ;

	extern	StringDB	*g_theStringDB ;

	extern	AdvanceDB	*g_theAdvanceDB ;



	extern	TokenData	g_ImpDB_token_data[] ;


	sint32	g_parse_uv_abort ;









UVRecord::UVRecord()
	{
	memset((uint8*)&m_meridian_prob, 0, (uint8*)&m_dummy - (uint8*)&m_meridian_prob + sizeof(m_dummy));

	memset(m_dead_food, 0, sizeof(m_dead_food));
	memset(m_dead_shield, 0, sizeof(m_dead_shield));
	memset(m_dead_trade, 0, sizeof(m_dead_trade));
	memset(m_dead_gold, 0, sizeof(m_dead_gold));
	memset(m_meridian_prob, 0, sizeof(m_meridian_prob));
	memset(m_meridian_phase_bonus, 0, sizeof(m_meridian_phase_bonus));
	}


UVRecord::~UVRecord()
	{
	}









sint32 UVRecord::ParseUVTriggerRecord(Token *uvToken)
	{
	if (!token_ParseAnOpenBraceNext(uvToken))
		return (FALSE) ;

	
	
	if (uvToken->Next() != TOKEN_UV_NORTH_POLAR_PROB)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected north polar region probability found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (uvToken->Next() != TOKEN_NUMBER)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected north polar region probability found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	uvToken->GetFloat(m_meridian_prob[REGION_TYPE_NORTH_POLAR]) ;

	if (uvToken->Next() != TOKEN_NUMBER)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected north polar region phase bonus found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	uvToken->GetFloat(m_meridian_phase_bonus[REGION_TYPE_NORTH_POLAR]) ;

	
	
	if (uvToken->Next() != TOKEN_UV_UPPER_NORTH_MERIDIAN_PROB)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected upper north meridian region probability found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (uvToken->Next() != TOKEN_NUMBER)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected upper north meridian region probability found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	uvToken->GetFloat(m_meridian_prob[REGION_TYPE_UPPER_NORTH_MERIDIAN]) ;

	if (uvToken->Next() != TOKEN_NUMBER)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected upper north meridian region phaes bonus found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	uvToken->GetFloat(m_meridian_phase_bonus[REGION_TYPE_UPPER_NORTH_MERIDIAN]) ;

	
	
	if (uvToken->Next() != TOKEN_UV_LOWER_NORTH_MERIDIAN_PROB)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected lower north meridian region probability found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (uvToken->Next() != TOKEN_NUMBER)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected lower north meridian region probability found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	uvToken->GetFloat(m_meridian_prob[REGION_TYPE_LOWER_NORTH_MERIDIAN]) ;

	if (uvToken->Next() != TOKEN_NUMBER)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected lower north meridian region phaes bonus found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	uvToken->GetFloat(m_meridian_phase_bonus[REGION_TYPE_LOWER_NORTH_MERIDIAN]) ;

	
	
	if (uvToken->Next() != TOKEN_UV_EQUATORIAL_PROB)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected equatorial region probability found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (uvToken->Next() != TOKEN_NUMBER)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected equatorial region probability found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	uvToken->GetFloat(m_meridian_prob[REGION_TYPE_EQUATORIAL]) ;

	if (uvToken->Next() != TOKEN_NUMBER)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected equatorial region phaes bonus found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	uvToken->GetFloat(m_meridian_phase_bonus[REGION_TYPE_EQUATORIAL]) ;

	
	
	if (uvToken->Next() != TOKEN_UV_UPPER_SOUTH_MERIDIAN_PROB)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected upper south meridian region probability found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (uvToken->Next() != TOKEN_NUMBER)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected upper south meridian region probability found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	uvToken->GetFloat(m_meridian_prob[REGION_TYPE_UPPER_SOUTH_MERIDIAN]) ;

	if (uvToken->Next() != TOKEN_NUMBER)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected upper south meridian region phaes bonus found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	uvToken->GetFloat(m_meridian_phase_bonus[REGION_TYPE_UPPER_SOUTH_MERIDIAN]) ;


	
	
	if (uvToken->Next() != TOKEN_UV_LOWER_SOUTH_MERIDIAN_PROB)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected lower south meridian region probability found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (uvToken->Next() != TOKEN_NUMBER)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected lower south meridian region probability found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	uvToken->GetFloat(m_meridian_prob[REGION_TYPE_LOWER_SOUTH_MERIDIAN]) ;

	if (uvToken->Next() != TOKEN_NUMBER)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected lower south meridian region phaes bonus found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	uvToken->GetFloat(m_meridian_phase_bonus[REGION_TYPE_LOWER_SOUTH_MERIDIAN]) ;

	
	
	if (uvToken->Next() != TOKEN_UV_SOUTH_POLAR_PROB)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected south polar region probability found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (uvToken->Next() != TOKEN_NUMBER)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected south polar region probability found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	uvToken->GetFloat(m_meridian_prob[REGION_TYPE_SOUTH_POLAR]) ;

	if (uvToken->Next() != TOKEN_NUMBER)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected south polar region phase bonus found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	uvToken->GetFloat(m_meridian_phase_bonus[REGION_TYPE_SOUTH_POLAR]) ;


	
	
	if (uvToken->Next() != TOKEN_CLOSE_BRACE)
		return (FALSE) ;

	return (TRUE) ;
	}








sint32 UVRecord::ParseUVDeadTilesRecord(Token *uvToken)
	{
	if (!token_ParseAnOpenBraceNext(uvToken))
		return (FALSE) ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_TILE_DESERT)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead desert tile production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadTileNumbers(uvToken, m_dead_food[DEAD_TILE_TYPE_DESERT], m_dead_shield[DEAD_TILE_TYPE_DESERT], m_dead_trade[DEAD_TILE_TYPE_DESERT]))
		return (FALSE) ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_TILE_FORESTS)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead forests tile production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadTileNumbers(uvToken, m_dead_food[DEAD_TILE_TYPE_FORESTS], m_dead_shield[DEAD_TILE_TYPE_FORESTS], m_dead_trade[DEAD_TILE_TYPE_FORESTS]))
		return (FALSE) ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_TILE_GRASSLANDS)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead grasslands tile production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadTileNumbers(uvToken, m_dead_food[DEAD_TILE_TYPE_GRASSLANDS], m_dead_shield[DEAD_TILE_TYPE_GRASSLANDS], m_dead_trade[DEAD_TILE_TYPE_GRASSLANDS]))
		return (FALSE) ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_TILE_HILLS)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead hills tile production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadTileNumbers(uvToken, m_dead_food[DEAD_TILE_TYPE_HILLS], m_dead_shield[DEAD_TILE_TYPE_HILLS], m_dead_trade[DEAD_TILE_TYPE_HILLS]))
		return (FALSE) ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_TILE_MOUNTAINS)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead mountains tile production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadTileNumbers(uvToken, m_dead_food[DEAD_TILE_TYPE_MOUNTAINS], m_dead_shield[DEAD_TILE_TYPE_MOUNTAINS], m_dead_trade[DEAD_TILE_TYPE_MOUNTAINS]))
		return (FALSE) ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_TILE_PLAINS)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead plains tile production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadTileNumbers(uvToken, m_dead_food[DEAD_TILE_TYPE_PLAINS], m_dead_shield[DEAD_TILE_TYPE_PLAINS], m_dead_trade[DEAD_TILE_TYPE_PLAINS]))
		return (FALSE) ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_TILE_SWAMPS)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead swamps tile production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadTileNumbers(uvToken, m_dead_food[DEAD_TILE_TYPE_SWAMPS], m_dead_shield[DEAD_TILE_TYPE_SWAMPS], m_dead_trade[DEAD_TILE_TYPE_SWAMPS]))
		return (FALSE) ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_TILE_TUNDRA)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead tundra tile production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadTileNumbers(uvToken, m_dead_food[DEAD_TILE_TYPE_TUNDRA], m_dead_shield[DEAD_TILE_TYPE_TUNDRA], m_dead_trade[DEAD_TILE_TYPE_TUNDRA]))
		return (FALSE) ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_TILE_CONTINENTAL_SHELF)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead continental shelf tile production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadTileNumbers(uvToken, m_dead_food[DEAD_TILE_TYPE_CONTINENTAL_SHELF], m_dead_shield[DEAD_TILE_TYPE_CONTINENTAL_SHELF], m_dead_trade[DEAD_TILE_TYPE_CONTINENTAL_SHELF]))
		return (FALSE) ;

	
	if (uvToken->Next() != TOKEN_CLOSE_BRACE)
		return (FALSE) ;

	return (TRUE) ;
	}









sint32 UVRecord::ParseDeadTileNumbers(Token *uvToken, sint32 &food, sint32 &shield, sint32 &trade)
	{
	if (uvToken->Next() != TOKEN_NUMBER)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead tile food production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	uvToken->GetNumber(food) ;

	if (uvToken->Next() != TOKEN_NUMBER)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead tile shield production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}
	
	uvToken->GetNumber(shield) ;

	if (uvToken->Next() != TOKEN_NUMBER)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead tile trade production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}
	
	uvToken->GetNumber(trade) ;

	return (TRUE) ;
	}









sint32 UVRecord::ParseUVDeadResourcesRecord(Token *uvToken)
	{
	sint32	i = 0 ;

	if (!token_ParseAnOpenBraceNext(uvToken))
		return (FALSE) ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_PALM_OIL)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead palm oil resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_OIL)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead oil resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_PHEASANT)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead pheasant resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_SILK)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead silk resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_SPICE)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead spice resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_TUSKS)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead tusks resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_BLUBBER)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead blubber resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_ALIEN_CORPSES)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead alien corpses resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_CLAY)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead clay resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_TOBACCO)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead tobacco resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_COTTON)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead cotton resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_COAL)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead coal resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_GRAPES)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead grapes oil resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_GRANITE)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead granite oil resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_BANANAS)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead bananas resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_JADE)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead jade resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_HARDWOOD)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead hardwood resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_IRON)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead iron resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_GOLD)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead gold resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_SILVER)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead silver resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_BUFFALO)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead buffalo resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_WHEAT)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead wheat resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_CORN)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead corn resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_PEAT)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead peat resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_MANGROVE)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead mangrove resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_MINK)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead mink resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_CARIBOU)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead caribou resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_FISH)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead fish resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_WHALES)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead whales resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_CRABS)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead crabs resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_DIAMONDS)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead diamonds resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_MAGNESIUM_NODULE)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead magnesium nodule resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_PHOSPHORUS)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead phosphorus resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_PLANKTON_UPWELLING)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead plankton upwelling resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_GIANT_CLAM)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead giant clam resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	i++ ;

	
	if (uvToken->Next() != TOKEN_UV_DEAD_RESOURCE_GIANT_SQUID)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead giant squid `resource production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	if (!ParseDeadResourceNumbers(uvToken, m_dead_food[i], m_dead_shield[i], m_dead_trade[i], m_dead_gold[i]))
		return (FALSE) ;

	
	if (uvToken->Next() != TOKEN_CLOSE_BRACE)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "missing closing brace") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	return (TRUE) ;
	}









sint32 UVRecord::ParseDeadResourceNumbers(Token *uvToken, sint32 &food, sint32 &shield, sint32 &trade, sint32 &gold)
	{
	if (uvToken->Next() != TOKEN_NUMBER)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead tile food production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	uvToken->GetNumber(food) ;

	if (uvToken->Next() != TOKEN_NUMBER)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead tile shield production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}
	
	uvToken->GetNumber(shield) ;

	if (uvToken->Next() != TOKEN_NUMBER)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead tile trade production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}
	
	uvToken->GetNumber(trade) ;

	if (uvToken->Next() != TOKEN_NUMBER)
		{
		c3errors_ErrorDialog (uvToken->ErrStr(), "expected dead tile gold base production not found") ;
		g_parse_uv_abort = TRUE ;

		return (FALSE) ;
		}

	uvToken->GetNumber(food) ;

	return (TRUE) ;
	}












void UVRecord::Serialize(CivArchive &archive)
	{
	if (archive.IsStoring())
		{
		Record::Serialize(archive);
		archive.StoreChunk((uint8 *)&m_meridian_prob, ((uint8 *)&m_dead_gold)+sizeof(m_dead_gold));

		}
	else
		{
		Record::Serialize(archive);
		archive.LoadChunk((uint8 *)&m_meridian_prob, ((uint8 *)&m_dead_gold)+sizeof(m_dead_gold));

		}

	}
#endif
