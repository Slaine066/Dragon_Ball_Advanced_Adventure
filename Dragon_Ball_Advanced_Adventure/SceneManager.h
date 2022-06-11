#pragma once

#include "Enum.h"
#include "Function.h"
#include "Scene.h"

class SceneManager
{
private:
	SceneManager();
	~SceneManager();

	static SceneManager* m_pInstance;
	Scene* m_pScene;

	SCENEID	m_ePreScene;
	SCENEID	m_eCurScene;

public:
	void Change_Scene(SCENEID eScene);

	void Release();
	
	// Functions which run every frame
	void Update();
	void Late_Update();
	void Render(HDC hDC);

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

