#include "stdafx.h"
#include "PigWarrior.h"
#include "BmpManager.h"
#include "ScrollManager.h"
#include "TileManager.h"

PigWarrior::PigWarrior() : m_ePreState(END), m_eCurState(IDLE), m_bIsAttacking(false)
{
}

PigWarrior::~PigWarrior()
{
}

void PigWarrior::Initialize()
{
	// Enemy Rect
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;

	// Sprite Frame Size
	m_tFrameInfo.fCX = 100.f;
	m_tFrameInfo.fCY = 60.f;

	m_tStats.iHealthMax = 30.f;
	m_tStats.iHealth = m_tStats.iHealthMax;
	m_tStats.iDamage = 5.f;

	m_fSpeed = 5.f;

	m_eDir = DIR_RIGHT;
	m_pFrameKey = L"Pig_Warrior_LEFT";

	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Enemy/Pig_Warrior_LEFT.bmp", L"Pig_Warrior_LEFT");
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Enemy/Pig_Warrior_RIGHT.bmp", L"Pig_Warrior_RIGHT");

	// Start First Animation
	Change_Motion();
}

void PigWarrior::Release()
{
}

int PigWarrior::Update()
{
	if (Die())
		return OBJ_DEAD;

	AI_Behavior();
	Gravity();

	Update_Rect();
	Update_Collision_Rect(10, Get_ColSize());

	return OBJ_NOEVENT;
}

void PigWarrior::Late_Update()
{
	Change_Motion();
	Change_Frame();
	Reset_Animation();
}

void PigWarrior::Render(HDC hDC)
{
	int iScrollX = (int)ScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)ScrollManager::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = BmpManager::Get_Instance()->Find_Bmp(m_pFrameKey);

	// Test Enemy Rectangle
	//Rectangle(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

	// Test Collision Rectangle
	//Rectangle(hDC, m_tCollisionRect.left + iScrollX, m_tCollisionRect.top + iScrollY, m_tCollisionRect.right + iScrollX, m_tCollisionRect.bottom + iScrollY);

	float fRectFrameDiffX = (m_tFrameInfo.fCX - m_tInfo.fCX) / 2;
	float fRectFrameDiffY = (m_tFrameInfo.fCY - m_tInfo.fCY) / 2;

	GdiTransparentBlt(
		hDC, m_tRect.left - fRectFrameDiffX + iScrollX, m_tRect.top - fRectFrameDiffY + iScrollY, m_tFrameInfo.fCX, m_tFrameInfo.fCY,
		hMemDC, m_tFrame.iFrameStart * m_tFrameInfo.fCX, m_tFrame.iMotion * m_tFrameInfo.fCY, m_tFrameInfo.fCX, m_tFrameInfo.fCY, RGB(132, 0, 132));
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

bool PigWarrior::Die()
{
	if (m_eCurState == DEAD && m_tFrame.iFrameStart == m_tFrame.iFrameEnd && GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed + 1000)
		return true;
	else if (m_bDead && m_eCurState == HIT && m_tFrame.iFrameStart == m_tFrame.iFrameEnd && GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed)
		m_eCurState = DEAD;
	else if (m_bDead)
		m_bIsHit = true;

	return false;
}

int PigWarrior::Get_ColSize()
{
	return 20;
}

void PigWarrior::Can_Damage()
{
}

void PigWarrior::Reset_Animation()
{
	// Reset HIT
	if (m_eCurState == HIT && m_tFrame.iFrameStart == m_tFrame.iFrameEnd && GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed)
		m_bIsHit = false;
}

void PigWarrior::AI_Behavior()
{
	// TODO: Check for Target
		// If Target:
			// If in Attack Range: Attack
			// If NOT in Attack Range: Follow Target
		// If !Target: Partol

	if (m_bIsHit && m_eCurState != DEAD)
		m_eCurState = HIT;
	else if (!m_bIsAttacking && !m_bIsHit && !m_bDead)
		m_eCurState = IDLE;
}

void PigWarrior::Gravity()
{
	bool bFloor = false;
	float fTargetY = 0.f;

	// If TRUE there is a Collision Tile below
	// If FALSE there is NO Collision Tile below
	bFloor = TileManager::Get_Instance()->Tile_Collision(m_tInfo.fX, m_tInfo.fY, (m_tFrameInfo.fCY / 2) - 6, &fTargetY); // 6: Distance from end of Sprite to end of the Frame (in Pixels)

	if (bFloor)
		m_tInfo.fY = fTargetY;
}