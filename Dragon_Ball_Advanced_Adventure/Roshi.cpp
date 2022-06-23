#include "stdafx.h"
#include "Roshi.h"
#include "BmpManager.h"
#include "ScrollManager.h"
#include "TileManager.h"

Roshi::Roshi()
{
}

Roshi::~Roshi()
{
}

void Roshi::Initialize()
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

	m_pFrameKey = L"Roshi";
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Roshi.bmp", m_pFrameKey);

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iMotion = 0;
	m_tFrame.dwFrameSpeed = 70;
	m_tFrame.dwFrameTime = GetTickCount();
}

void Roshi::Release()
{
}

int Roshi::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Gravity();

	Update_Rect();

	return OBJ_NOEVENT;
}

void Roshi::Late_Update()
{
}

void Roshi::Render(HDC hDC)
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

void Roshi::Gravity()
{
	float fTargetY = 0.f;
	bool bFloor = false;

	// If TRUE there is a Collision Tile below
	// If FALSE there is NO Collision Tile below
	bFloor = TileManager::Get_Instance()->Tile_Collision(m_tInfo.fX, m_tInfo.fY, (m_tFrameInfoRender.fCY / 2) - 6, &fTargetY);
	if (bFloor)
		m_tInfo.fY = fTargetY;
}

bool Roshi::Die()
{
	return false;
}

int Roshi::Get_ColSize()
{
	return 0;
}

void Roshi::Can_Damage()
{
}

void Roshi::Reset_Animation()
{
}

void Roshi::Sound_On_Animation()
{
}