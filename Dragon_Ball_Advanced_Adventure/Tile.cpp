#include "stdafx.h"
#include "Tile.h"
#include "BmpManager.h"
#include "ScrollManager.h"

Tile::Tile() : m_iDrawID{ 0 }, m_iOption{ 0 }
{
}

Tile::~Tile()
{
	Release();
}

void Tile::Initialize()
{
	m_tInfo.fCX = TILECX;
	m_tInfo.fCY = TILECY;

	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Editor/Tile.bmp", L"Tile");
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
	int iScrollX = (int)ScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)ScrollManager::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = BmpManager::Get_Instance()->Find_Bmp(L"Tile");
	BitBlt(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, hMemDC, TILECX * m_iDrawID, 0, SRCCOPY);
}