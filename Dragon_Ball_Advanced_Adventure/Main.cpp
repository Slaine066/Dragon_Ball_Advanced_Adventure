#include "stdafx.h"
#include "Main.h"
#include "Define.h"
#include "SceneManager.h"
#include "KeyManager.h"
#include "BmpManager.h"
#include "ScrollManager.h"
#include "ObjManager.h"

Main::Main() : m_dwTime(GetTickCount()), m_iFPS(0)
{
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
}

Main::~Main()
{
	Release();
}

void Main::Initialize()
{
	m_hDC = GetDC(g_hWnd);

	// To open Map Builder uncomment line 25 and comment line 26
	//SceneManager::Get_Instance()->Change_Scene(SCENE_EDITOR);
	SceneManager::Get_Instance()->Change_Scene(SCENE_STAGE);
}

void Main::Release()
{
	ReleaseDC(g_hWnd, m_hDC);

	KeyManager::Get_Instance()->Destroy_Instance();
	BmpManager::Get_Instance()->Destroy_Instance();
	SceneManager::Get_Instance()->Destroy_Instance();
	ScrollManager::Get_Instance()->Destroy_Instance();
	ObjManager::Get_Instance()->Destroy_Instance();
}

void Main::Update()
{
	SceneManager::Get_Instance()->Update();
}

void Main::Late_Update()
{
	SceneManager::Get_Instance()->Late_Update();
	ScrollManager::Get_Instance()->Scroll_Lock();
}

void Main::Render()
{
	Render_FPS();
	
	SceneManager::Get_Instance()->Render(m_hDC);
}

void Main::Render_FPS()
{
	m_iFPS++;

	// Every second
	if (GetTickCount() > m_dwTime + 1000)
	{
		swprintf_s(m_szFPS, L"Dragon Ball: Advanced Adventure (FPS : %d)", m_iFPS);
		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount();
	}
}