#include "stdafx.h"
#include "Kamehameha.h"
#include "ScrollManager.h"
#include "BmpManager.h"
#include "ObjManager.h"
#include "Character.h"

Kamehameha::Kamehameha() : m_bFirstTick(true), m_dwDamageTime(GetTickCount()), m_dwDuration(GetTickCount()), m_bIsLimit(false), m_bShouldDestroy(false)
{
}

Kamehameha::~Kamehameha()
{
}

void Kamehameha::Initialize()
{
	// Player Rect Size
	m_tInfo.fCX = 100.f;
	m_tInfo.fCY = 50.f;

	// Sprite REAL Size
	m_tFrameInfo.fCX = m_tInfo.fCX;
	m_tFrameInfo.fCY = m_tInfo.fCY;

	// Sprite RENDER Size
	m_tFrameInfoRender.fCX = m_tInfo.fCX * 2;
	m_tFrameInfoRender.fCY = m_tInfo.fCY * 2;

	m_tInfo.fX += m_eDir == DIR_RIGHT ? m_tInfo.fCX / 2 : -m_tInfo.fCX / 2;
	
	// Frame
	m_pFrameKey = L"Kamehameha";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iMotion = 0;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();
}

void Kamehameha::Release()
{
}

int Kamehameha::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Change_Motion(); 
	Check_DamageTime();
	Update_Rect();

	return OBJ_NOEVENT;
}

void Kamehameha::Late_Update()
{
}

void Kamehameha::Render(HDC hDC)
{
	int iScrollX = (int)ScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)ScrollManager::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = BmpManager::Get_Instance()->Find_Bmp(m_pFrameKey);

	// Test Rectangle
	//Rectangle(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

	GdiTransparentBlt(
		hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tFrameInfo.fCX, m_tFrameInfo.fCY,
		hMemDC, 0, m_tFrame.iMotion * m_tFrameInfo.fCY, m_tFrameInfo.fCX, m_tFrameInfo.fCY, RGB(132, 0, 132));
}

void Kamehameha::Change_Motion()
{
	if (GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed)
	{
		// Over Duration
		if (GetTickCount() > m_dwDuration + 2000)
		{
			if (m_bShouldDestroy)
			{
				m_bDead = true;

				Character* pCharacter = dynamic_cast<Character*>(ObjManager::Get_Instance()->Get_Player().front());
				if (pCharacter)
					pCharacter->Set_IsAttacking(false);
			}
				
			else
			{
				m_tFrame.iMotion = 7;
				m_tFrame.dwFrameTime = GetTickCount();
				m_bShouldDestroy = true;
			}
		}
		// Kamehameha Animation
		else
		{
			if (m_tFrame.iMotion < 6)
			{
				m_tFrame.iMotion += 1;

				if (m_tFrame.iMotion != 6)
				{
					m_tInfo.fX += m_eDir == DIR_RIGHT ? 50.f : -50.f; // 50: Half width of the first Frame
					m_tInfo.fCX += 100.f;
					m_tFrameInfo.fCX = m_tInfo.fCX;
					m_tFrameInfoRender.fCX = m_tInfo.fCX * 2;
				}
			}
			else
				m_tFrame.iMotion -= 1;

			m_tFrame.dwFrameTime = GetTickCount();
		}
	}

	m_bCanDamage = false;
}

bool Kamehameha::Get_CanDamage()
{
	return m_bCanDamage;
}

void Kamehameha::Check_DamageTime()
{
	if (GetTickCount() > m_dwDamageTime + 200)
	{
		m_bCanDamage = true;
		m_dwDamageTime = GetTickCount();
	}
}