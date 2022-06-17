#include "stdafx.h"
#include "Player.h"
#include "BmpManager.h"
#include "ScrollManager.h"
#include "BmpManager.h"
#include "KeyManager.h"
#include "TileManager.h"

Player::Player() 
	: m_ePreState(END), m_eCurState(IDLE), m_fSprintSpeed(0.f), m_fFallSpeed(6.f), 
	m_bIsJumping(false), m_fJumpPower(0.f), m_fJumpTime(0.f), m_fAccel(9.8f),
	m_bIsComboActive(false), m_dwChargingTime(0)
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	// Player Rect Size
	m_tInfo.fCX = 80.f;
	m_tInfo.fCY = 80.f;

	// Sprite REAL Size
	m_tFrameInfo.fCX = 100.f;
	m_tFrameInfo.fCY = 50.f;

	// Sprite RENDER Size
	m_tFrameInfoRender.fCX = 200.f;
	m_tFrameInfoRender.fCY = 100.f;

	// Stats
	m_tStats.iHealthMax = 100.f;
	m_tStats.iHealth = m_tStats.iHealthMax;
	m_tStats.iEnergyMax = 100.f;
	m_tStats.iEnergy = m_tStats.iEnergyMax;
	m_tStats.iDamage = 10.f;

	// Speeds
	m_fSpeed = 3.f;
	m_fSprintSpeed = 5.f;
	m_fJumpPower = 14.f;

	m_eDir = DIR_RIGHT;
	m_pFrameKey = L"Player_RIGHT";

	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Player/Goku_LEFT.bmp", L"Player_LEFT");
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Player/Goku_RIGHT.bmp", L"Player_RIGHT");
	// TODO: Load "Kamehameha.bmp"
	
	// Start First Animation
	Change_Motion(); 
}

void Player::Release()
{
}

int Player::Update()
{
	if (Die())
		return OBJ_DEAD;
	
	Can_Damage();
	Key_Input();
	Offset();
	Gravity();

	Update_Rect();
	Update_Collision_Rect(10, Get_ColSize());

	return OBJ_NOEVENT;
}

void Player::Late_Update()
{
	Reset_Animation();

	Change_Motion();
	Change_Frame();
	
}

void Player::Render(HDC hDC)
{
	int iScrollX = (int)ScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)ScrollManager::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = BmpManager::Get_Instance()->Find_Bmp(m_pFrameKey);

	// Test Player Rectangle
	//Rectangle(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

	// Test Collision Rectangle
	//Rectangle(hDC, m_tCollisionRect.left + iScrollX, m_tCollisionRect.top + iScrollY, m_tCollisionRect.right + iScrollX, m_tCollisionRect.bottom + iScrollY);

	float fRectFrameDiffX = (m_tFrameInfoRender.fCX - m_tInfo.fCX) / 2;
	float fRectFrameDiffY = (m_tFrameInfoRender.fCY - m_tInfo.fCY) / 2;

	GdiTransparentBlt(
		hDC, m_tRect.left - fRectFrameDiffX + iScrollX, m_tRect.top - fRectFrameDiffY + iScrollY, m_tFrameInfoRender.fCX, m_tFrameInfoRender.fCY,
		hMemDC, m_tFrame.iFrameStart * m_tFrameInfo.fCX, m_tFrame.iMotion * m_tFrameInfo.fCY, m_tFrameInfo.fCX, m_tFrameInfo.fCY, RGB(132, 0, 132));
}

void Player::Key_Input()
{
	// Left Arrow - Move Left
	if (KeyManager::Get_Instance()->Key_Pressing(VK_LEFT) && !m_bIsAttacking && !m_bIsHit && m_eCurState != CHARGING && m_eCurState != ATTACK_SPECIAL)
		Move(false);

	// Right Arrow - Move Right
	else if (KeyManager::Get_Instance()->Key_Pressing(VK_RIGHT) && !m_bIsAttacking && !m_bIsHit  && m_eCurState != CHARGING && m_eCurState != ATTACK_SPECIAL)
		Move(true);

	else if (!m_bIsJumping && !m_bIsAttacking && !m_bIsHit && m_eCurState != CHARGING && m_eCurState != ATTACK_SPECIAL)
		m_eCurState = IDLE;

	// Space Bar - Jump
	if (KeyManager::Get_Instance()->Key_Down(VK_SPACE) && !m_bIsAttacking && !m_bIsHit)
	{
		m_bIsJumping = true;
		m_eCurState = JUMP;
	}

	// Keyboard 'A' - Attack
	if (KeyManager::Get_Instance()->Key_Down('A') && !m_bIsHit)
		Attack();

	// Keyboard 'S' Pressing - Charging
	if (KeyManager::Get_Instance()->Key_Pressing('S') && !m_bIsJumping && !m_bIsAttacking)
	{
		if (m_eCurState != CHARGING)
		{
			m_eCurState = CHARGING;
			m_dwChargingTime = GetTickCount();
		}
	}
		
	// Keyboard 'S' Up - Special Attack
	if (KeyManager::Get_Instance()->Key_Up('S') && !m_bIsJumping && !m_bIsAttacking)
	{
		m_eCurState = ATTACK_SPECIAL;
		Attack_Special();
	}

	// NO INPUT
	if (m_bIsHit && m_eCurState != DEAD)
		m_eCurState = HIT;
}

void Player::Offset()
{
	int iOffsetMinX = 100.f;
	int iOffsetMaxX = 700.f;

	int iOffsetMinY = 100.f;
	int iOffsetMaxY = 500.f;

	int iScrollX = (int)ScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)ScrollManager::Get_Instance()->Get_ScrollY();
	
	if (iOffsetMinX > m_tInfo.fX + iScrollX)
		ScrollManager::Get_Instance()->Set_ScrollX(m_fSpeed);

	if (iOffsetMaxX < m_tInfo.fX + iScrollX)
		ScrollManager::Get_Instance()->Set_ScrollX(-m_fSpeed);

	if (iOffsetMinY > m_tInfo.fY + iScrollY)
		ScrollManager::Get_Instance()->Set_ScrollY(m_fSpeed);

	if (iOffsetMaxY < m_tInfo.fY + iScrollY)
		ScrollManager::Get_Instance()->Set_ScrollY(-m_fSpeed);
}

void Player::Gravity()
{
	bool bFloor = false;
	float fTargetY = 0.f;

	if (m_eCurState != JUMP)
		// If TRUE there is a Collision Tile below
		// If FALSE there is NO Collision Tile below
		bFloor = TileManager::Get_Instance()->Tile_Collision(m_tInfo.fX, m_tInfo.fY, (m_tFrameInfoRender.fCY / 2) - 12, &fTargetY); // 6: Distance from end of Sprite to end of the Frame (in Pixels)
		
	// JUMPING
	if (m_bIsJumping)
	{	
		float fJumpPos = m_fJumpPower * m_fJumpTime - (m_fAccel * pow(m_fJumpTime, 2) * 0.5f);
		
		m_tInfo.fY -= fJumpPos;
		m_fJumpTime += 0.15f;

		// Highest point reached: Start FALL Animation
		if ((m_tInfo.fY - fJumpPos) > m_tInfo.fY)
		{
			m_bIsJumping = false;
			m_fJumpTime = 0.f;
		}
	}

	// NOT JUMPING
		// There is Floor and Player is above Target Tile
	else if (bFloor && m_tInfo.fY < fTargetY)
	{
		m_tInfo.fY += m_fFallSpeed;
		m_eCurState = FALL;
	}
		// There is Floor but Player is NOT above Target Tile
	else if (bFloor)
		m_tInfo.fY = fTargetY;
		// There is NO Floor
	else
	{
		m_tInfo.fY += m_fFallSpeed;
		m_eCurState = FALL;
	}
}

void Player::Move(bool bIsRight)
{
	bool bIsSprinting = KeyManager::Get_Instance()->Key_Pressing(VK_SHIFT);
	float fSpeed = bIsSprinting ? m_fSprintSpeed : m_fSpeed;
	STATE eNewState = bIsSprinting ? SPRINT : RUN;

	m_eDir = bIsRight ? DIR_RIGHT : DIR_LEFT;
	m_tInfo.fX += bIsRight ? fSpeed : -fSpeed;
	m_pFrameKey = bIsRight ? L"Player_RIGHT" : L"Player_LEFT";
	m_eCurState = m_bIsJumping ? m_eCurState : eNewState;
}

void Player::Attack()
{
	if (!m_bIsAttacking)
	{
		m_eCurState = ATTACK_1;
		m_bIsAttacking = true;
	}
	else if (m_eCurState != ATTACK_5)
		m_bIsComboActive = true;
}

void Player::Attack_Special()
{
	m_dwChargingTime = GetTickCount() - m_dwChargingTime;

	// TODO: Implement Attack_Special
}

void Player::Change_Motion()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 0;
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case RUN:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iMotion = 1;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case SPRINT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iMotion = 2;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case JUMP:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 3;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case FALL:
			m_tFrame.iFrameStart = 4;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iMotion = 3;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ATTACK_1:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iDamageNotifyStart = 1;
			m_tFrame.iDamageNotifyEnd = 1;
			m_tFrame.iMotion = 4;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ATTACK_2:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iDamageNotifyStart = 1;
			m_tFrame.iDamageNotifyEnd = 1;
			m_tFrame.iMotion = 5;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ATTACK_3:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iDamageNotifyStart = 4;
			m_tFrame.iDamageNotifyEnd = 4;
			m_tFrame.iMotion = 6;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ATTACK_4:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iDamageNotifyStart = 4;
			m_tFrame.iDamageNotifyEnd = 5;
			m_tFrame.iMotion = 7;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ATTACK_5:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iDamageNotifyStart = 2;
			m_tFrame.iDamageNotifyEnd = 3;
			m_tFrame.iMotion = 8;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ATTACK_JUMP:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iMotion = 9;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CHARGING:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 12;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ATTACK_SPECIAL:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 13;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case HIT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 10;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 11;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
		}

		m_ePreState = m_eCurState;
	}
}

void Player::Change_Frame()
{
	// NO LOOP Animations:
	// JUMP, FALL, ATTACK_1, ATTACK_2, ATTACK_3, ATTACK_4, ATTACK_5, ATTACK_JUMP, HIT, DEAD
	if (m_eCurState == JUMP || m_eCurState == FALL || m_eCurState == ATTACK_JUMP ||
		m_eCurState == ATTACK_1 || m_eCurState == ATTACK_2 || m_eCurState == ATTACK_3 || m_eCurState == ATTACK_4 || m_eCurState == ATTACK_5 ||
		m_eCurState == HIT || m_eCurState == DEAD)
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
	// IDLE, RUN, SPRINT, CHARGING, ATTACK_SPECIAL
	else
	{
		if (GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed)
		{
			m_tFrame.iFrameStart++;

			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				if (m_eCurState == CHARGING)
					m_tFrame.iFrameStart = 2;
				else 
					m_tFrame.iFrameStart = 0;
			}
				
			m_tFrame.dwFrameTime = GetTickCount();
		}
	}
}

bool Player::Die()
{
	if (m_eCurState == DEAD && m_tFrame.iFrameStart == m_tFrame.iFrameEnd && GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed + 1000)
		return true;
	else if (m_bDead && m_eCurState == HIT && m_tFrame.iFrameStart == m_tFrame.iFrameEnd && GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed)
		m_eCurState = DEAD;
	else if (m_bDead)
		m_bIsHit = true;

	return false;
}

int Player::Get_ColSize()
{
	switch (m_eCurState)
	{
	case ATTACK_3:
		return 50;
	case ATTACK_4:
		return 80;
	case ATTACK_5:
		return 90;
	default:
		return 35;
	}
}

void Player::Can_Damage()
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

void Player::Reset_Animation()
{
	// Reset ATTACK
	if (m_bIsAttacking && m_tFrame.iFrameStart == m_tFrame.iFrameEnd && GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed)
	{
		if (m_bIsComboActive)
		{
			switch (m_eCurState)
			{
			case ATTACK_1:
				m_eCurState = ATTACK_2;
				m_bIsAttacking = true;
				m_bIsComboActive = false;
				m_bMotionAlreadyDamaged = false;
				break;
			case ATTACK_2:
				m_eCurState = ATTACK_3;
				m_bIsAttacking = true;
				m_bIsComboActive = false;
				m_bMotionAlreadyDamaged = false;
				break;
			case ATTACK_3:
				m_eCurState = ATTACK_4;
				m_bIsAttacking = true;
				m_bIsComboActive = false;
				m_bMotionAlreadyDamaged = false;
				break;
			case ATTACK_4:
				m_eCurState = ATTACK_5;
				m_bIsAttacking = true;
				m_bIsComboActive = false;
				m_bMotionAlreadyDamaged = false;
				break;
			case ATTACK_5:
				m_bIsAttacking = true;
				m_bIsComboActive = false;
				m_bMotionAlreadyDamaged = false;
				break;
			}
		}
		else
		{
			m_bIsAttacking = false;
			m_bIsComboActive = false;
			m_bMotionAlreadyDamaged = false;
		}
	}

	// Reset HIT
	if (m_eCurState == HIT && m_tFrame.iFrameStart == m_tFrame.iFrameEnd && GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed)
		m_bIsHit = false;
}