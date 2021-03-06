#ifdef HAVE_PRAGMA_ONCE
#pragma once
#endif
#ifndef _NET_UNIT_H_
#define _NET_UNIT_H_

class NetUnit;
class NetUnitMove;
class NetUnitHP;

#include "net/general/net_packet.h"
#include "gs/world/MapPoint.h"
#include "gs/gameobj/Unit.h"

class UnitData;

class NetUnit : public Packetizer {
public:
	NetUnit(UnitData*, Unit useActor = Unit());
	NetUnit() {}

	void Packetize(uint8* buf, uint16& size);
	void Unpacketize(uint16 id, uint8* buf,	uint16 size);
private:
	friend class NetCity;
	static void PacketizeUnit(uint8* buf, uint16& size, UnitData* unitData);
	static void UnpacketizeUnit(uint8* buf, uint16& size, UnitData* unitData);

	UnitData* m_unitData;
	Unit m_actorId;
};

class NetUnitMove : public Packetizer
{
public:
	NetUnitMove(const Unit id, const MapPoint &pnt);
	NetUnitMove() {};

	void Packetize(uint8 *buf, uint16 &size);
	void Unpacketize(uint16 id, uint8 *buf, uint16 size);
private:
	uint32 m_id;
	MapPoint m_point;
};

class NetUnitHP : public Packetizer
{
public:
	NetUnitHP(const Unit &id, double hp) {
		m_unit = id;
		m_hp = hp;
	}
	NetUnitHP() {};

	void Packetize(uint8 *buf, uint16 &size);
	void Unpacketize(uint16 id, uint8 *buf, uint16 size);
private:
	double m_hp;
	Unit m_unit;
};

#endif
