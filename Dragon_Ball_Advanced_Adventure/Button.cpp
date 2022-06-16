#include "stdafx.h"
#include "Button.h"
#include "KeyManager.h"
#include "SceneManager.h"
#include "Enum.h"
#include "BmpManager.h"

Button::Button() : m_bShouldBlink(false), m_dwTime(GetTickCount()), m_bIsVisible(true)
{
}

Button::~Button()
{
	Release();
}

void Button::Initialize()
{
	m_tInfo.fCX = 200.f;
	m_tInfo.fCY = 36.f;
}

void Button::Release()
{
}

int Button::Update()
{
	Update_Rect();

	if (m_bShouldBlink)
	{
		if (GetTickCount() > m_dwTime + 1000)
		{
			m_bIsVisible = !m_bIsVisible;
			m_dwTime = GetTickCount();
		}
	}

	return OBJ_NOEVENT;
}

void Button::Late_Update()
{
	if (KeyManager::Get_Instance()->Key_Down('A'))
		SceneManager::Get_Instance()->Change_Scene(SCENE_MENU);
}

void Button::Render(HDC hDC)
{
	if (m_bIsVisible)
	{
		HDC	hMemDC = BmpManager::Get_Instance()->Find_Bmp(m_pFrameKey);
		GdiTransparentBlt(hDC, int(m_tRect.left), int(m_tRect.top), int(m_tInfo.fCX), int(m_tInfo.fCY), hMemDC, 0, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(132, 0, 132));
	}
}