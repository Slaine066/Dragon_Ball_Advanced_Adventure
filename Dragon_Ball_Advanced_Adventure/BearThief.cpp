#include "stdafx.h"
#include "BearThief.h"
#include "BmpManager.h"
#include "ScrollManager.h"
#include "ObjManager.h"
#include "TileManager.h"
#include "AbstractFactory.h"
#include "BearThiefSpecialAttack.h"

BearThief::BearThief() : m_ePreState(END), m_eCurState(IDLE)
{
}

BearThief::~BearThief()
{
}

void BearThief::Initialize()
{
	// Enemy Rect Size
	m_tInfo.fCX = 140.f;
	m_tInfo.fCY = 170.f;

	// Sprite REAL Size
	m_tFrameInfo.fCX = 150.f;
	m_tFrameInfo.fCY = 100.f;

	// Sprite RENDER Size
	m_tFrameInfoRender.fCX = 300.f;
	m_tFrameInfoRender.fCY = 200.f;

	m_tStats.iHealthMax = 500.f;
	m_tStats.iHealth = m_tStats.iHealthMax;
	m_tStats.iDamage = 15.f;
	m_tStats.iDamageOffset = 3.f;

	m_fSpeed = 1.f;

	// AI
	m_iAttackRange = 120;

	m_iStaggerLimit = 25;

	m_eDir = DIR_LEFT;
	m_pFrameKey = L"Bear_Thief_LEFT";

	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Enemy/Bear_Thief_LEFT.bmp", L"Bear_Thief_LEFT");
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Enemy/Bear_Thief_RIGHT.bmp", L"Bear_Thief_RIGHT");

	// Projectile
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Enemy/Bear_Thief_Attack_Special_LEFT.bmp", L"Bear_Thief_Attack_Special_LEFT");
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Enemy/Bear_Thief_Attack_Special_RIGHT.bmp", L"Bear_Thief_Attack_Special_RIGHT");

	srand(time(NULL));

	// Start First Animation
	Change_Motion();
}

void BearThief::Release()
{
}

int BearThief::Update()
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

void BearThief::Late_Update()
{
	Reset_Animation();

	Change_Motion();
	Change_Frame();
}

void BearThief::Render(HDC hDC)
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

	GdiTransparentBlt(
		hDC, m_tRect.left - fRectFrameDiffX + iScrollX, m_tRect.top - fRectFrameDiffY + iScrollY, m_tFrameInfoRender.fCX, m_tFrameInfoRender.fCY,
		hMemDC, m_tFrame.iFrameStart * m_tFrameInfo.fCX, m_tFrame.iMotion * m_tFrameInfo.fCY, m_tFrameInfo.fCX, m_tFrameInfo.fCY, RGB(10, 87, 106));
}

void BearThief::Change_Motion()
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
			m_tFrame.iFrameEnd = 8 ;
			m_tFrame.iDamageNotifyStart = 4;
			m_tFrame.iDamageNotifyEnd = 5;
			m_tFrame.iMotion = 2;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ATTACK_SPECIAL:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iDamageNotifyStart = 7;
			m_tFrame.iDamageNotifyEnd = 7;
			m_tFrame.iMotion = 3;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case HIT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = 4;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 5;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
		}

		m_ePreState = m_eCurState;
	}
}

void BearThief::Change_Frame()
{
	// NO LOOP Animations:
	// ATTACK, ATTACK_SPECIAL, HIT, DEAD
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

void BearThief::Gravity()
{
	float fTargetY = 0.f;
	bool bFloor = false;

	bFloor = TileManager::Get_Instance()->Tile_Collision(m_tInfo.fX, m_tInfo.fY, (m_tFrameInfoRender.fCY / 2) - 22, &fTargetY);
	if (bFloor)
		m_tInfo.fY = fTargetY;

	bool bWall = false;
	bWall = TileManager::Get_Instance()->Wall_Collision(this);
	if (bWall)
	{
		m_eDir = m_eDir == DIR_RIGHT ? DIR_LEFT : DIR_RIGHT;
		m_pFrameKey = m_pFrameKey == L"Bear_Thief_RIGHT" ? L"Bear_Thief_LEFT" : L"Bear_Thief_RIGHT";
	}
}

bool BearThief::Die()
{
	if (m_bDead && m_eCurState == HIT && m_tFrame.iFrameStart == m_tFrame.iFrameEnd && GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed)
		m_eCurState = DEAD;
	else if (m_bDead)
		m_bIsHit = true;

	return false;
}

int BearThief::Get_ColSize()
{
	return 150;
}

void BearThief::Can_Damage()
{
	if (m_bIsAttacking && m_eCurState != ATTACK_SPECIAL)
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

void BearThief::Reset_Animation()
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

void BearThief::Sound_On_Animation()
{
}

void BearThief::Find_Target()
{
	if (m_bDead)
		return;

	Obj* pPlayer = nullptr;
	if (!ObjManager::Get_Instance()->Get_Player().empty())
		pPlayer = ObjManager::Get_Instance()->Get_Player().front();

	if (pPlayer)
	{
		m_pTarget = pPlayer;

		float fDistanceX = abs(pPlayer->Get_Info().fX - m_tInfo.fX);
		if (fDistanceX <= m_iAttackRange)
			m_bIsInAttackRange = true;
		else
			m_bIsInAttackRange = false;

		// Set Direction
		if (m_pTarget->Get_Info().fX > m_tInfo.fX)
		{
			m_eDir = DIR_RIGHT;
			m_pFrameKey = L"Bear_Thief_RIGHT";
		}
		else
		{
			m_eDir = DIR_LEFT;
			m_pFrameKey = L"Bear_Thief_LEFT";
		}
	}
	else
		m_pTarget = nullptr;
}

void BearThief::AI_Behavior()
{
	if (!m_bIsAttacking && !m_bIsHit && !m_bDead)
	{
		// If Target:
		if (m_pTarget)
		{
			// If in Attack Range: Attack
			if (m_bIsInAttackRange)
			{
				if (!m_bIsAttacking && m_eCurState != ATTACK && m_eCurState != ATTACK_SPECIAL && GetTickCount() > m_dwAttackTime + 1500)
				{
					m_eCurState = ATTACK;
					m_bIsAttacking = true;
					m_bMotionAlreadyDamaged = false;
					m_iPattern = 0;
				}
				else if (!m_bIsAttacking)
					m_eCurState = IDLE;
			}
			// If NOT in Attack Range: 1 = Follow Player, 2 = Special Attack
			else 
			{
				if (!m_iPattern)
					m_iPattern = rand() % 2 + 1;

				if (m_iPattern == 2)
				{
					if (!m_bIsAttacking && m_eCurState != ATTACK && m_eCurState != ATTACK_SPECIAL && GetTickCount() > m_dwAttackTime + 1500)
					{
						m_eCurState = ATTACK_SPECIAL;
						m_bIsAttacking = true;
						m_bMotionAlreadyDamaged = false;
						m_iPattern = 0;

					}
					else if (!m_bIsAttacking)
						m_eCurState = IDLE;
				}
				else
					Move_ToTarget();
			}
		}
	}

	// Spawn Projectile
	if (m_bIsAttacking && m_eCurState == ATTACK_SPECIAL && m_tFrame.iFrameStart >= m_tFrame.iDamageNotifyStart && m_tFrame.iFrameStart <= m_tFrame.iDamageNotifyEnd && GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed)
	{
		float fBulletOffset = m_eDir == DIR_RIGHT ? m_tInfo.fCX : -m_tInfo.fCX;
		ObjManager::Get_Instance()->Add_Object(OBJ_PROJECTILE, AbstractFactory<BearThiefSpecialAttack>::Create(m_tInfo.fX + fBulletOffset, m_tInfo.fY + 50, m_eDir, m_eObjId, this));
	}

	if (m_bIsHit && m_eCurState != DEAD)
		m_eCurState = HIT;
}

void BearThief::Move_ToTarget()
{
	if (!m_pTarget)
		return;

	m_eCurState = WALK;
	m_tInfo.fX += m_eDir == DIR_RIGHT ? m_fSpeed : -m_fSpeed;

	m_bIsAttacking = false;
	m_bMotionAlreadyDamaged = false;
}

void BearThief::Patrol()
{
}

bool BearThief::Can_Stagger()
{
	if (m_iStaggerDamage > m_iStaggerLimit)
	{
		m_iStaggerDamage = 0;
		return true;
	}
	
	return false;
}