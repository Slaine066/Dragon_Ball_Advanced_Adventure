#include "stdafx.h"
#include "PigWarrior.h"
#include "BmpManager.h"
#include "ScrollManager.h"
#include "TileManager.h"
#include "ObjManager.h"

PigWarrior::PigWarrior()
{
}

PigWarrior::~PigWarrior()
{
}

void PigWarrior::Initialize()
{
	// Enemy Rect Size
	m_tInfo.fCX = 100.f;
	m_tInfo.fCY = 100.f;

	// Sprite REAL Size
	m_tFrameInfo.fCX = 100.f;
	m_tFrameInfo.fCY = 60.f;

	// Sprite RENDER Size
	m_tFrameInfoRender.fCX = 200.f;
	m_tFrameInfoRender.fCY = 120.f;

	m_tStats.iHealthMax = 70.f;
	m_tStats.iHealth = m_tStats.iHealthMax;
	m_tStats.iDamage = 5.f;

	m_fSpeed = .5f;

	// AI
	m_iWalkRange = 200;
	m_iTargetRange = 250;
	m_iAttackRange = 70;

	m_eDir = DIR_RIGHT;
	m_pFrameKey = L"Pig_Warrior_RIGHT";

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

	Gravity();
	Can_Damage();
	Find_Target();
	AI_Behavior();

	Update_Rect();
	Update_Collision_Rect(10, Get_ColSize());

	return OBJ_NOEVENT;
}

void PigWarrior::Late_Update()
{
	Reset_Animation();
	
	Change_Motion();
	Change_Frame();
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

	float fRectFrameDiffX = (m_tFrameInfoRender.fCX - m_tInfo.fCX) / 2;
	float fRectFrameDiffY = (m_tFrameInfoRender.fCY - m_tInfo.fCY) / 2;

	if (m_eCurState != DEAD)
	{
		GdiTransparentBlt(
			hDC, m_tRect.left - fRectFrameDiffX + iScrollX, m_tRect.top - fRectFrameDiffY + iScrollY, m_tFrameInfoRender.fCX, m_tFrameInfoRender.fCY,
			hMemDC, m_tFrame.iFrameStart * m_tFrameInfo.fCX, m_tFrame.iMotion * m_tFrameInfo.fCY, m_tFrameInfo.fCX, m_tFrameInfo.fCY, RGB(132, 0, 132));
	}
	else
	{
		if (GetTickCount() > m_dwDeadTime + 100)
			m_dwDeadTime = GetTickCount();
		else
		{
			GdiTransparentBlt(
				hDC, m_tRect.left - fRectFrameDiffX + iScrollX, m_tRect.top - fRectFrameDiffY + iScrollY, m_tFrameInfoRender.fCX, m_tFrameInfoRender.fCY,
				hMemDC, m_tFrame.iFrameStart * m_tFrameInfo.fCX, m_tFrame.iMotion * m_tFrameInfo.fCY, m_tFrameInfo.fCX, m_tFrameInfo.fCY, RGB(132, 0, 132));
		}
	}	
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
			m_tFrame.iDamageNotifyStart = 4;
			m_tFrame.iDamageNotifyEnd = 5;
			m_tFrame.iMotion = 2;
			m_tFrame.dwFrameSpeed = 100;
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
	if (m_eCurState == DEAD && m_tFrame.iFrameStart == m_tFrame.iFrameEnd && GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed + 500)
		return true;
	else if (m_bDead && m_eCurState == HIT && m_tFrame.iFrameStart == m_tFrame.iFrameEnd && GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed)
		m_eCurState = DEAD;
	else if (m_bDead)
		m_bIsHit = true;

	return false;
}

int PigWarrior::Get_ColSize()
{
	return 90;
}

void PigWarrior::Can_Damage()
{
	if (m_bIsAttacking)
	{
		if (m_tFrame.iFrameStart >= m_tFrame.iDamageNotifyStart && m_tFrame.iFrameStart <= m_tFrame.iDamageNotifyEnd)
			if (!m_bMotionAlreadyDamaged)
			{
				m_bCanDamage = true;
				return;
			}

		m_bCanDamage = false;
	}
}

void PigWarrior::Reset_Animation()
{
	// Reset ATTACK (When Target turns FALSE)
	if (m_bIsAttacking && m_tFrame.iFrameStart == m_tFrame.iFrameEnd && GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed && !m_pTarget)
	{
		m_bIsAttacking = false;
		m_dwAttackTime = GetTickCount();
		m_eCurState = IDLE;
	}
	// Reset ATTACK (When Attack Animation ends)
	else if (m_bIsAttacking && m_tFrame.iFrameStart == m_tFrame.iFrameEnd && GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed)
	{
		m_bIsAttacking = false;
		m_dwAttackTime = GetTickCount();
		
	}
	
	// Reset HIT
	if (m_eCurState == HIT && m_tFrame.iFrameStart == m_tFrame.iFrameEnd && GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed)
		m_bIsHit = false;
}

void PigWarrior::Find_Target()
{
	if (m_bDead)
		return;

	Obj* pPlayer = ObjManager::Get_Instance()->Get_Player().front();
	
	if (pPlayer)
	{
		float fDistanceX = abs(pPlayer->Get_Info().fX - m_tInfo.fX);
		float fDistanceY = abs(pPlayer->Get_Info().fY - m_tInfo.fY);

		if (fDistanceX <= m_iTargetRange && fDistanceY < (TILECX >> 1))
		{
			m_pTarget = pPlayer; // Set pTarget

			// Set bIsInAttackRange
			if (fDistanceX <= m_iAttackRange)
				m_bIsInAttackRange = true;
			else
			{
				m_bIsInAttackRange = false;
				m_dwAttackTime = 0;
			}

			// Set Direction
			if (m_pTarget->Get_Info().fX > m_tInfo.fX)
			{
				m_eDir = DIR_RIGHT;
				m_pFrameKey = L"Pig_Warrior_RIGHT";
			}
			else
			{
				m_eDir = DIR_LEFT;
				m_pFrameKey = L"Pig_Warrior_LEFT";
			}
		}
		else
			m_pTarget = nullptr;
	}
	else
		m_pTarget = nullptr;
}

void PigWarrior::AI_Behavior()
{
	if (!m_bIsAttacking && !m_bIsHit && !m_bDead)
	{
		// If Target:
		if (m_pTarget)
		{
			// If in Attack Range: Attack
			if (m_bIsInAttackRange)
			{
				if (!m_bIsAttacking && m_eCurState != ATTACK && GetTickCount() > m_dwAttackTime + 2000)
				{
					m_eCurState = ATTACK;
					m_bIsAttacking = true;
					m_bMotionAlreadyDamaged = false;
				}
				else if (!m_bIsAttacking)
					m_eCurState = IDLE;
			}
			// If NOT in Attack Range: Move to Target
			else
				Move_ToTarget();
		}
		// If !Target: Patrol
		else
			Patrol();
	}	

	if (m_bIsHit && m_eCurState != DEAD)
		m_eCurState = HIT;
}

void PigWarrior::Move_ToTarget()
{
	if (!m_pTarget)
		return;

	m_eCurState = WALK;
	m_tInfo.fX += m_eDir == DIR_RIGHT ? m_fSpeed : -m_fSpeed;

	m_bIsAttacking = false;
	m_bMotionAlreadyDamaged = false;
}

void PigWarrior::Patrol()
{
	// Switch between Idle and Walk based on time
	if (m_eCurState == IDLE)
	{
		if (GetTickCount() > m_dwIdleTime + 2000)
		{
			m_eCurState = WALK;
			m_dwWalkTime = GetTickCount();
		}
	}
	else if (m_eCurState == WALK)
	{
		if (GetTickCount() > m_dwWalkTime + 4000)
		{
			m_eCurState = IDLE;
			m_dwIdleTime = GetTickCount();
			m_bShouldSwitchDir = true;
		}
		else
			m_bShouldSwitchDir = false;
	}

	// Movement
	if (m_eCurState == WALK)
	{
		// Switch Direction
		if (m_bShouldSwitchDir)
		{
			m_eDir = m_eDir == DIR_RIGHT ? DIR_LEFT : DIR_RIGHT;
			m_pFrameKey = m_pFrameKey == L"Pig_Warrior_RIGHT" ? L"Pig_Warrior_LEFT" : L"Pig_Warrior_RIGHT";
		}
			
		bool bIsRight = m_eDir == DIR_RIGHT ? true : false;
		m_tInfo.fX += m_eDir ? m_fSpeed : -m_fSpeed;
	}
}

void PigWarrior::Gravity()
{
	float fTargetY = 0.f;
	bool bFloor = false;
	
	// If TRUE there is a Collision Tile below
	// If FALSE there is NO Collision Tile below
	bFloor = TileManager::Get_Instance()->Tile_Collision(m_tInfo.fX, m_tInfo.fY, (m_tFrameInfoRender.fCY / 2) - 12, &fTargetY); // 6: Distance from end of Sprite to end of the Frame (in Pixels)
	if (bFloor)
		m_tInfo.fY = fTargetY;
	
	bool bWall = false;
	bWall = TileManager::Get_Instance()->Wall_Collision(this);
	if (bWall)
	{
		m_eDir = m_eDir == DIR_RIGHT ? DIR_LEFT : DIR_RIGHT;
		m_pFrameKey = m_pFrameKey == L"Pig_Warrior_RIGHT" ? L"Pig_Warrior_LEFT" : L"Pig_Warrior_RIGHT";
	}
}