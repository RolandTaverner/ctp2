//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ header
// Description  : Handling of the action on the screen
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
//----------------------------------------------------------------------------

#ifdef HAVE_PRAGMA_ONCE
#pragma once
#endif

#ifndef __DIRECTOR_H__
#define __DIRECTOR_H__

//----------------------------------------------------------------------------
//
// Library imports
//
//----------------------------------------------------------------------------

#include <deque>
#include <list>
#include <memory>
#include <vector>

//----------------------------------------------------------------------------
//
// Exported names
//
//----------------------------------------------------------------------------

class Director;
class DQItem;
class Sequence;

enum DQITEM_TYPE {
  DQITEM_MOVE,
  DQITEM_MOVEPROJECTILE,
  DQITEM_SPECEFFECT,
  DQITEM_ATTACK,
  DQITEM_ATTACKPOS,
  DQITEM_SPECATTACK,
  DQITEM_DEATH,
  DQITEM_MORPH,
  DQITEM_HIDE,
  DQITEM_SHOW,
  DQITEM_WORK,
  DQITEM_FASTKILL,
  DQITEM_ADDVISION,
  DQITEM_REMOVEVISION,
  DQITEM_SETOWNER,
  DQITEM_SETVISIBILITY,
  DQITEM_SETVISIONRANGE,
  DQITEM_COMBATFLASH,
  DQITEM_TELEPORT,
  DQITEM_COPYVISION,
  DQITEM_CENTERMAP,
  DQITEM_SELECTUNIT,
  DQITEM_ENDTURN,
  DQITEM_BATTLE,
  DQITEM_PLAYSOUND,
  DQITEM_PLAYWONDERMOVIE,
  DQITEM_PLAYVICTORYMOVIE,
  DQITEM_MESSAGE,
  DQITEM_FACEOFF,
  DQITEM_TERMINATE_FACEOFF,
  DQITEM_TERMINATE_SOUND,

  DQITEM_INVOKE_THRONE_ROOM,

  DQITEM_INVOKE_RESEARCH_ADVANCE,

  DQITEM_BEGIN_SCHEDULER,

  DQITEM_MAX
};


#define k_MAX_DIRECTOR_QUEUE_ITEMS 2000
#define k_MAXFRAMERATE 20

#define k_FIRSTACTOR 0
#define k_NOPROJECTILE -1

#define k_TRANSPORTREMOVEONLY -1
#define k_TRANSPORTADDONLY -2

#define k_TIME_LOG_SIZE 30
#define k_DEFAULT_FPS 10
#define k_ELAPSED_CEILING 100

//----------------------------------------------------------------------------
//
// Project imports
//
//----------------------------------------------------------------------------
#include "gfx/spritesys/DirectorActionHandlers.h"
#include "gfx/spritesys/Sequence.h"
#include "gfx/spritesys/SpriteState.h"
#include "gfx/spritesys/directoractions.h"
#include "gs/gameobj/Unit.h"  // Unit, SPECATTACK
#include "sound/gamesounds.h"
#include "ui/aui_common/aui.h"
#include "ui/aui_common/aui_mouse.h"

class TradeRoute;
class UnitActor;
class ProjectileActor;
class EffectActor;
class TradeActor;
class aui_Surface;
class Battle;

//----------------------------------------------------------------------------
//
// Declarations
//
//----------------------------------------------------------------------------



class DQItem;
typedef std::shared_ptr<DQItem> DQItemPtr;
typedef std::weak_ptr<DQItem> DQItemWeakPtr;

class DQItem : std::enable_shared_from_this<DQItem> {
 private:
  DQItem(DQITEM_TYPE type, DQAction* action, DQHandler* handler);

 public:
  static DQItemPtr CreatePtr(DQITEM_TYPE type,
                             DQAction* action,
                             DQHandler* handler);
  ~DQItem();

  void SetOwner(sint32 owner) { m_owner = (sint8)owner; }
  sint32 GetOwner(void) const { return (sint32)m_owner; }

  SequenceWeakPtr getSequence();

 public:
  DQITEM_TYPE m_type;
  uint8 m_addedToSavedList;
  sint8 m_owner;
  uint16 m_round;
  DQAction* m_action;
  DQHandler* m_handler;

 private:
  SequencePtr m_sequence;
};

class Director {
 public:
  typedef std::vector<std::weak_ptr<UnitActor> > UnitActorVec;

  Director(void);
  ~Director(void);

  SequencePtr NewSequence(void);

  void UpdateTimingClock(void);
  void Process(void);
  void PauseDirector(BOOL pause);

#ifdef _DEBUG
  void DumpItem(DQItem* item);
  void DumpInfo(void);
#endif

  void HandleNextAction(void);

  void ActionFinished(SequenceWeakPtr seq);

  void HandleFinishedItem(DQItemPtr item);
  void SaveFinishedItem(DQItemPtr item);
  void GarbageCollectItems(void);

  void ProcessImmediately(DQItemPtr item);

  void CatchUp(void);
  bool CaughtUp(void);

  bool TileIsVisibleToPlayer(MapPoint& pos);

  bool IsProcessing();

  void AddMoveProcess(std::shared_ptr<UnitActor> top,
                      std::shared_ptr<UnitActor> dest,
                      sint32 arraySize,
                      UnitActor** moveActors,
                      BOOL isTransported);

  void AddMove(Unit mover,
               MapPoint const& oldPos,
               MapPoint const& newPos,
               const UnitActorVec& revealedActors,
               const UnitActorVec& restOfStack,
               bool isTransported,
               sint32 soundID);

  void AddTeleport(Unit top,
                   MapPoint const& oldPos,
                   MapPoint const& newPos,
                   const UnitActorVec& revealedActors,
                   const UnitActorVec& moveActors);

  void AddAttack(Unit attacker, Unit attacked);
  void AddAttackPos(Unit attacker, MapPoint const& pos);
  void AddSpecialAttack(Unit attacker, Unit attacked, SPECATTACK attack);
  void AddWinnerLoser(Unit victor, Unit dead);
  void AddDeath(Unit dead);
  void AddDeathWithSound(Unit dead, sint32 soundID);
  void AddProjectileAttack(Unit shooting,
                           Unit target,
                           SpriteStatePtr projectile_state,
                           SpriteStatePtr projectileEnd_state,
                           sint32 projectile_Path);
  void AddSpecialEffect(MapPoint& pos, sint32 spriteID, sint32 soundID);
  void AddMorphUnit(std::shared_ptr<UnitActor> morphingActor,
                    SpriteStatePtr ss,
                    sint32 type,
                    Unit id);
  void AddHide(Unit hider);
  void AddShow(Unit hider);
  void AddWork(Unit worker);
  void AddFastKill(Unit dead);
  void AddRemoveVision(const MapPoint& pos, double range);
  void AddAddVision(const MapPoint& pos, double range);
  void AddSetVisibility(std::shared_ptr<UnitActor> actor, uint32 visibility);
  void AddSetOwner(std::shared_ptr<UnitActor> actor, sint32 owner);
  void AddSetVisionRange(std::shared_ptr<UnitActor> actor, double range);
  void AddCombatFlash(MapPoint const& pos);
  void AddCopyVision(void);
  void AddCenterMap(const MapPoint& pos);
  void AddSelectUnit(uint32 flags);
  void AddEndTurn(void);
  void AddBattle(Battle* battle);
  void AddPlaySound(sint32 soundID, MapPoint const& pos);
  void AddGameSound(GAMESOUNDS sound);
  void AddPlayWonderMovie(sint32 which);
  void AddPlayVictoryMovie(GAME_OVER reason,
                           BOOL previouslyWon,
                           BOOL previouslyLost);
  void AddMessage(const Message& message);
  void AddFaceoff(Unit& attacker, Unit& defender);
  void AddTerminateFaceoff(Unit& faceroffer);
  void AddTerminateSound(Unit& unit);
  void AddInvokeThroneRoom(void);
  void AddInvokeResearchAdvance(MBCHAR* text);
  void AddBeginScheduler(sint32 player);

  void ActiveUnitAdd(std::shared_ptr<UnitActor> unitActor);
  void ActiveUnitRemove(std::shared_ptr<UnitActor> unitActor);

  void ActiveEffectAdd(EffectActor* effectActor);
  void ActiveEffectRemove(EffectActor* effectActor);
  void TradeActorCreate(TradeRoute newRoute);
  void TradeActorDestroy(TradeRoute routeToDestroy);

  uint32 KillAllActiveEffects(void);

  void DrawActiveUnits(RECT* paintRect, sint32 layer);

  void DrawActiveEffects(RECT* paintRect, sint32 layer);
  void DrawTradeRouteAnimations(RECT* paintRect, sint32 layer);

  uint32 ProcessActiveUnits(void);

  uint32 ProcessActiveEffects(void);
  void ProcessTradeRouteAnimations(void);

  void OffsetActiveUnits(sint32 deltaX, sint32 deltaY);

  void OffsetActiveEffects(sint32 deltaX, sint32 deltaY);
  void OffsetTradeRouteAnimations(sint32 deltaX, sint32 deltaY);

  std::shared_ptr<UnitActor> GetClickedActiveUnit(aui_MouseEvent* mouse);

  void NextPlayer(BOOL forcedUpdate = FALSE);

  uint32 GetMasterCurTime(void) { return m_masterCurTime; }
  void SetMasterCurTime(uint32 val) { m_masterCurTime = val; }
  sint32 GetAverageFPS(void) const { return m_averageFPS; }

  void Kill(std::shared_ptr<UnitActor> actor);
  void FastKill(std::shared_ptr<UnitActor> actor);
  void FastKill(EffectActor* actor);

  BOOL GetActionFinished(void) const { return m_actionFinished; }
  void SetActionFinished(BOOL finished = TRUE) { m_actionFinished = finished; }

  BOOL TileWillBeCompletelyVisible(sint32 x, sint32 y);

  void SetHoldSchedulerSequence(SequenceWeakPtr seq) {
    m_holdSchedulerSequence = seq;
  }

  void IncrementPendingGameActions();
  void DecrementPendingGameActions();

  void ReloadAllSprites();

  void NotifyResync();

  typedef std::deque<std::shared_ptr<UnitActor> > UnitActorList;
  UnitActorList m_activeUnitList;

  typedef std::deque<EffectActor*> EffectActorList;
  EffectActorList m_activeEffectList;

  typedef std::deque<TradeActor*> TradeActorList;
  TradeActorList m_tradeActorList;

  BOOL m_nextPlayer;

  uint32 m_masterCurTime;
  sint32 m_lastTickCount;
  sint32 m_timeLog[k_TIME_LOG_SIZE];
  sint32 m_timeLogIndex;
  sint32 m_averageElapsed;
  sint32 m_averageFPS;

  BOOL m_actionFinished;

  BOOL m_paused;
  BOOL m_processingActiveUnits;
  BOOL m_processingActiveEffects;

  sint32 m_curSequenceID;
  sint32 m_lastSequenceID;

  std::list<DQItemPtr> m_dispatchedItems;
  std::list<DQItemPtr> m_savedItems;
  std::list<DQItemPtr> m_itemQueue;

  SequenceWeakPtr m_holdSchedulerSequence;

  sint32 m_pendingGameActions;
  bool m_endTurnRequested;
};

extern Director* g_director;

#endif
