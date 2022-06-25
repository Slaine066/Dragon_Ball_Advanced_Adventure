#include "stdafx.h"
#include "SceneManager.h"
#include "Logo.h"
#include "Menu.h"
#include "Stage1_1.h"
#include "Stage1_2.h"
#include "BossStage.h"
#include "Editor.h"
#include "ObjManager.h"
#include "ScrollManager.h"

SceneManager* SceneManager::m_pInstance = nullptr;

SceneManager::SceneManager() : m_pScene(nullptr), m_pPlayer(nullptr), m_ePreScene(SCENE_END), m_eCurScene(SCENE_LOGO)
{
}

SceneManager::~SceneManager()
{
	Release();
}

void SceneManager::Release()
{
	Safe_Delete(m_pScene);
	Safe_Delete(m_pPlayer);
}

void SceneManager::Reset_Scene()
{
	Safe_Delete(m_pScene);

	m_pScene = new Stage1_1();
	m_pScene->Initialize();
	m_eCurScene = SCENE_STAGE1_1;
	m_ePreScene = SCENE_STAGE1_1;
	ScrollManager::Get_Instance()->Reset_Scroll();
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
		case SCENE_STAGE1_1:
			m_pScene = new Stage1_1();
			break;
		case SCENE_STAGE1_2:
			m_pScene = new Stage1_2();
			break;
		case SCENE_BOSS_STAGE:
			m_pScene = new BossStage();
			break;
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
	if ((m_eCurScene == SCENE_STAGE1_1 || m_eCurScene == SCENE_STAGE1_2 || m_eCurScene == SCENE_BOSS_STAGE) && ObjManager::Get_Instance()->Get_Player().empty())
		Reset_Scene();

	m_pScene->Late_Update();
}

void SceneManager::Render(HDC hDC)
{
	m_pScene->Render(hDC);
}