#pragma once

#include "stdafx.h"
#include "Obj.h"
#include "Player.h"

class TileManager
{
private:
	TileManager();
	~TileManager();

	static TileManager* m_pInstance;
	vector<Obj*> m_vecTile;
	bool m_bIsBossTile;

public:
	void Initialize();
	void Release();

	// Functions which run every frame
	void Update();
	void Late_Update();
	void Render(HDC hDC);

	void Set_BossStage() { m_bIsBossTile = true; }

	bool Tile_Collision(float fX, float fY, float fFrameOffset, float* pY);
	bool Wall_Collision(Obj* pObj);

	void Pick_Tile(POINT& _pt);
	void Reset_Tile(POINT& _pt);

	void Save_Tile();
	void Load_Tile();

	static TileManager* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new TileManager;

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
};