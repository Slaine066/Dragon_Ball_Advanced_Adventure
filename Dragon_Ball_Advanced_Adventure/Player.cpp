#include "stdafx.h"
#include "Player.h"
#include "BmpManager.h"
#include "ScrollManager.h"
#include "BmpManager.h"
#include "KeyManager.h"
#include "TileManager.h"

Player::Player() 
	: m_ePreState(END), m_eCurState(IDLE), m_fSprintSpeed(0.f), m_bJump(false), m_bIsInAir(false), m_fJumpPower(0.f), m_fJumpTime(0.f), m_fAccel(9.8f), 
	m_bIsAttacking(false), m_bIsComboActive(false)
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;

	// Sprite Real Size
	m_tFrameInfo.fCX = 100.f;
	m_tFrameInfo.fCY = 50.f;

	m_fSpeed = 3.f;
	m_fSprintSpeed = 5.f;
	m_fJumpPower = 15.f;

	m_pFrameKey = L"Player_RIGHT";

	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Player/Goku_LEFT.bmp", L"Player_LEFT");
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Player/Goku_RIGHT.bmp", L"Player_RIGHT");
	// TODO: Load Projectile *.bmp
	
	// Start First Animation
	Change_Motion(); 
}

void Player::Release()
{
}

int Player::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Key_Input();
	Offset();
	Gravity();

	Update_Rect();

	return OBJ_NOEVENT;
}

void Player::Late_Update()
{
	Change_Motion();
	Change_Frame();
	Check_Combo();
}

void Player::Render(HDC hDC)
{
	int iScrollX = (int)ScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)ScrollManager::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = BmpManager::Get_Instance()->Find_Bmp(m_pFrameKey);

	// Test Rectangle
	//Rectangle(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

	float fRectFrameDiffX = (m_tFrameInfo.fCX - m_tInfo.fCX) / 2;
	float fRectFrameDiffY = (m_tFrameInfo.fCY - m_tInfo.fCY) / 2;

	GdiTransparentBlt(
		hDC, m_tRect.left - fRectFrameDiffX + iScrollX, m_tRect.top - fRectFrameDiffY + iScrollY, m_tFrameInfo.fCX, m_tFrameInfo.fCY,
		hMemDC, m_tFrame.iFrameStart * m_tFrameInfo.fCX, m_tFrame.iMotion * m_tFrameInfo.fCY, m_tFrameInfo.fCX, m_tFrameInfo.fCY, RGB(132, 0, 132));
}

void Player::Key_Input()
{
	// Left Arrow
	if (KeyManager::Get_Instance()->Key_Pressing(VK_LEFT) && !m_bIsAttacking)
		Move(false);

	// Right Arrow
	else if (KeyManager::Get_Instance()->Key_Pressing(VK_RIGHT) && !m_bIsAttacking)
		Move(true);

	else if (!m_bJump && !m_bIsAttacking)
		m_eCurState = IDLE;

	// Space Bar
	if (KeyManager::Get_Instance()->Key_Down(VK_SPACE) && !m_bIsInAir && !m_bIsAttacking)
	{
		m_bJump = true;
		m_eCurState = JUMP;
	}

	// Left Mouse
	if (KeyManager::Get_Instance()->Key_Down('A'))
	{
		if (m_bJump)
			m_eCurState = ATTACK_JUMP;
		else
			Attack();
	}
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
	float fY = 0.f;
	
	// If TRUE there is a Collision Tile below
	// If FALSE there is NO Collision Tile below
	bool bFloor = TileManager::Get_Instance()->Tile_Collision(m_tInfo.fX, m_tInfo.fY, (m_tFrameInfo.fCY / 2) - 6, &fY); // 6: Distance from end of to the end of the Frame

	// Jump
	if (m_bJump)
	{
		float fPos = m_fJumpPower * m_fJumpTime - (m_fAccel * pow(m_fJumpTime, 2) * 0.5f);

		// Highest point reached: 
		// Start FALL Animation
		/*if ((m_tInfo.fY - fPos) > m_tInfo.fY && m_eCurState != FALL)
			m_eCurState = FALL;*/

		m_tInfo.fY -= fPos;
		m_fJumpTime += 0.15f;

		if (bFloor && fY < m_tInfo.fY)
		{
			m_bJump = false;
			m_fJumpTime = 0.f;
			m_tInfo.fY = fY;
		}
	}

	// Floor Collision
	else if (bFloor)
	{
		m_tInfo.fY = fY;
	}
	else
	{
		m_tInfo.fY += m_fSpeed;
		m_eCurState = FALL;
	}
}

void Player::Move(bool bIsRight)
{
	bool bIsSprinting = KeyManager::Get_Instance()->Key_Pressing(VK_SHIFT);
	float fSpeed = bIsSprinting ? m_fSprintSpeed : m_fSpeed;
	STATE eNewState = bIsSprinting ? SPRINT : RUN;

	m_tInfo.fX += bIsRight ? fSpeed : -fSpeed;
	m_pFrameKey = bIsRight ? L"Player_RIGHT" : L"Player_LEFT";
	m_eCurState = m_bJump ? m_eCurState : eNewState;
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
			m_tFrame.iMotion = 4;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ATTACK_2:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 5;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ATTACK_3:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iMotion = 6;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ATTACK_4:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iMotion = 7;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ATTACK_5:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 8;
			m_tFrame.dwFrameSpeed = 100;
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
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 12;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ATTACK_SPECIAL:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
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
	// JUMP, FALL, ATTACK_1, ATTACK_2, ATTACK_3, ATTACK_4, ATTACK_5, ATTACK_JUMP, CHARGING, ATTACK_SPECIAL, HIT, DEAD
	if (m_eCurState == JUMP || m_eCurState == FALL || m_eCurState == ATTACK_JUMP ||
		m_eCurState == ATTACK_1 || m_eCurState == ATTACK_2 || m_eCurState == ATTACK_3 || m_eCurState == ATTACK_4 || m_eCurState == ATTACK_5 ||
		m_eCurState == CHARGING || m_eCurState == ATTACK_SPECIAL ||
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
	// IDLE, RUN, SPRINT
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

void Player::Check_Combo()
{
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
				break;
			case ATTACK_2:
				m_eCurState = ATTACK_3;
				m_bIsAttacking = true;
				m_bIsComboActive = false;
				break;
			case ATTACK_3:
				m_eCurState = ATTACK_4;
				m_bIsAttacking = true;
				m_bIsComboActive = false;
				break;
			case ATTACK_4:
				m_eCurState = ATTACK_5;
				m_bIsAttacking = true;
				m_bIsComboActive = false;
				break;
			case ATTACK_5:
				m_bIsAttacking = true;
				m_bIsComboActive = false;
			}
		}
		else
		{
			m_bIsAttacking = false;
			m_bIsComboActive = false;
		}
	}
}