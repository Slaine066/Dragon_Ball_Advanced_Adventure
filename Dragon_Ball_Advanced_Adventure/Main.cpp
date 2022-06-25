#include "stdafx.h"
#include "Main.h"
#include "Define.h"
#include "SceneManager.h"
#include "KeyManager.h"
#include "BmpManager.h"
#include "ScrollManager.h"
#include "ObjManager.h"
#include "SoundManager.h"

float g_fSound = 1.f;

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

	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Back.bmp", L"Back");
	SoundManager::Get_Instance()->Initialize();
	SceneManager::Get_Instance()->Change_Scene(SCENE_LOGO);

	// To open the Stage Builder uncomment the line below and comment line 28
	//SceneManager::Get_Instance()->Change_Scene(SCENE_EDITOR);
}

void Main::Release()
{
	ReleaseDC(g_hWnd, m_hDC);

	SoundManager::Get_Instance()->Destroy_Instance();
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
	ScrollManager::Get_Instance()->Scroll_Lock(SceneManager::Get_Instance()->Get_Current_Scene());
}

void Main::Render()
{
	if (SceneManager::Get_Instance()->Get_Current_Scene() == SCENE_ENDING)
		return;

	Render_FPS();

	HDC	hBackDC = BmpManager::Get_Instance()->Find_Bmp(L"Back");
	
	SceneManager::Get_Instance()->Render(hBackDC);

	BitBlt(m_hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);
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