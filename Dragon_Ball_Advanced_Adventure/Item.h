#pragma once

#include "Obj.h"
#include "SoundManager.h"
#include "TileManager.h"

class Character;

class Item abstract : public Obj
{
public:
	Item() : m_fSpawnRate(0) {};
	virtual ~Item() {};

	int Get_SpawnRate() { return m_fSpawnRate; }

	virtual void Item_Effect(Character* pPlayer) PURE;
	void Item_Sound() { SoundManager::Get_Instance()->PlaySound(L"Item_Pickup.wav", CHANNEL_EFFECT, .5f); };

protected:
	virtual void Gravity() 
	{
		float fY;
		bool bFloor = TileManager::Get_Instance()->Tile_Collision(m_tInfo.fX, m_tInfo.fY, (m_tFrameInfoRender.fCY / 2) - 6, &fY);
		if (bFloor)
			m_tInfo.fY = fY;
	}

	// From 1 to 10
	// 1	= 10% Drop Rate
	// 10	= 100% Drop Rate
	int m_fSpawnRate;
};