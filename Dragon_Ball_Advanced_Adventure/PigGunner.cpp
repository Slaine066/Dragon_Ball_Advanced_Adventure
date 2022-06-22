#include "stdafx.h"
#include "PigGunner.h"
#include "BmpManager.h"
#include "ScrollManager.h"
#include "ObjManager.h"
#include "TileManager.h"
#include "ObjManager.h"
#include "Enum.h"
#include "AbstractFactory.h"
#include "Bullet.h"
#include "Function.h"

PigGunner::PigGunner()
{
}

PigGunner::~PigGunner()
{
	Release();
}

void PigGunner::Initialize()
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

	// Stats
	m_tStats.iHealthMax = 50.f;
	m_tStats.iHealth = m_tStats.iHealthMax;
	m_tStats.iDamage = 7.f;
	m_tStats.iDamageOffset = 2.f;

	m_fSpeed = .5f;

	// AI
	m_iWalkRange = 200;
	m_iTargetRange = 350;
	m_iAttackRange = 300;

	m_eDir = DIR_RIGHT;
	m_pFrameKey = L"Pig_Gunner_RIGHT";

	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Enemy/Pig_Gunner_LEFT.bmp", L"Pig_Gunner_LEFT");
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Enemy/Pig_Gunner_RIGHT.bmp", L"Pig_Gunner_RIGHT");
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Enemy/Bullet.bmp", L"Bullet");

	// Start First Animation
	Change_Motion();
}

void PigGunner::Release()
{
	list<Obj*> Projectiles = ObjManager::Get_Instance()->Get_Projectiles();
	for (auto& pProj : Projectiles)
	{
		if (pProj->Get_Owner() == this)
			pProj->Set_Dead();
	}
}

int PigGunner::Update()
{
	if (Die())
		return OBJ_DEAD;

	Gravity();
	Can_Damage();
	Find_Target();
	AI_Behavior();
	
	Update_Rect();
	// Update_Collision_Rect(0, Get_ColSize()); // Don't Need.

	return OBJ_NOEVENT;
}

void PigGunner::Late_Update()
{
	Reset_Animation();

	Change_Motion();
	Change_Frame();
}

void PigGunner::Render(HDC hDC)
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
			hMemDC, m_tFrame.iFrameStart * m_tFrameInfo.fCX, m_tFrame.iMotion * m_tFrameInfo.fCY, m_tFrameInfo.fCX, m_tFrameInfo.fCY, RGB(89, 5, 167));
	}
	else
	{
		if (GetTickCount() > m_dwDeadTime + 100)
			m_dwDeadTime = GetTickCount();
		else
		{
			GdiTransparentBlt(
				hDC, m_tRect.left - fRectFrameDiffX + iScrollX, m_tRect.top - fRectFrameDiffY + iScrollY, m_tFrameInfoRender.fCX, m_tFrameInfoRender.fCY,
				hMemDC, m_tFrame.iFrameStart * m_tFrameInfo.fCX, m_tFrame.iMotion * m_tFrameInfo.fCY, m_tFrameInfo.fCX, m_tFrameInfo.fCY, RGB(89, 5, 167));
		}
	}
}

void PigGunner::Change_Motion()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
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
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iDamageNotifyStart = 4;
			m_tFrame.iDamageNotifyEnd = 4;
			m_tFrame.iMotion = 2;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case HIT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 3;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 4;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
		}

		m_ePreState = m_eCurState;
	}
}

void PigGunner::Change_Frame()
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

bool PigGunner::Die()
{
	if (m_eCurState == DEAD && m_tFrame.iFrameStart == m_tFrame.iFrameEnd && GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed + 500)
		return true;
	else if (m_bDead && m_eCurState == HIT && m_tFrame.iFrameStart == m_tFrame.iFrameEnd && GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed)
		m_eCurState = DEAD;
	else if (m_bDead)
		m_bIsHit = true;

	return false;
}

int PigGunner::Get_ColSize()
{
	return 0;
}

void PigGunner::Can_Damage()
{
}

void PigGunner::Reset_Animation()
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
	{
		m_bIsHit = false;
		m_eCurState = IDLE;
	}
}

void PigGunner::Sound_On_Animation()
{
}

void PigGunner::Find_Target()
{
	if (m_bDead)
		return;

	Obj* pPlayer = nullptr;
	if (!ObjManager::Get_Instance()->Get_Player().empty())
		pPlayer = ObjManager::Get_Instance()->Get_Player().front();

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
				m_pFrameKey = L"Pig_Gunner_RIGHT";
			}
			else
			{
				m_eDir = DIR_LEFT;
				m_pFrameKey = L"Pig_Gunner_LEFT";
			}
		}
		else
			m_pTarget = nullptr;
	}
	else
		m_pTarget = nullptr;
}

void PigGunner::AI_Behavior()
{
	// If Target:
		// If in Attack Range: Attack
		// If NOT in Attack Range: Move to Target
	// If !Target:
		// Patrol
	if (!m_bIsAttacking && !m_bIsHit && !m_bDead)
	{
		if (m_pTarget)
		{
			if (m_bIsInAttackRange)
			{
				if (!m_bIsAttacking && m_eCurState != ATTACK && GetTickCount() > m_dwAttackTime + 1500)
				{
					m_eCurState = ATTACK;
					m_bIsAttacking = true;
					m_bMotionAlreadyDamaged = false;

						
				}
				else if (!m_bIsAttacking)
					m_eCurState = IDLE;
			}
			else
				Move_ToTarget();
		}
		else
			Patrol();
	}

	// Spawn Bullet
	if (m_bIsAttacking && m_tFrame.iFrameStart >= m_tFrame.iDamageNotifyStart && m_tFrame.iFrameStart <= m_tFrame.iDamageNotifyEnd && GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed)
	{
		float fBulletOffset = m_eDir == DIR_RIGHT ? m_tInfo.fCX : -m_tInfo.fCX;
		ObjManager::Get_Instance()->Add_Object(OBJ_PROJECTILE, AbstractFactory<Bullet>::Create(m_tInfo.fX + fBulletOffset, m_tInfo.fY, m_eDir, m_eObjId, this));
	}

	if (m_bIsHit && m_eCurState != DEAD)
		m_eCurState = HIT;
}

void PigGunner::Move_ToTarget()
{
	if (!m_pTarget)
		return;

	m_eCurState = WALK;
	m_bIsAttacking = false;
	m_bMotionAlreadyDamaged = false;
	m_tInfo.fX += m_eDir == DIR_RIGHT ? m_fSpeed : -m_fSpeed;
}

void PigGunner::Patrol()
{
	// Switch between Idle and Walk based on time
	if (m_eCurState == IDLE)
	{
		if (GetTickCount() > m_dwIdleTime + 500)
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
			m_pFrameKey = m_pFrameKey == L"Pig_Gunner_RIGHT" ? L"Pig_Gunner_LEFT" : L"Pig_Gunner_RIGHT";
		}

		bool bIsRight = m_eDir == DIR_RIGHT ? true : false;
		m_tInfo.fX += m_eDir ? m_fSpeed : -m_fSpeed;
	}
}

void PigGunner::Gravity()
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
		m_pFrameKey = m_pFrameKey == L"Pig_Gunner_RIGHT" ? L"Pig_Gunner_LEFT" : L"Pig_Gunner_RIGHT";
	}
}