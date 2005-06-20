#ifdef HAVE_PRAGMA_ONCE
#pragma once
#endif
#ifndef __SLIC_TRIGGER_LISTS_H__
#define __SLIC_TRIGGER_LISTS_H__

enum TRIGGER_LIST {
	TRIGGER_LIST_YEARLY,
	TRIGGER_LIST_PLAYER,
	TRIGGER_LIST_DISCOVERY,
	TRIGGER_LIST_DISCOVERY_TRADED,
	TRIGGER_LIST_CITY_BUILT,
	TRIGGER_LIST_CITY_BUILDINGNOTHING,
	TRIGGER_LIST_CITY_PRODUCTION,
	TRIGGER_LIST_CITY_PRODUCTION_UNIT,
	TRIGGER_LIST_CITY_PRODUCTION_WONDER,
	TRIGGER_LIST_CITY_PRODUCTION_POP,
	TRIGGER_LIST_CITY_PRODUCTION_ENDGAME,
	TRIGGER_LIST_CITY_QUEUE,
	TRIGGER_LIST_CITY,
	TRIGGER_LIST_CITY_POP,
	TRIGGER_LIST_UNIT_BEGIN_TURN,
	TRIGGER_LIST_WONDER_STARTED,
	TRIGGER_LIST_WONDER_FINISHED,

	TRIGGER_LIST_CLICKED_UNIT,
	TRIGGER_LIST_CLICKED_UNEXPLORED,
	TRIGGER_LIST_CLICKED_CITY,

	TRIGGER_LIST_CITY_SELECTED,
	TRIGGER_LIST_UNIT_SELECTED,
	TRIGGER_LIST_UNIT_DESELECTED,
	TRIGGER_LIST_CITY_DESELECTED,
	TRIGGER_LIST_IDLE,
	TRIGGER_LIST_ATTACK,
	TRIGGER_LIST_ALL_UNITS_MOVED,
	TRIGGER_LIST_UNIT_MOVED,
	TRIGGER_LIST_ZOC,
	TRIGGER_LIST_UNIT_DONE_MOVING,
	TRIGGER_LIST_UNIT_DEAD,
	TRIGGER_LIST_UNIT_DEAD_OUTOFFUEL,
	TRIGGER_LIST_UNIT_DEAD_CANT_SUPPORT,
	TRIGGER_LIST_UNIT_DEAD_MISC,
	TRIGGER_LIST_CITY_CAPTURED,
	TRIGGER_LIST_CONTACT,
	TRIGGER_LIST_SIGHTED_UNIT,
	TRIGGER_LIST_UNIT_BUILT,
	TRIGGER_LIST_BUILDING_BUILT,
	TRIGGER_LIST_SAME_GOOD,
	TRIGGER_LIST_SAME_GOOD_AS_TRADED,
	TRIGGER_LIST_TRADE_SCREEN,
	TRIGGER_LIST_DIPLOMATIC_SCREEN,

	TRIGGER_LIST_UNIT_QUEUE,
	TRIGGER_LIST_PRODUCTION_QUEUE,
	TRIGGER_LIST_CREATE_STACK,
	TRIGGER_LIST_CREATE_MIXED_STACK,
	TRIGGER_LIST_LOCK_STACK,
	TRIGGER_LIST_AUTO_ARRANGE_OFF,
	TRIGGER_LIST_BOMBARDMENT,
	TRIGGER_LIST_COUNTER_BOMBARDMENT,
	TRIGGER_LIST_ACTIVE_DEFENSE,
	TRIGGER_LIST_INDULGENCES,
	TRIGGER_LIST_TERRORISM,
	TRIGGER_LIST_CONVERSION,
	TRIGGER_LIST_UNIT_LAUNCHED,
	TRIGGER_LIST_ENSLAVEMENT,
	TRIGGER_LIST_SETTLERENSLAVED,
	TRIGGER_LIST_VICTORYENSLAVEMENT,
	TRIGGER_LIST_POP_MOVED,
	TRIGGER_LIST_POP_MOVED_OFF_GOOD,
	TRIGGER_LIST_POP_ON_GOOD,
	TRIGGER_LIST_BUILD_FARM,
	TRIGGER_LIST_BUILD_ROAD,
	TRIGGER_LIST_BUILD_MINE,
	TRIGGER_LIST_BUILD_INSTALLATION,
	TRIGGER_LIST_BUILD_TRANSFORM,
	TRIGGER_LIST_SCIENCE_RATE,
	TRIGGER_LIST_PUBLIC_WORKS_TAX,
	TRIGGER_LIST_TRADE_OFFER,
	TRIGGER_LIST_TREATY_BROKEN,
	TRIGGER_LIST_WONDER_ALMOST_DONE,
	TRIGGER_LIST_GOVERNMENT_CHANGED,
	TRIGGER_LIST_TRADE_ROUTE,
	TRIGGER_LIST_FOREIGN_TRADE_ROUTE,
	TRIGGER_LIST_PIRACY,
	TRIGGER_LIST_PILLAGE,
	TRIGGER_LIST_CANT_SETTLE_MOVEMENT,
	TRIGGER_LIST_AGE_CHANGE,
	TRIGGER_LIST_TIMER,
	TRIGGER_LIST_WORK_VIEW,
	TRIGGER_LIST_CEASE_FIRE,
	TRIGGER_LIST_EMBARKED,
	TRIGGER_LIST_DEBARKED,
	TRIGGER_LIST_FORTIFY,
	TRIGGER_LIST_READINESS,
	TRIGGER_LIST_STACKED_COMBAT,
	TRIGGER_LIST_TUTORIAL_OFF,
	TRIGGER_LIST_SINGLE_COMBAT,

	TRIGGER_LIST_KEY_PRESSED,

	TRIGGER_LIST_GAME_LOADED,

	TRIGGER_LIST_PLAYER_RESEARCHING,

	TRIGGER_LIST_MAX
};

#endif
