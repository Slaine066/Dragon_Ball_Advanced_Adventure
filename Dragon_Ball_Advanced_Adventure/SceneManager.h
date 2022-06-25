#pragma once

#include "Enum.h"
#include "Function.h"
#include "Scene.h"
#include "Obj.h"
#include "Player.h"

class SceneManager
{
private:
	SceneManager();
	~SceneManager();

	static SceneManager* m_pInstance;
	Scene* m_pScene;

	Player* m_pPlayer;

	SCENEID	m_ePreScene;
	SCENEID	m_eCurScene;

	void Reset_Scene();

public:
	void Change_Scene(SCENEID eScene);

	void Release();
	
	// Functions which run every frame
	void Update();
	void Late_Update();
	void Render(HDC hDC);

	void Set_Player(Player* pPlayer) { m_pPlayer = new Player(*pPlayer); }
	Player* Get_Player() { return m_pPlayer; }
	SCENEID Get_Current_Scene() { return m_eCurScene; }

	static SceneManager* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new SceneManager();

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