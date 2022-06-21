#include "stdafx.h"
#include "BearThiefSpecialAttack.h"
#include "ScrollManager.h"
#include "BmpManager.h"

BearThiefSpecialAttack::BearThiefSpecialAttack()
{
}

BearThiefSpecialAttack::~BearThiefSpecialAttack()
{
}

void BearThiefSpecialAttack::Initialize()
{
	// Projectile Rect Size
	m_tInfo.fCX = 70.f;
	m_tInfo.fCY = 60.f;

	// Sprite REAL Size
	m_tFrameInfo.fCX = 50.f;
	m_tFrameInfo.fCY = 50.f;

	// Sprite RENDER Size
	m_tFrameInfoRender.fCX = 100.f;
	m_tFrameInfoRender.fCY = 100.f;

	m_fSpeed = 5.f;

	m_pFrameKey = m_eDir == DIR_RIGHT ? L"Bear_Thief_Attack_Special_RIGHT" : L"Bear_Thief_Attack_Special_LEFT";

	// Frame
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 2;
	m_tFrame.iMotion = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();
}

void BearThiefSpecialAttack::Release()
{
}

int BearThiefSpecialAttack::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fX += m_eDir == DIR_RIGHT ? m_fSpeed : -m_fSpeed;

	Update_Rect();

	return OBJ_NOEVENT;
}

void BearThiefSpecialAttack::Late_Update()
{
	// TODO: Destroy if out of screen

	Change_Frame();

	int iScrollX = (int)ScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)ScrollManager::Get_Instance()->Get_ScrollY();

	if ((m_tRect.left + iScrollX) <= 0 || (m_tRect.right + iScrollX) >= WINCX)
		m_bDead = true;
}

void BearThiefSpecialAttack::Render(HDC hDC)
{
	int iScrollX = (int)ScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)ScrollManager::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = BmpManager::Get_Instance()->Find_Bmp(m_pFrameKey);

	// Test Enemy Rectangle
	//Rectangle(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

	float fRectFrameDiffX = (m_tFrameInfoRender.fCX - m_tInfo.fCX) / 2;
	float fRectFrameDiffY = (m_tFrameInfoRender.fCY - m_tInfo.fCY) / 2;

	GdiTransparentBlt(
		hDC, m_tRect.left - fRectFrameDiffX + iScrollX, m_tRect.top - fRectFrameDiffY + iScrollY, m_tFrameInfoRender.fCX, m_tFrameInfoRender.fCY,
		hMemDC, m_tFrame.iFrameStart * m_tFrameInfo.fCX, m_tFrame.iMotion * m_tFrameInfo.fCY, m_tFrameInfo.fCX, m_tFrameInfo.fCY, RGB(10, 87, 106));
}

bool BearThiefSpecialAttack::Get_CanDamage()
{
	return m_bCanDamage;
}