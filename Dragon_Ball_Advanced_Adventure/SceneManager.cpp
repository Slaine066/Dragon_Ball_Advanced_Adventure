#include "stdafx.h"
#include "SceneManager.h"
#include "Logo.h"
#include "Menu.h"
#include "Stage.h"
#include "BossStage.h"
#include "Editor.h"

SceneManager* SceneManager::m_pInstance = nullptr;

SceneManager::SceneManager() : m_pScene(nullptr), m_ePreScene(SCENE_END), m_eCurScene(SCENE_LOGO)
{
}

SceneManager::~SceneManager()
{
	Release();
}

void SceneManager::Release()
{
	Safe_Delete(m_pScene);
}

void SceneManager::Change_Scene(SCENEID eScene)
{
	m_eCurScene = eScene;

	if (m_ePreScene != m_eCurScene)
	{
		Safe_Delete(m_pScene);

		switch (m_eCurScene)
		{
		case SCENE_LOGO:
			m_pScene = new Logo();
			break;
		case SCENE_MENU:
			m_pScene = new Menu();
			break;
		case SCENE_STAGE:
			m_pScene = new Stage();
			break;
		case SCENE_BOSS_STAGE:
			m_pScene = new BossStage();
		case SCENE_EDITOR:
			m_pScene = new Editor();
			break;
		}

		m_pScene->Initialize();
		m_ePreScene = m_eCurScene;
	}
}

void SceneManager::Update()
{
	m_pScene->Update();
}

void SceneManager::Late_Update()
{
	m_pScene->Late_Update();
}

void SceneManager::Render(HDC hDC)
{
	m_pScene->Render(hDC);
}