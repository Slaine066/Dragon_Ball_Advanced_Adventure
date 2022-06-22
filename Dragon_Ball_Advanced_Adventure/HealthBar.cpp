#include "stdafx.h"
#include "HealthBar.h"
#include "BmpManager.h"
#include "ScrollManager.h"
#include "ObjManager.h"
#include "Character.h"

HealthBar::HealthBar() : m_iCurrentHealth(100), m_fBarLength(0.f)
{
	ZeroMemory(&m_tBackgroundBar, sizeof(m_tBackgroundBar));
	ZeroMemory(&m_tBarInfoFirst, sizeof(m_tBarInfoFirst));
	ZeroMemory(&m_tBarInfoSecond, sizeof(m_tBarInfoSecond));
}

HealthBar::~HealthBar()
{
}

void HealthBar::Initialize()
{
	// Player Rect Size
	m_tInfo.fCX = 300.f;
	m_tInfo.fCY = 86.f;

	// Background Health Bar
	m_tBackgroundBar.left = 80.f;
	m_tBackgroundBar.top = 45.f;
	m_tBackgroundBar.right = 260.f;
	m_tBackgroundBar.bottom = 66.f;

	// First Health Bar
	m_tBarInfoFirst.left = 80.f;
	m_tBarInfoFirst.top = 45.f;
	m_tBarInfoFirst.right = 260.f;
	m_tBarInfoFirst.bottom = 66.f;

	// Second Health Bar
	m_tBarInfoSecond.left = 80.f;
	m_tBarInfoSecond.top = 45.f;
	m_tBarInfoSecond.right = 260.f;
	m_tBarInfoSecond.bottom = 66.f;

	m_fBarLength = m_tBarInfoFirst.right - m_tBarInfoSecond.left; // 200

	m_eObjId = OBJ_PLAYER;

	if (!ObjManager::Get_Instance()->Get_Player().empty())
		m_iCurrentHealth = static_cast<Character*>(ObjManager::Get_Instance()->Get_Player().front())->Get_Stats().iHealth;

	m_pFrameKey = L"Health_Bar";
	
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Player/Health_Bar.bmp", m_pFrameKey);
}

void HealthBar::Release()
{
}

int HealthBar::Update()
{
	Update_Bars();
	Update_Rect();

	return 0;
}

void HealthBar::Late_Update()
{
}

void HealthBar::Render(HDC hDC)
{
	HDC	hMemDC = BmpManager::Get_Instance()->Find_Bmp(m_pFrameKey);

	// Test Rectangle
	//Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

	{
		// Background Bar
		HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(48, 104, 164));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, myBrush);
		Rectangle(hDC, m_tBackgroundBar.left, m_tBackgroundBar.top, m_tBackgroundBar.right, m_tBackgroundBar.bottom);
		SelectObject(hDC, oldBrush);
		DeleteObject(myBrush);
	}

	{
		// First Bar
		HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(245, 32, 59));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, myBrush);
		Rectangle(hDC, m_tBarInfoFirst.left, m_tBarInfoFirst.top, m_tBarInfoFirst.right, m_tBarInfoFirst.bottom);
		SelectObject(hDC, oldBrush);
		DeleteObject(myBrush);
	}

	{
		// Second Bar
		HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(247, 142, 1));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, myBrush);
		Rectangle(hDC, m_tBarInfoSecond.left, m_tBarInfoSecond.top, m_tBarInfoSecond.right, m_tBarInfoSecond.bottom);
		SelectObject(hDC, oldBrush);
		DeleteObject(myBrush);
	}

	GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, hMemDC, 0, 0, m_tInfo.fCX, m_tInfo.fCY, RGB(103, 184, 208));
}

void HealthBar::Update_Bars()
{
	if (!ObjManager::Get_Instance()->Get_Player().empty())
		m_iCurrentHealth = static_cast<Character*>(ObjManager::Get_Instance()->Get_Player().front())->Get_Stats().iHealth;

	if (m_iCurrentHealth > 0 && m_iCurrentHealth < 100)
	{
		// Reduce Orange Bar
		if (m_iCurrentHealth > 50)
		{
			m_tBarInfoSecond.right = 260.f;
			m_tBarInfoSecond.right = 260.f - (m_fBarLength / (50.f / (100.f - (float)m_iCurrentHealth)));
		}
		// Reduce Red Bar
		else
		{
			m_tBarInfoSecond.right = 80.f; // Empty the Orange Bar

			m_tBarInfoFirst.right = 260.f;
			m_tBarInfoFirst.right = 260.f - (m_fBarLength / (50.f / (50.f - (float)m_iCurrentHealth)));
		}
	}
	else if (m_iCurrentHealth <= 0)
		m_tBarInfoFirst.right = 80.f; // Empty the Red Bar
}