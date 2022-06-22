#include "stdafx.h"
#include "BossHealthBar.h"
#include "BmpManager.h"
#include "ObjManager.h"
#include "Character.h"

BossHealthBar::BossHealthBar() : m_iCurrentHealth(0), m_iMaxHealth(0), m_fBarLength(0.f)
{
	ZeroMemory(&m_tBackgroundBar, sizeof(m_tBackgroundBar));
	ZeroMemory(&m_tBarInfoFirst, sizeof(m_tBarInfoFirst));
	ZeroMemory(&m_tBarInfoSecond, sizeof(m_tBarInfoSecond));
}

BossHealthBar::~BossHealthBar()
{
}

void BossHealthBar::Initialize()
{
	// Player Rect Size
	m_tInfo.fCX = 300.f;
	m_tInfo.fCY = 50.f;

	// Background Health Bar
	m_tBackgroundBar.left = 520.f;
	m_tBackgroundBar.top = 40.f;
	m_tBackgroundBar.right = 780.f;
	m_tBackgroundBar.bottom = 70.f;

	// First Health Bar
	m_tBarInfoFirst.left = 520.f;
	m_tBarInfoFirst.top = 40.f;
	m_tBarInfoFirst.right = 780.f;
	m_tBarInfoFirst.bottom = 70.f;

	// Second Health Bar
	m_tBarInfoSecond.left = 520.f;
	m_tBarInfoSecond.top = 40.f;
	m_tBarInfoSecond.right = 780.f;
	m_tBarInfoSecond.bottom = 70.f;

	m_fBarLength = m_tBarInfoFirst.right - m_tBarInfoSecond.left;

	m_eObjId = OBJ_ENEMY;

	m_iCurrentHealth = static_cast<Character*>(ObjManager::Get_Instance()->Get_Enemies().front())->Get_Stats().iHealth;

	m_pFrameKey = L"Health_Bar_Boss";
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Enemy/Health_Bar_Boss.bmp", m_pFrameKey);
}

void BossHealthBar::Release()
{
}

int BossHealthBar::Update()
{
	Update_Bars();
	Update_Rect();

	return 0;
}

void BossHealthBar::Late_Update()
{
}

void BossHealthBar::Render(HDC hDC)
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

void BossHealthBar::Update_Bars()
{
	Character* pBoss = static_cast<Character*>(ObjManager::Get_Instance()->Get_Enemies().front());
	m_iCurrentHealth = pBoss->Get_Stats().iHealth;
	m_iMaxHealth = pBoss->Get_Stats().iHealthMax;

	if (m_iCurrentHealth > 0 && m_iCurrentHealth <= m_iMaxHealth)
	{
		// Reduce Orange Bar
		if (m_iCurrentHealth > m_iMaxHealth / 2)
		{
			m_tBarInfoSecond.right = 780.f;
			m_tBarInfoSecond.right = 780.f - (m_fBarLength / ((m_iMaxHealth / 2) / (m_iMaxHealth - (float)m_iCurrentHealth)));
		}
		// Reduce Red Bar
		else
		{
			m_tBarInfoSecond.right = 520.f; // Empty the Orange Bar

			m_tBarInfoFirst.right = 780.f;
			m_tBarInfoFirst.right = 780.f - (m_fBarLength / ((m_iMaxHealth / 2) / (m_iMaxHealth / 2 - (float)m_iCurrentHealth)));
		}
	}
	else if (m_iCurrentHealth <= 0)
		m_tBarInfoFirst.right = 520.f; // Empty the Red Bar
}