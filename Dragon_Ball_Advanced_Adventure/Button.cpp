#include "stdafx.h"
#include "Button.h"
#include "KeyManager.h"
#include "SceneManager.h"
#include "Enum.h"
#include "BmpManager.h"
#include "SoundManager.h"

extern float g_fSound;

Button::Button() : 
	m_bShouldBlink(false), m_bShouldChange(false), 
	m_dwTime(GetTickCount()), m_dwSoundTime(GetTickCount()), m_bIsVisible(true)
{
}

Button::~Button()
{
	Release();
}

void Button::Initialize()
{
	m_tInfo.fCX = 267.f;
	m_tInfo.fCY = 50.f;

	// Sprite REAL Size
	m_tFrameInfo.fCX = 267.f;
	m_tFrameInfo.fCY = 50.f;

	// Sprite RENDER Size
	m_tFrameInfoRender.fCX = m_tFrameInfo.fCX * .7;
	m_tFrameInfoRender.fCY = m_tFrameInfo.fCY * .7;
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
	{
		m_dwSoundTime = GetTickCount();
		m_bShouldChange = true;
		SoundManager::Get_Instance()->PlaySound(L"Start.wav", CHANNEL_SYSTEM, .5f);
	}

	if (m_bShouldChange && GetTickCount() > m_dwSoundTime + 1000)
		SceneManager::Get_Instance()->Change_Scene(SCENE_STAGE1_1);
}

void Button::Render(HDC hDC)
{
	if (m_bIsVisible)
	{
		float fRectFrameDiffX = (m_tFrameInfoRender.fCX - m_tInfo.fCX) / 2;
		float fRectFrameDiffY = (m_tFrameInfoRender.fCY - m_tInfo.fCY) / 2;

		HDC	hMemDC = BmpManager::Get_Instance()->Find_Bmp(m_pFrameKey);
		//GdiTransparentBlt(hDC, int(m_tRect.left), int(m_tRect.top), int(m_tInfo.fCX), int(m_tInfo.fCY), hMemDC, 0, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(132, 0, 132));

		GdiTransparentBlt(
			hDC, m_tRect.left - fRectFrameDiffX, m_tRect.top - fRectFrameDiffY, m_tFrameInfoRender.fCX, m_tFrameInfoRender.fCY,
			hMemDC, m_tFrame.iFrameStart * m_tFrameInfo.fCX, m_tFrame.iMotion * m_tFrameInfo.fCY, m_tFrameInfo.fCX, m_tFrameInfo.fCY, RGB(132, 0, 132));
	}
}