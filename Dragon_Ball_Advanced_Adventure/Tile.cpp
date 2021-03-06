#include "stdafx.h"
#include "Tile.h"
#include "BmpManager.h"
#include "ScrollManager.h"

Tile::Tile() : m_iDrawID(-1), m_iOption(0), m_bIsBossTile(false)
{
}

Tile::~Tile()
{
	Release();
}

void Tile::Initialize()
{
	// Tile Size 
	m_tInfo.fCX = TILECX;
	m_tInfo.fCY = TILECY;

	// Tile Frame Real Size
	m_tFrameInfo.fCX = 64.f;
	m_tFrameInfo.fCY = 64.f;

	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Editor/Tile.bmp", L"Tile");
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Editor/Boss_Tile.bmp", L"Boss Tile");
}

void Tile::Release()
{
}

int Tile::Update()
{
	Update_Rect();

	return OBJ_NOEVENT;
}

void Tile::Late_Update()
{
}

void Tile::Render(HDC hDC)
{
	if (m_iDrawID != -1)
	{
		int iScrollX = (int)ScrollManager::Get_Instance()->Get_ScrollX();
		int iScrollY = (int)ScrollManager::Get_Instance()->Get_ScrollY();

		HDC	hMemDC = BmpManager::Get_Instance()->Find_Bmp(m_bIsBossTile ? L"Boss Tile" : L"Tile");

		GdiTransparentBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tInfo.fCX, m_tInfo.fCY, 
			hMemDC, m_tFrameInfo.fCX * m_iDrawID, 0, m_tFrameInfo.fCX, m_tFrameInfo.fCY, RGB(24, 176, 248));
	}
}