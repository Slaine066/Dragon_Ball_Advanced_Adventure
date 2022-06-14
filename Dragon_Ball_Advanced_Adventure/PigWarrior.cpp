#include "stdafx.h"
#include "PigWarrior.h"
#include "BmpManager.h"
#include "ScrollManager.h"

PigWarrior::PigWarrior() : m_ePreState(END), m_eCurState(IDLE), m_bIsAttacking(false)
{
}

PigWarrior::~PigWarrior()
{
}

void PigWarrior::Initialize()
{
	/*m_tInfo.fCX = 100;
	m_tInfo.fCY = 60;*/

	m_fSpeed = 5.f;

	m_pFrameKey = L"Pig_Warrior_RIGHT";

	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Enemy/Pig_Warrior_LEFT.bmp", L"Pig_Warrior_LEFT");
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Enemy/Pig_Warrior_RIGHT.bmp", L"Pig_Warrior_RIGHT");

	Change_Motion();
}

void PigWarrior::Release()
{
}

int PigWarrior::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Gravity();

	// TODO: Check for Target
	// If Target:
		// If in Attack Range: Attack
		// If NOT in Attack Range: Follow Target
	// If !Target: Partol

	Update_Rect();

	return OBJ_NOEVENT;
}

void PigWarrior::Late_Update()
{
	Change_Motion();
	Change_Frame();
}

void PigWarrior::Render(HDC hDC)
{
	int iScrollX = (int)ScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)ScrollManager::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = BmpManager::Get_Instance()->Find_Bmp(m_pFrameKey);

	// Test Rectangle
	Rectangle(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

	GdiTransparentBlt(
		hDC, int(m_tRect.left + iScrollX), int(m_tRect.top + iScrollY), 100, 60,
		hMemDC, m_tFrame.iFrameStart * 100, m_tFrame.iMotion * 60, 100, 60, RGB(132, 0, 132));
}

void PigWarrior::Change_Motion()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 1;
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iMotion = 2;
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case HIT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iMotion = 3;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = 4;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
		}

		m_ePreState = m_eCurState;
	}
}

void PigWarrior::Change_Frame()
{
	// NO LOOP Animations:
	// ATTACK, HIT, DEAD
	if (m_eCurState == ATTACK || m_eCurState == HIT || m_eCurState == DEAD)
	{
		if (GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed)
		{
			if (m_tFrame.iFrameStart < m_tFrame.iFrameEnd)
			{
				m_tFrame.iFrameStart++;
				m_tFrame.dwFrameTime = GetTickCount();
			}
		}
	}

	// LOOP Animations:
	// IDLE, WALK
	else
	{
		if (GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed)
		{
			m_tFrame.iFrameStart++;

			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_tFrame.iFrameStart = 0;

			m_tFrame.dwFrameTime = GetTickCount();
		}
	}
}

void PigWarrior::Gravity()
{
}