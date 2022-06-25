#include "stdafx.h"
#include "Ending.h"
#include "KeyManager.h"
#include "SceneManager.h"

Ending::Ending() : m_hVideo(nullptr)
{
}

Ending::~Ending()
{
	Release();
}

void Ending::Initialize()
{
	m_hVideo = MCIWndCreate(g_hWnd, nullptr, WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR, L"../Video/Ending.wmv");
	MoveWindow(m_hVideo, 0, 0, WINCX, WINCY, FALSE);
	MCIWndPlay(m_hVideo);
}

void Ending::Release()
{
	MCIWndClose(m_hVideo);
}

int Ending::Update()
{
	return 0;
}

void Ending::Late_Update()
{
	if ((MCIWndGetLength(m_hVideo) <= MCIWndGetPosition(m_hVideo)) || KeyManager::Get_Instance()->Key_Down(VK_RETURN))
	{
		SceneManager::Get_Instance()->Change_Scene(SCENE_LOGO);
		return;
	}
}

void Ending::Render(HDC hDC)
{
}