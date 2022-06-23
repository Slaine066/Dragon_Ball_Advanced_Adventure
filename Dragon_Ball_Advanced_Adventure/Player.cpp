#include "stdafx.h"
#include "Player.h"
#include "BmpManager.h"
#include "ScrollManager.h"
#include "BmpManager.h"
#include "KeyManager.h"
#include "TileManager.h"
#include "ObjManager.h"
#include "SoundManager.h"
#include "AbstractFactory.h"
#include "Kamehameha.h"
#include "EnergySphere.h"
#include "UIManager.h"
#include "HealthBar.h"
#include "EnergyBar.h"
#include "ComboCounter.h"

extern float g_fSound;

Player::Player() 
	: m_ePreState(END), m_eCurState(IDLE), m_fSprintSpeed(0.f), m_fFallSpeed(6.f), 
	m_bIsJumping(false), m_fJumpPower(0.f), m_fJumpTime(0.f), m_fAccel(9.8f),
	m_iComboCounter(0), m_bIsComboActive(false), 
	m_dwComboTime(GetTickCount()), m_dwChargeTime(GetTickCount()), m_dwEnergyReloadTime(GetTickCount()), m_bSpecialNoLoop(false)
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	// Player Rect Size
	m_tInfo.fCX = 60.f;
	m_tInfo.fCY = 70.f;

	// Sprite REAL Size
	m_tFrameInfo.fCX = 100.f;
	m_tFrameInfo.fCY = 50.f;

	// Sprite RENDER Size
	m_tFrameInfoRender.fCX = 200.f;
	m_tFrameInfoRender.fCY = 100.f;

	// Stats
	m_tStats.iHealthMax = 100.f;
	m_tStats.iHealth = m_tStats.iHealthMax;
	m_tStats.iEnergyMax = 50.f;
	m_tStats.iEnergy = m_tStats.iEnergyMax;
	m_tStats.iDamage = 10.f;
	m_tStats.iDamageOffset = 3;

	// Speeds
	m_fSpeed = 3.f;
	m_fSprintSpeed = 5.f;
	m_fJumpPower = 14.f;

	m_eDir = DIR_RIGHT;
	m_eObjId = OBJ_PLAYER;

	m_pFrameKey = L"Player_RIGHT";

	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Player/Goku_LEFT.bmp", L"Player_LEFT");
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Player/Goku_RIGHT.bmp", L"Player_RIGHT");
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Player/Kamehameha.bmp", L"Kamehameha");
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Player/Kamehameha_Small.bmp", L"Kamehameha_Small");
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Player/Energy_Sphere_LEFT.bmp", L"Energy_Sphere_LEFT");
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Player/Energy_Sphere_RIGHT.bmp", L"Energy_Sphere_RIGHT");
	
	// Start First Animation
	Change_Motion(); 

	// UI
	UIManager::Get_Instance()->Add_Object(UI_HEALTH_BAR, AbstractFactory<HealthBar>::Create(150, 43));
	UIManager::Get_Instance()->Add_Object(UI_ENERGY_BAR, AbstractFactory<EnergyBar>::Create(75, 560));
	UIManager::Get_Instance()->Add_Object(UI_COMBO_COUNTER, AbstractFactory<ComboCounter>::Create(700, 200));
	UIManager::Get_Instance()->Initialize();
}

void Player::Release()
{
}

int Player::Update()
{
	if (Die())
		return OBJ_DEAD;

	Gravity();
	Can_Damage();
	Key_Input();
	Offset();
	Jump();
	Reload_Energy();
	Reset_Combo_Counter();
	
	Update_Rect();
	Update_Collision_Rect(10, Get_ColSize());

	return OBJ_NOEVENT;
}

void Player::Late_Update()
{
	Check_Combo();
	Change_Motion();
	Change_Frame();
	Sound_On_Animation();
	Reset_Animation();
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
	// MOVEMENT
		// Left Arrow - Move Left
	if (KeyManager::Get_Instance()->Key_Pressing(VK_LEFT) && !m_bIsAttacking && !m_bIsHit && m_eCurState != CHARGING && m_eCurState != ATTACK_SPECIAL)
		Move(false);
		// Right Arrow - Move Right
	else if (KeyManager::Get_Instance()->Key_Pressing(VK_RIGHT) && !m_bIsAttacking && !m_bIsHit  && m_eCurState != CHARGING && m_eCurState != ATTACK_SPECIAL)
		Move(true);
		// NO Input - Idle
	else if (!m_bIsJumping && !m_bIsAttacking && !m_bIsHit && m_eCurState != CHARGING && m_eCurState != ATTACK_SPECIAL && !m_bDead && m_eCurState != DEAD)
		m_eCurState = IDLE;
		

	// ACTION
		// Space Bar - Jump
	if (KeyManager::Get_Instance()->Key_Down(VK_SPACE) && !m_bIsJumping && !m_bIsAttacking && !m_bIsHit)
	{
		m_bIsJumping = true;
		m_eCurState = JUMP;
		m_tStats.iCharge = 0;
	}
		// Keyboard 'A' - Attack
	if (KeyManager::Get_Instance()->Key_Down('A') && !m_bIsHit && !m_bIsJumping && m_eCurState != CHARGING && m_eCurState != ATTACK_SPECIAL)
		Attack();
		// Keyboard 'S' Pressing - Charging
	if (KeyManager::Get_Instance()->Key_Pressing('S') && !m_bIsHit && !m_bIsJumping && !m_bIsAttacking)
		Charge();
		// Keyboard 'S' Up - Special Attack
	if (KeyManager::Get_Instance()->Key_Up('S') && !m_bIsHit && !m_bIsJumping && !m_bIsAttacking)
		Attack_Special();
	
	if (m_bIsHit && 
		(m_eCurState != ATTACK_SPECIAL || (m_eCurState == ATTACK_SPECIAL && m_bDead) || (m_eCurState == ATTACK_SPECIAL && m_bSpecialNoLoop))
		&& m_eCurState != DEAD)
	{
		m_eCurState = HIT;
		m_tStats.iCharge = 0;
		m_bIsAttacking = false;
	}
}

void Player::Offset()
{
	int iOffsetMinX = 200.f;
	int iOffsetMaxX = 400.f;

	int iOffsetMinY = 300.f;
	int iOffsetMaxY = 400.f;

	int iScrollX = (int)ScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)ScrollManager::Get_Instance()->Get_ScrollY();
	
	if (iOffsetMinX > m_tInfo.fX + iScrollX)
		ScrollManager::Get_Instance()->Set_ScrollX(m_eCurState == SPRINT ? m_fSprintSpeed : m_fSpeed);

	if (iOffsetMaxX < m_tInfo.fX + iScrollX)
		ScrollManager::Get_Instance()->Set_ScrollX(m_eCurState == SPRINT ? -m_fSprintSpeed : -m_fSpeed);

	if (iOffsetMinY > m_tInfo.fY + iScrollY)
		ScrollManager::Get_Instance()->Set_ScrollY(m_fSpeed);

	if (iOffsetMaxY < m_tInfo.fY + iScrollY)
		ScrollManager::Get_Instance()->Set_ScrollY(-m_fSpeed);
}

void Player::Gravity()
{
	if (m_bDead || m_eCurState == DEAD)
		return;

	bool bFloor = false;
	float fTargetY = 0.f;

	if (m_eCurState != JUMP && m_eCurState != FALL)
		bFloor = TileManager::Get_Instance()->Tile_Collision(m_tInfo.fX, m_tInfo.fY, (m_tFrameInfoRender.fCY / 2) - 12, &fTargetY);

	if (bFloor && !m_bIsJumping && !m_bIsAttacking && !m_bIsHit && m_eCurState != CHARGING)
	{
		m_eCurState = IDLE;
		m_tInfo.fY = fTargetY;
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

void Player::Jump()
{
	if (m_bDead || m_eCurState == DEAD)
		return;

	bool bFloor = false;
	float fTargetY = 0.f;

	if (m_eCurState != JUMP)
		bFloor = TileManager::Get_Instance()->Tile_Collision(m_tInfo.fX, m_tInfo.fY, (m_tFrameInfoRender.fCY / 2) - 12, &fTargetY);

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
		m_eCurState = m_bIsHit ? HIT : FALL;
	}
	// There is Floor but Player is NOT above Target Tile
	else if (!bFloor)
	{
		m_tInfo.fY += m_fFallSpeed;
		m_eCurState = m_bIsHit ? HIT : FALL;
	}
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

void Player::Charge()
{
	if (m_tStats.iCharge < 100)
	{
		if (m_eCurState != CHARGING)
			m_eCurState = CHARGING;

		if (GetTickCount() > m_dwChargeTime + 200)
		{
			m_tStats.iCharge += 10;
			if (m_tStats.iCharge > m_tStats.iEnergy)
				m_tStats.iCharge = m_tStats.iEnergy;
			m_dwChargeTime = GetTickCount();
		}
	}
}

void Player::Attack_Special()
{
	// If 'S' has been held more than 2 seconds
	if (m_tStats.iEnergy == 100 && m_tStats.iCharge == 100)
	{
		m_eCurState = ATTACK_SPECIAL;
		m_bIsAttacking = true;
		m_tStats.iEnergy = 0;

		// Spawn Kamehameha
		float fOffset = m_eDir == DIR_RIGHT ? m_tInfo.fCX / 2 : -m_tInfo.fCX / 2;
		Obj* pObj = AbstractFactory<Kamehameha>::Create(m_tInfo.fX + fOffset, m_tInfo.fY + 4, m_eDir, m_eObjId, this);
		ObjManager::Get_Instance()->Add_Object(OBJ_PROJECTILE, pObj);
		Kamehameha* pKamehameha = static_cast<Kamehameha*>(pObj);
		pKamehameha->Set_IsBig();
		SoundManager::Get_Instance()->PlaySound(L"Kamehameha.wav", CHANNEL_VOICE, g_fSound);
	}
	else if (m_tStats.iEnergy >= 75 && m_tStats.iCharge >= 75)
	{
		m_eCurState = ATTACK_SPECIAL;
		m_bIsAttacking = true;
		m_tStats.iEnergy = m_tStats.iEnergy - m_tStats.iCharge;

		// Spawn Kamehameha Small
		float fOffset = m_eDir == DIR_RIGHT ? m_tInfo.fCX / 2 : -m_tInfo.fCX / 2;
		ObjManager::Get_Instance()->Add_Object(OBJ_PROJECTILE, AbstractFactory<Kamehameha>::Create(m_tInfo.fX + fOffset, m_tInfo.fY + 4, m_eDir, m_eObjId, this));
		SoundManager::Get_Instance()->PlaySound(L"Kamehameha_Small.wav", CHANNEL_VOICE, g_fSound);
	}
	else if (m_tStats.iEnergy >= 50 && m_tStats.iCharge >= 50)
	{
		m_eCurState = ATTACK_SPECIAL;
		m_bIsAttacking = true;
		m_bSpecialNoLoop = true;
		m_tStats.iEnergy = m_tStats.iEnergy - m_tStats.iCharge;

		// Spawn Energy Sphere
		float fOffset = m_eDir == DIR_RIGHT ? m_tInfo.fCX / 2 : -m_tInfo.fCX / 2;
		ObjManager::Get_Instance()->Add_Object(OBJ_PROJECTILE, AbstractFactory<EnergySphere>::Create(m_tInfo.fX + fOffset, m_tInfo.fY + 4, m_eDir, m_eObjId, this));
		SoundManager::Get_Instance()->PlaySound(L"Energy_Sphere.wav", CHANNEL_VOICE, g_fSound);
	}
	else
		m_eCurState = IDLE;

	m_tStats.iCharge = 0;
}

void Player::Reload_Energy()
{
	if (m_tStats.iEnergy < m_tStats.iEnergyMax)
	{
		if (GetTickCount() > m_dwEnergyReloadTime + 70)
		{
			m_tStats.iEnergy += 1;
			m_dwEnergyReloadTime = GetTickCount();
		}
	}
}

void Player::Check_Combo()
{
	if (m_bIsAttacking && m_eCurState != ATTACK_SPECIAL && m_tFrame.iFrameStart == m_tFrame.iFrameEnd && GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed)
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
	else if (m_bIsAttacking && m_eCurState == ATTACK_SPECIAL && m_bSpecialNoLoop&& m_tFrame.iFrameStart == m_tFrame.iFrameEnd && GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed)
	{
		m_bIsAttacking = false;
		m_bSpecialNoLoop = false;
	}
}

void Player::Reset_Combo_Counter()
{
	if (GetTickCount() > m_dwComboTime + 1500)
	{
		m_iComboCounter = 0;
		m_dwComboTime = GetTickCount();
	}
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
			m_bCanPlaySound = true;
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
			m_bCanPlaySound = true;
			break;
		case ATTACK_4:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iDamageNotifyStart = 4;
			m_tFrame.iDamageNotifyEnd = 5;
			m_tFrame.iMotion = 7;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bCanPlaySound = true;
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
			m_bCanPlaySound = true;
			break;
		case HIT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 10;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bCanPlaySound = true;
			break;
		case DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 11;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			m_bCanPlaySound = true;
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
		(m_eCurState == ATTACK_SPECIAL && m_bSpecialNoLoop) ||
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
	float fY = ScrollManager::Get_Instance()->Get_ScrollY();
	if (m_tInfo.fY > WINCY + abs(fY))
	{
		if (m_eCurState == DEAD && m_tFrame.iFrameStart == m_tFrame.iFrameEnd && GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed + 1000)
			return true;
		else
			m_eCurState = DEAD;
	}
	else
	{
		if (m_eCurState == DEAD && m_tFrame.iFrameStart == m_tFrame.iFrameEnd && GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed + 1000)
			return true;
		else if (m_bDead && m_eCurState == HIT && m_tFrame.iFrameStart == m_tFrame.iFrameEnd && GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed)
			m_eCurState = DEAD;
		else if (m_bDead)
			m_bIsHit = true;
	}

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
	else
		m_bCanDamage = false;
}

void Player::Reset_Animation()
{
	// Reset HIT
	if (m_eCurState == HIT && m_tFrame.iFrameStart == m_tFrame.iFrameEnd && GetTickCount() > m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed)
		m_bIsHit = false;

}

void Player::Sound_On_Animation()
{
	switch (m_eCurState)
	{
	case JUMP:
		break;
	case FALL:
		break;
	case ATTACK_1:
		if (m_tFrame.iFrameStart == m_tFrame.iSoundNotifyStart && m_bCanPlaySound)
		{
			SoundManager::Get_Instance()->PlaySound(L"Goku_Attack_1.wav", CHANNEL_VOICE, g_fSound);
			m_bCanPlaySound = false;
		}
		break;
	case ATTACK_3:
		if (m_tFrame.iFrameStart == m_tFrame.iSoundNotifyStart && m_bCanPlaySound)
		{
			SoundManager::Get_Instance()->PlaySound(L"Goku_Attack_2.wav", CHANNEL_VOICE, g_fSound);
			m_bCanPlaySound = false;
		}
		break;
	case ATTACK_4:
		if (m_tFrame.iFrameStart == m_tFrame.iSoundNotifyStart && m_bCanPlaySound)
		{
			SoundManager::Get_Instance()->PlaySound(L"Goku_Attack_3.wav", CHANNEL_VOICE, g_fSound);
			m_bCanPlaySound = false;
		}
		break;
	case CHARGING:
		break;
	case HIT:
		if (m_tFrame.iFrameStart == m_tFrame.iSoundNotifyStart && m_bCanPlaySound)
		{
			SoundManager::Get_Instance()->PlaySound(L"Goku_Hit.wav", CHANNEL_VOICE, g_fSound);
			m_bCanPlaySound = false;
		}
		break;
	case DEAD:
		if (m_tFrame.iFrameStart == m_tFrame.iSoundNotifyStart && m_bCanPlaySound)
		{
			SoundManager::Get_Instance()->PlaySound(L"Goku_Dead.wav", CHANNEL_VOICE, g_fSound);
			m_bCanPlaySound = false;
		}
		break;
	}
}