#include "stdafx.h"
#include "DamageNumbers.h"
#include "ScrollManager.h"

DamageNumbers::DamageNumbers() : m_iNumber(0), m_dwTime(GetTickCount())
{
}

DamageNumbers::~DamageNumbers()
{
}

void DamageNumbers::Initialize()
{
	m_fSpeed = 1.f;
}

void DamageNumbers::Release()
{
}

int DamageNumbers::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fY -= m_fSpeed;

	return OBJ_NOEVENT;
}

void DamageNumbers::Late_Update()
{
	if (GetTickCount() > m_dwTime + 1000)
		m_bDead = true;
}

void DamageNumbers::Render(HDC hDC)
{
	int iScrollX = (int)ScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)ScrollManager::Get_Instance()->Get_ScrollY();

	if (m_iNumber != 0)
	{
		TCHAR sztDamage[32] = L"";
		swprintf_s(sztDamage, L"%d", m_iNumber);
		TextOut(hDC, m_tInfo.fX + iScrollX, m_tInfo.fY + iScrollY, sztDamage, lstrlen(sztDamage));
	}
}