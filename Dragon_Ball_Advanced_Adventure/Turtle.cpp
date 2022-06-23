#include "stdafx.h"
#include "Turtle.h"
#include "BmpManager.h"
#include "TileManager.h"
#include "ScrollManager.h"

Turtle::Turtle()
{
}

Turtle::~Turtle()
{
}

void Turtle::Initialize()
{
	// NPC Rect Size
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;

	// Sprite REAL Size
	m_tFrameInfo.fCX = 50.f;
	m_tFrameInfo.fCY = 50.f;

	// Sprite RENDER Size
	m_tFrameInfoRender.fCX = m_tFrameInfo.fCX * 2;
	m_tFrameInfoRender.fCY = m_tFrameInfo.fCY * 2;

	m_fSpeed = 3.f;

	m_pFrameKey = L"Turtle";
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Turtle.bmp", m_pFrameKey);

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.iMotion = 0;
	m_tFrame.dwFrameSpeed = 70;
	m_tFrame.dwFrameTime = GetTickCount();
}

void Turtle::Release()
{
}

int Turtle::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Gravity();
	m_tInfo.fX -= m_fSpeed;
	Update_Rect();

	return OBJ_NOEVENT;
}

void Turtle::Late_Update()
{
	Die();
	Change_Frame();
}

void Turtle::Render(HDC hDC)
{
	int iScrollX = (int)ScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)ScrollManager::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = BmpManager::Get_Instance()->Find_Bmp(m_pFrameKey);

	// Test NPC Rectangle
	//Rectangle(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

	float fRectFrameDiffX = (m_tFrameInfoRender.fCX - m_tInfo.fCX) / 2;
	float fRectFrameDiffY = (m_tFrameInfoRender.fCY - m_tInfo.fCY) / 2;

	GdiTransparentBlt(
		hDC, m_tRect.left - fRectFrameDiffX + iScrollX, m_tRect.top - fRectFrameDiffY + iScrollY, m_tFrameInfoRender.fCX, m_tFrameInfoRender.fCY,
		hMemDC, m_tFrame.iFrameStart * m_tFrameInfo.fCX, m_tFrame.iMotion * m_tFrameInfo.fCY, m_tFrameInfo.fCX, m_tFrameInfo.fCY, RGB(0, 152, 128));
}

void Turtle::Gravity()
{
	float fTargetY = 0.f;
	bool bFloor = false;

	// If TRUE there is a Collision Tile below
	// If FALSE there is NO Collision Tile below
	bFloor = TileManager::Get_Instance()->Tile_Collision(m_tInfo.fX, m_tInfo.fY, (m_tFrameInfoRender.fCY / 2) - 18, &fTargetY);
	if (bFloor)
		m_tInfo.fY = fTargetY;
}

bool Turtle::Die()
{
	if ((m_tRect.left) <= 0 || (m_tRect.right) >= WINCX)
	{
		m_bDead = true;
		return true;
	}

	return false;
}

int Turtle::Get_ColSize()
{
	return 0;
}

void Turtle::Can_Damage()
{
}

void Turtle::Reset_Animation()
{
}

void Turtle::Sound_On_Animation()
{
}