











#pragma once
#ifndef __TOKEN_H__
#define __TOKEN_H__ 1

#define k_MAX_TOKEN_LEN _MAX_PATH

enum C3DIR;
#include "dbtypes.h"

enum TOKEN_TYPES { 
	
	
	TOKEN_STRING,  
		TOKEN_QUOTED_STRING,   
		TOKEN_MISSING_QUOTE,   
		
		TOKEN_NUMBER, 
		TOKEN_EOF, 
		
		TOKEN_UNKNOWN,   
		
		TOKEN_OPEN_BRACE,  
		TOKEN_CLOSE_BRACE,  
		
		TOKEN_IMPORT,
		
		
		TOKEN_ENABLING_ADVANCE,  
		TOKEN_OBSOLETE_ADVANCE, 
		TOKEN_ADVANCE_POLLUTION_SIZE_MODIFIER,
		TOKEN_ADVANCE_POLLUTION_PRODUCTION_MODIFIER,
		TOKEN_ADVANCE_IS_GENETIC,
		TOKEN_ADVANCE_ALLOWS_ROADS,
		TOKEN_ADVANCE_ALLOWS_RAILROADS,
		TOKEN_ADVANCE_ALLOWS_MAGLEV,
		TOKEN_ADVANCE_ALLOWS_IRRIGATION1,
		TOKEN_ADVANCE_ALLOWS_IRRIGATION2,
		TOKEN_ADVANCE_ALLOWS_IRRIGATION3,
		TOKEN_ADVANCE_ALLOWS_MINE1,
		TOKEN_ADVANCE_ALLOWS_MINE2,
		TOKEN_ADVANCE_ALLOWS_MINE3,
		TOKEN_ADVANCE_ALLOWS_CANAL_TUNNEL,
		TOKEN_ADVANCE_ALLOWS_TRANSFORM,
		TOKEN_ADVANCE_CIVILIZATION,
		TOKEN_ADVANCE_COST,
		TOKEN_ADVANCE_ALLOWS_DEEP_OCEAN,
		TOKEN_ADVANCE_ALLOWS_CAPITALIZATION,
		TOKEN_ADVANCE_ALLOWS_INFRASTRUCTURE,
	    TOKEN_ADVANCE_ALLOWS_ALIEN_LIFE,
	    TOKEN_ADVANCE_REQUIRES_PROBE_RECOVERY,
		TOKEN_ADVANCE_ALLOWS_NUKE,

		TOKEN_NULL, 
		
		TOKEN_SHIELD_COST,
		TOKEN_POWER_POINTS,
		TOKEN_ADVANCE_AGE_INDEX,
		
		TOKEN_WONDER_POLLUTION_CLEANER,
		
		TOKEN_WONDER_POLLUTION_BONUS,
		
		
		
		TOKEN_MAX_HP, 
		TOKEN_ATTACK, 
		TOKEN_DEFENSE, 
		TOKEN_FIREPOWER, 
		TOKEN_ZB_RANGE_ATTACK,
		
		TOKEN_BATTLEFIELD_RANGE, 
		TOKEN_BATTLEFIELD_RADIUS, 
		TOKEN_VISION_RANGE, 
		TOKEN_ACTIVE_DEFENSE_RANGE, 
		TOKEN_ELECTRONIC_COMBAT_FACTOR, 
		TOKEN_MAX_MOVEMENT, 
		TOKEN_FUEL,
		
		TOKEN_FOOD_HUNGER, 
		TOKEN_SHIELD_HUNGER,
		TOKEN_DEFAULT_SPRITE,
		TOKEN_DEFAULT_ICON,
		TOKEN_DESCRIPTION,
		
		TOKEN_SETTLE_CITY_TYPE, 
		
		TOKEN_SOUND_SELECT1,
		TOKEN_SOUND_SELECT2,
		TOKEN_SOUND_MOVE,
		TOKEN_SOUND_ACKNOWLEDGE,
		TOKEN_SOUND_CANTMOVE,
		TOKEN_SOUND_ATTACK,
		
		TOKEN_SOUND_WORK,
		TOKEN_SOUND_VICTORY,
		TOKEN_SOUND_DEATH,
		
		
		TOKEN_MOVEMENT_TYPE_LAND, 
		TOKEN_MOVEMENT_TYPE_WATER, 
		TOKEN_MOVEMENT_TYPE_SPACE, 
		TOKEN_MOVEMENT_TYPE_AIR, 
		TOKEN_MOVEMENT_TYPE_MOUNTAIN, 
		TOKEN_MOVEMENT_TYPE_TRADE,
		TOKEN_MOVEMENT_TYPE_SHALLOW_WATER,
		
		TOKEN_LOSS_MOVE_TO_DMG_NONE,
		TOKEN_LOSS_MOVE_TO_DMG_TWO, 
		TOKEN_TRANSFORM_UNIT,
		
		TOKEN_NO_FUEL_THEN_CRASH,
		
		TOKEN_CAN_CARRY_SMALL_LAND,  
		TOKEN_CAN_CARRY_MED_LAND,      
		TOKEN_CAN_CARRY_LARGE_LAND,
		
		TOKEN_CAN_CARRY_SMALL_AIR,   
		TOKEN_CAN_CARRY_MED_AIR,     
		TOKEN_CAN_CARRY_LARGE_AIR,   
		
		TOKEN_CAN_CARRY_SMALL_WATER, 
		TOKEN_CAN_CARRY_MED_WATER,
		TOKEN_CAN_CARRY_LARGE_WATER,
		
		TOKEN_CAN_CARRY_SMALL_SPACE, 
		TOKEN_CAN_CARRY_MED_SPACE,
		TOKEN_CAN_CARRY_LARGE_SPACE,
		TOKEN_CAN_LIFT_OFF,
		
		TOKEN_PRIMARY_PROJECTILE, 
		TOKEN_SECONDARY_PROJECTILE, 
		TOKEN_PROJECTILE_PATH, 
		
		TOKEN_SIZE_SMALL,   
		TOKEN_SIZE_MED, 
		TOKEN_SIZE_LARGE, 
		
		TOKEN_NCARGO, 
		
		
		TOKEN_SETTLE_LAND, 
		TOKEN_SETTLE_MOUNTAINS, 
		TOKEN_SETTLE_WATER, 
		TOKEN_SETTLE_SPACE, 
		
		TOKEN_CANT_CAPTURE_CITY,
		TOKEN_HAS_POP_AND_CAN_BUILD,
		TOKEN_IS_TRADER,
		
		TOKEN_LAUNCH_DESTROYS_OZONE,
		TOKEN_DEATH_INCREASES_GLOBAL_WARMING,
		
		TOKEN_VISIBILITY_CLASS_0,
		TOKEN_VISIBILITY_CLASS_1,
		TOKEN_VISIBILITY_CLASS_2,
		TOKEN_VISIBILITY_CLASS_3,
		TOKEN_VISIBILITY_CLASS_4,
		TOKEN_VISIBILITY_CLASS_5,
		TOKEN_VISIBILITY_CLASS_6,
		TOKEN_VISIBILITY_CLASS_7,
		TOKEN_VISIBILITY_CLASS_8,
		
		TOKEN_VISIBILITY_CAN_SEE_0,
		TOKEN_VISIBILITY_CAN_SEE_1,
		TOKEN_VISIBILITY_CAN_SEE_2,
		TOKEN_VISIBILITY_CAN_SEE_3,
		TOKEN_VISIBILITY_CAN_SEE_4,
		TOKEN_VISIBILITY_CAN_SEE_5,
		TOKEN_VISIBILITY_CAN_SEE_6,
		TOKEN_VISIBILITY_CAN_SEE_7,
		TOKEN_VISIBILITY_CAN_SEE_8,
		
		TOKEN_IS_SPACE_LAUNCH,  
		TOKEN_IS_SPACE_LAND, 
		TOKEN_EXERTS_MARTIAL_LAW,
		TOKEN_CANT_ENTRENCH,
		TOKEN_CANT_PATROL,
		TOKEN_NEEDS_NO_SUPPORT, 
		TOKEN_CAN_EXPEL_POP,
		TOKEN_IS_FLANKER,
		TOKEN_DEATH_EFFECTS_HAPPY,
		TOKEN_CAN_BEACH_ASSAULT,
		
		
		TOKEN_IGNORE_ZOC,
		TOKEN_HAS_NO_ZOC,
		TOKEN_CAN_BE_EXPELLED,
		TOKEN_CAN_BE_SUED,
		
		TOKEN_ATTACK_FROM_SPACESHIP,
		TOKEN_CANT_BE_ATTACKED,
		TOKEN_CAN_BOMBARD_AIR,
		TOKEN_CAN_BOMBARD_LAND,
		TOKEN_CAN_BOMBARD_WATER,
		TOKEN_CAN_BOMBARD_SPACE,
		TOKEN_CAN_BOMBARD_MOUNTAIN,
		TOKEN_BOMBARD_ROUNDS,
		TOKEN_CAN_COUNTER_BOMBARD,
		
		TOKEN_DEFEND_LAND,
		TOKEN_DEFEND_AIR,
		TOKEN_DEFEND_WATER,
		TOKEN_DEFEND_SPACE,
		TOKEN_DEFEND_MOUNTAIN,
		
		TOKEN_DEFEND_AGAINST_SPIES,
		
		TOKEN_IS_MOUNTED,
		TOKEN_IS_CARGO_POD,
		TOKEN_IGNORE_CITY_WALLS,
		TOKEN_IS_SUBMARINE,
		
		TOKEN_NO_LAND_ATTACK,
		TOKEN_NO_SEA_ATTACK,
		TOKEN_NO_AIR_ATTACK,
		TOKEN_NO_MOUNTAIN_ATTACK,
		TOKEN_NO_SPACE_ATTACK,
		TOKEN_NO_SHALLOW_WATER_ATTACK,
		TOKEN_NO_TRADE_ATTACK,
		TOKEN_NO_UNDERWATER_ATTACK,
		
		TOKEN_LAND_ATTACK,
		TOKEN_SEA_ATTACK,
		TOKEN_AIR_ATTACK,
		TOKEN_MOUNTAIN_ATTACK,
		TOKEN_SPACE_ATTACK,
		TOKEN_SHALLOW_WATER_ATTACK,
		TOKEN_TRADE_ATTACK,
		TOKEN_UNDERWATER_ATTACK,
		
		TOKEN_PARATROOPER,
		TOKEN_ASSISTED_DROPS,
		TOKEN_BONUS_AGAINST_MOUNTED,
		TOKEN_SINGLE_USE,
		TOKEN_BONUS_AIR_DEFENSE,
		
		TOKEN_WOODEN_SHIP,
		TOKEN_ATTACK_WOODEN_SHIP_BONUS,
		TOKEN_ATTACK_CITY_BONUS,
		TOKEN_ATTACK_BONUS_SUBMARINE,
		
		TOKEN_REPLACE_WITH_CARGO_MODULE,
		TOKEN_VISIBILE_ATTACKING,
		
		TOKEN_NUCLEAR_ATTACK,
		
		TOKEN_CAN_SUE,
		TOKEN_IS_STEALTHY,
		TOKEN_SEE_SURFACE_FROM_SPACE,
		
		TOKEN_ESTABLISH_EMBASSY,
		TOKEN_THROW_PARTY,
		TOKEN_HEAR_GOSSIP,
		
		TOKEN_INVESTIGATE_CITY,
		TOKEN_STEAL_TECHNOLOGY,
		TOKEN_INCITE_REVOLUTION,
		TOKEN_ASSASINATE_RULER,
		TOKEN_INVESTIGATE_READINESS,
		TOKEN_NULLIFY_CITY_WALLS,
		
		TOKEN_CREATE_FRANCHISE,
		TOKEN_CAUSE_UNHAPPINESS,
		
		TOKEN_CONDUCT_HITS,
		TOKEN_BIO_TERROR,
		TOKEN_NANO_TERROR,
		
		TOKEN_SLAVE_RAIDS,
		TOKEN_SETTLER_SLAVE_RAIDS,
		TOKEN_SLAVE_UPRISING,
		TOKEN_UNDERGROUND_RAILWAY,
		TOKEN_VICTORY_ENSLAVEMENT,
		
		TOKEN_DEFUSE_LAND_MINES,
		TOKEN_DEFUSE_SEA_MINES,
		TOKEN_DEFUSE_XRAY_MINES,
		
		TOKEN_INDULGENCE_SALES,
		TOKEN_CONVERT_CITIES,
		TOKEN_BUILD_XRAY_MINES,
		
		TOKEN_BUILD_WONDER,
		TOKEN_GOVERNMENT_TYPE,
		TOKEN_NO_SLAVES,
		TOKEN_CREATE_RIFT,
		TOKEN_CAN_CLOAK,
		
		TOKEN_PARATROOPER_TRANSPORT,
		TOKEN_CANT_BUILD,
		TOKEN_PLANT_NUKE,
		TOKEN_IS_TELEVANGELIST,
		TOKEN_CAN_SOOTHSAY,
		TOKEN_CAN_BE_RUSTLED,
		TOKEN_CREATE_PARKS,
		TOKEN_INJUNCTIONS,
		TOKEN_LAND_CITY_CAN_BUILD,
		TOKEN_SEA_CITY_CAN_BUILD,
		TOKEN_SPACE_CITY_CAN_BUILD,
		TOKEN_BOMBARD_RANGE,
		TOKEN_IS_SPECIAL_FORCES,
		TOKEN_WORMHOLE_PROBE,
		TOKEN_NOT_AFFECTED_BY_LAND_MINES,
		
		TOKEN_ENABLES_CARRIER_ACTIVE_DEFENSE,
		TOKEN_ACTIVE_DEFENSE_ONLY_WHEN_CARRYING_ENABLERS,
		TOKEN_BUILDING_REMOVES_A_POP,
		TOKEN_UNIT_EXPLODES,
		TOKEN_UNIT_ADVERTISES,
		TOKEN_UNIT_CAN_RUSTLE,
	    TOKEN_ONLY_BUILD_ONE,
	    TOKEN_CAN_REFORM,
	    TOKEN_CAN_PILLAGE,
	    TOKEN_CAN_PIRATE,
	    TOKEN_CAN_EXPEL,
	    TOKEN_CITY_GROWTH_MODIFIER,
	    TOKEN_SET_USER_FLAG,
		
		
		TOKEN_REVOLUTION,
		
		
		
		TOKEN_BONUS_TRADE, 
		TOKEN_BONUS_SHIELD,
		TOKEN_BONUS_FOOD, 
		
		TOKEN_HUNGER,
		TOKEN_BONUS_GOLD,
		TOKEN_BONUS_LUXURY,
		TOKEN_BONUS_SCIENCE,
		TOKEN_KILL_PREF,
		TOKEN_TYPE_PREF,
		TOKEN_MIN_SIZE,
		TOKEN_MAX_SIZE,
		TOKEN_IMPROVEMENT,
		
		TOKEN_ATTITUDE_NO_EFFECT,
		TOKEN_CAN_WORK_FIELDS,
		TOKEN_AFFECTS_CONTENTMENT_TOTAL,
		TOKEN_IS_UNPAID,
		TOKEN_CAN_WORK_CITY,
		TOKEN_GOVERNMENT,
		TOKEN_ADD_HAPPY_ADJUST,
		TOKEN_REMOVE_HAPPY_ADJUST,
		TOKEN_IS_SLAVE,
		
		
		TOKEN_BASE_CONTENTMENT, 
		TOKEN_GRANARY_COEF,    
		TOKEN_BIG_CITY_SCALE,
		TOKEN_BIG_CITY_OFFSET,
		TOKEN_SCIENCE_HANDICAP,
		
		TOKEN_DIFFICULTY_POLLUTION_MULTIPLIER,
		TOKEN_DIFFICULTY_RIOT_CHANCE,
		TOKEN_DIFFICULTY_STARVATION_EFFECT,
		TOKEN_DIFFICULTY_STARTING_GOLD,
		TOKEN_DIFFICULTY_VISION_BONUS,
		TOKEN_DIFFICULTY_BASE_SCORE,
		
		
		TOKEN_TIME_SCALE,
		TOKEN_START_YEAR,
		TOKEN_NUM_PERIODS,
		TOKEN_PERIOD,
		TOKEN_START_TURN,
		TOKEN_YEARS_PER_TURN,
	    TOKEN_NEGATIVE_YEAR_FORMAT,
	    TOKEN_POSITIVE_YEAR_FORMAT,

		TOKEN_CELEBRATION_FACTOR,
		TOKEN_ADVANCES_FACTOR,
		TOKEN_WONDERS_FACTOR,
		TOKEN_POPULATION_FACTOR,
		TOKEN_CITY_FACTOR,
		TOKEN_PEACE_FACTOR,
		TOKEN_TURN_FACTOR,
		TOKEN_DIFFICULTY_BONUS,
		TOKEN_MAP_FACTOR,
		TOKEN_OPPONENTS_FACTOR,
		TOKEN_WONDER_VICTORY_BONUS,
		TOKEN_UNIT_FACTOR,
		TOKEN_RIOT_FACTOR,
		TOKEN_REVOLUTION_FACTOR,
		TOKEN_POLLUTION_FACTOR,
		
		TOKEN_AI_START_UNITS,
		TOKEN_AI_START_GOLD,
		TOKEN_AI_START_ADVANCES,
		TOKEN_AI_START_PUBLIC_WORKS,
		TOKEN_HUMAN_START_LOCATION,
		TOKEN_AI_INTELLIGENCE_FACTOR,
		TOKEN_AI_GANG_UP_FACTOR,
		TOKEN_DISTANCE_FROM_CAPITOL_ADJUSTMENT,
		TOKEN_AI_DISTANCE_FROM_CAPITOL_ADJUSTMENT,
		TOKEN_POLLUTION_ADJUST,
		TOKEN_AI_MIN_BEHIND_PERCENT,
		TOKEN_AI_MAX_BEHIND_PERCENT,
		TOKEN_AI_MIN_AHEAD_PERCENT,
		TOKEN_AI_MAX_AHEAD_PERCENT,
		TOKEN_AI_MIN_BEHIND_TECHNOLOGY_COST,
		TOKEN_AI_MAX_BEHIND_TECHNOLOGY_COST,
		TOKEN_AI_MIN_BEHIND_PRODUCTION_COST_ADJUSTMENT,
		TOKEN_AI_MAX_BEHIND_PRODUCTION_COST_ADJUSTMENT,
		TOKEN_AI_MIN_BEHIND_GOLD_ADJUSTMENT,
		TOKEN_AI_MAX_BEHIND_GOLD_ADJUSTMENT,
		TOKEN_AI_MIN_AHEAD_TECHNOLOGY_COST,
		TOKEN_AI_MAX_AHEAD_TECHNOLOGY_COST,
		TOKEN_AI_MIN_AHEAD_PRODUCTION_COST_ADJUSTMENT,
		TOKEN_AI_MAX_AHEAD_PRODUCTION_COST_ADJUSTMENT,
		TOKEN_AI_MIN_AHEAD_GOLD_ADJUSTMENT,
		TOKEN_AI_MAX_AHEAD_GOLD_ADJUSTMENT,
		TOKEN_MAX_HUMAN_ADVANCES,
		TOKEN_MAX_AI_ADVANCES,
		TOKEN_ADVANCE_CHANCES,
	    TOKEN_HUMAN_SCIENCE_BONUS,
	    TOKEN_HUMAN_FOOD_BONUS,
	    TOKEN_EXTRA_SETTLER_CHANCE,
	    TOKEN_MAX_SCORE,

	    
		TOKEN_UNIT_SPRITE,
		TOKEN_PROJECTILE_SPRITE,
		TOKEN_EFFECT_SPRITE,
		
		
		TOKEN_SPRITE_NUM_FRAMES,
		TOKEN_SPRITE_FIRST_FRAME,
		TOKEN_SPRITE_WIDTH,
		TOKEN_SPRITE_HEIGHT,
		TOKEN_SPRITE_HOT_POINT,
		TOKEN_SPRITE_HOT_POINTS,
		
		TOKEN_UNIT_SPRITE_MOVE,
		TOKEN_UNIT_SPRITE_ATTACK,
		TOKEN_UNIT_SPRITE_VICTORY,
		TOKEN_UNIT_SPRITE_IDLE,
		TOKEN_UNIT_SPRITE_WORK,
		TOKEN_UNIT_SPRITE_FIREPOINTS,
		TOKEN_UNIT_SPRITE_FIREPOINTS_WORK,
		TOKEN_UNIT_SPRITE_MOVEOFFSETS,
		TOKEN_UNIT_SPRITE_SHIELDPOINTS,
		TOKEN_UNIT_SPRITE_SHIELDPOINTS_MOVE,
		TOKEN_UNIT_SPRITE_SHIELDPOINTS_ATTACK,
		TOKEN_UNIT_SPRITE_SHIELDPOINTS_IDLE,
		TOKEN_UNIT_SPRITE_SHIELDPOINTS_VICTORY,
		TOKEN_UNIT_SPRITE_SHIELDPOINTS_WORK,
		TOKEN_UNIT_SPRITE_IS_DEATH,
		TOKEN_UNIT_SPRITE_ATTACK_IS_DIRECTIONAL,
		
		TOKEN_PROJECTILE_SPRITE_MOVE,
		TOKEN_PROJECTILE_SPRITE_FIREPOINTS,
		TOKEN_PROJECTILE_SPRITE_MOVEOFFSETS,
		
		TOKEN_EFFECT_SPRITE_PLAY,
		TOKEN_EFFECT_SPRITE_FLASH,
		TOKEN_EFFECT_SPRITE_FIREPOINTS,
		TOKEN_EFFECT_SPRITE_MOVEOFFSETS,
		
		TOKEN_GOOD_SPRITE,
		TOKEN_GOOD_SPRITE_IDLE,
		TOKEN_GOOD_GOLD_VALUE,
		TOKEN_GOOD_SOUND_ID,
		
		TOKEN_ANIM,
		TOKEN_ANIM_TYPE,
		TOKEN_ANIM_NUM_FRAMES,
		TOKEN_ANIM_PLAYBACK_TIME,
		TOKEN_ANIM_DELAY,
		TOKEN_ANIM_FRAME_DATA,
		TOKEN_ANIM_MOVE_DELTAS,
		TOKEN_ANIM_TRANSPARENCIES,
		
		
		
		TOKEN_TILESET_TILE,
		TOKEN_TILESET_TILE_BASE_TYPE,
		TOKEN_TILESET_TILE_TRANS_0,
		TOKEN_TILESET_TILE_TRANS_1,
		TOKEN_TILESET_TILE_TRANS_2,
		TOKEN_TILESET_TILE_TRANS_3,
		TOKEN_TILESET_MEGATILE,
		TOKEN_TILESET_MEGATILE_CONFIG,
		TOKEN_TILESET_MEGATILE_INFO,
		TOKEN_TILESET_TRANSFORM,
		TOKEN_TILESET_TRANSFORM_TO,
		TOKEN_TILESET_TRANSFORM_TO_LIST,
		TOKEN_TILESET_TRANSITION,
		TOKEN_TILESET_TRANSITION_FROM,
		TOKEN_TILESET_TRANSITION_TO,
		TOKEN_TILESET_RIVER_TRANSFORM,
		TOKEN_TILESET_RIVER_PIECE,
		TOKEN_TILESET_END,
		
		TOKEN_COLORSET,
		TOKEN_COLORSET_COLOR,
		
		TOKEN_PRODUCTION_COST,
		TOKEN_PRODUCTION_TIME,
		TOKEN_RADAR_RANGE,
		TOKEN_DEFENSE_BONUS,
		TOKEN_IS_AIRFIELD,
		TOKEN_IS_FORT,
		TOKEN_IS_LAND_MINE,
		TOKEN_IS_LISTENING_POST,
		TOKEN_IS_RADAR,
		TOKEN_IS_HEAL_UNITS,
		TOKEN_INSTALLATION_LAND,
		TOKEN_INSTALLATION_WATER,
		TOKEN_INSTALLATION_SPACE,
		
		TOKEN_FOREST_COST,
		TOKEN_PLAINS_COST,
		TOKEN_TUNDRA_COST,
		TOKEN_GLACIER_COST,
		TOKEN_GRASSLAND_COST,
		TOKEN_DESERT_COST,
		TOKEN_SWAMP_COST,
		TOKEN_JUNGLE_COST,
		TOKEN_MOUNTAIN_COST,
		TOKEN_HILL_COST,
		TOKEN_SHALLOW_COST,
		TOKEN_DEEP_COST,
		TOKEN_RIFT_COST,
		TOKEN_TRENCH_COST,
		
		TOKEN_BEACH_COST,
		TOKEN_SHELF_COST,
		TOKEN_VOLCANO_COST,
		TOKEN_SPACE_COST,
		TOKEN_DEAD_COST,
		TOKEN_DEAD_HILL_COST,
		
		TOKEN_BROWN_HILL_COST, 
		TOKEN_BROWN_MOUNTAIN_COST, 
		TOKEN_WHITE_HILL_COST, 
		TOKEN_WHITE_MOUNTAIN_COST, 
		
		TOKEN_BUILD_SOUND_ID,	
		
		
		TOKEN_WONDER_DEFAULT_ICON,
		TOKEN_WONDER_MOVIE,
		TOKEN_WONDER_DESCRIPTION,
		TOKEN_IMPROVE_DEFAULT_ICON,
		TOKEN_IMPROVE_DESCRIPTION,
		
		
		TOKEN_ADVANCE_DEFAULT_ICON,
		TOKEN_CONCEPT_DEFAULT_ICON,
		TOKEN_TERRAIN_DEFAULT_ICON,
		
		
		TOKEN_TILEIMP_DEFAULT_ICON,

		TOKEN_BARBARIAN_HUT_CHANCE,
		TOKEN_MAX_HUT_BARBARIANS,
		TOKEN_GOLD_HUT_CHANCE,
	    TOKEN_MIN_HUT_GOLD,
		TOKEN_MAX_HUT_GOLD,
		TOKEN_ADVANCE_HUT_CHANCE,
		TOKEN_MAX_HUT_ADVANCE_PREREQUISITES,
		TOKEN_UNIT_HUT_CHANCE,
		TOKEN_MAX_UNIT_PREREQUISITES,
		TOKEN_CITY_HUT_CHANCE,
		TOKEN_SETTLER_HUT_CHANCE,
		
		TOKEN_BARBARIAN_CHANCE,
		TOKEN_BARBARIAN_RANK_MAX,
		TOKEN_BARBARIAN_RANK_MIN,
	    TOKEN_MAX_SPONTANEOUS_BARBARIANS,
	    TOKEN_FIRST_BARBARIAN_TURN,
	    TOKEN_LAST_BARBARIAN_TURN,
	    TOKEN_MINIMUM_BARBARIAN_DISTANCE,

		
		TOKEN_ADVANCE_DEFAULT_BRANCH,

		
		TOKEN_ENDGAME_DEFAULT_ICON,

		TOKEN_NO_INDEX,
		TOKEN_CHEAT_INDEX,
		
		
		TOKEN_FEATS_FACTOR,
		
		
		TOKEN_CITIES0TO30_FACTOR,
		TOKEN_CITIES30TO100_FACTOR,
		TOKEN_CITIES100TO500_FACTOR,
		TOKEN_CITIES500PLUS_FACTOR,
		TOKEN_CITIES_RECAPTURED_FACTOR,
		
		TOKEN_RANK_FACTOR,
		TOKEN_OPPONENTS_CONQUERED_FACTOR,
		TOKEN_ALLIES_FACTOR,
		
		TOKEN_ALLIED_VICTORY_BONUS,
		TOKEN_SOLO_VICTORY_BONUS,

		TOKEN_POLLUTION_START_PRODUCTION_LEVEL,
		TOKEN_POLLUTION_START_POPULATION_LEVEL,
		TOKEN_POLLUTION_PRODUCTION_RATIO,
		TOKEN_POLLUTION_POPULATION_RATIO,

		
		TOKEN_CONST_LAST,
		TOKEN_MAX   
};

class StringDB; 

struct TokenData { 
	sint32 tok; 
	char keyword[k_MAX_NAME_LEN]; 
};

class Token { 
	
	FILE			*m_fin;
	char			m_filename[_MAX_PATH];
	char			m_buf[k_MAX_TOKEN_LEN]; 
	sint32			m_len, 
					m_index, 
					m_val_string_len, 
					m_current_type; 
	double			m_val_number ;
	char			m_val_string[k_MAX_TEXT_LEN]; 
	char			m_cur; 
	char			m_estr[_MAX_PATH + 10];
	
	sint32			m_num_it; 
	TokenData		*m_imported_tokens;
	
	C3DIR			m_dir;
	FILE			*m_importFile;

	MBCHAR			m_savedFilename[_MAX_PATH];
	sint32			m_savedLineNumber;
	FILE			*m_savedFin;
	
public:
	
	Token(char *fn, C3DIR dir);
	Token(char *fn, sint32, TokenData *il, C3DIR dir);
	~Token(); 
	sint32 IsWhitespace(const char c);
	sint32 IsNumber(char *str);
	sint32 GetType ();
	void NextNumber();
	void NextString();

	sint32 HandleImport(void);
	sint32 CloseImport(void);

	sint32 Next();
	void GetString (char *str);
	void GetNumber (sint32 &n); 
	void GetFloat(double &n); 
	
	char * ErrStr(); 
	
	sint32 ValidateAllTokens();
	
	char * GetKeyword(const sint32 tok) const;
	
	
	friend sint32 token_ParseValNext(Token *aToken, const sint32 t, sint32 &val);
	friend sint32 token_ParseFloatNext(Token *aToken, const sint32 t, double &val);
	
};



extern char *token_GetKeyword(const sint32 i);

sint32	token_ParseKeywordNext(Token *aToken, const sint32 t);
sint32	token_ParseValNext(Token *aToken, const sint32 t, sint32 &val);
sint32  token_ParseFloatNext(Token *aToken, const sint32 t, double &val);
sint32	token_ParseAnOpenBraceNext(Token *aToken);
sint32	token_ParseAnCloseBraceNext(Token *aToken);
sint32	token_ParseAnCloseBrace(Token *aToken);
POINT	token_ParsePoint(Token *theToken);

template <class T> class CTPDatabase;
class AdvanceRecord;

sint32 token_ParseFieldAdvance(StringDB *g_theStringDB, 
							   CTPDatabase<AdvanceRecord> *g_theAdvanceDB, Token *theToken, const sint32 keyword,
							   sint32 &a);
sint32 token_ParseFieldImprovement(Token *aToken, const sint32 keyword, sint32 &imp);


#endif
