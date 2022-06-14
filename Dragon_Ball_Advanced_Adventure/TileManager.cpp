#include "stdafx.h"
#include "TileManager.h"
#include "AbstractFactory.h"
#include "Tile.h"
#include "ScrollManager.h"
#include "Function.h"

TileManager* TileManager::m_pInstance = nullptr;

TileManager::TileManager()
{
}

TileManager::~TileManager()
{
	Release();
}

void TileManager::Initialize()
{
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float fX = (float)(TILECX * j) + (TILECX >> 1);
			float fY = (float)(TILECY * i) + (TILECY >> 1);

			Obj* pTile = AbstractFactory<Tile>::Create(fX, fY);
			m_vecTile.push_back(pTile);
		}
	}
}

void TileManager::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
}

void TileManager::Update()
{
	for (auto& iter : m_vecTile)
		iter->Update();
}

void TileManager::Late_Update()
{
	for (auto& iter : m_vecTile)
		iter->Late_Update();
}

void TileManager::Render(HDC hDC)
{
	int	iMoveX = abs((int)ScrollManager::Get_Instance()->Get_ScrollX() / TILECX);
	int	iMoveY = abs((int)ScrollManager::Get_Instance()->Get_ScrollY() / TILECY);

	int	iCullCX = iMoveX + (WINCX / TILECX) + 2;
	int	iCullCY = iMoveY + (WINCY / TILECY) + 2;

	for (int i = iMoveY; i < iCullCY; ++i)
	{
		for (int j = iMoveX; j < iCullCX; ++j)
		{
			int iIndex = i * TILEX + j;

			if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
				continue;

			m_vecTile[iIndex]->Render(hDC);
		}
	}
}

bool TileManager::Tile_Collision(float fX, float fY, float fFrameOffset, float* pY)
{
	if (m_vecTile.empty())
		return false;

	Tile* pTarget = nullptr;

	for (auto& iter : m_vecTile)
	{
		// Obj is in between a Tile (X Axis oriented) and has a Tile below (Y Axis oriented)
		if (fX >= iter->Get_Rect().left && fX < iter->Get_Rect().right && fY <= iter->Get_Rect().bottom)
		{
			Tile* pTile = static_cast<Tile*>(iter);

			// If Collision Tile
			if (pTile->Get_Option() == 1)
				pTarget = pTile;
		}
	}

	if (!pTarget)
		return false;

	float x1 = pTarget->Get_Rect().left;
	float x2 = pTarget->Get_Rect().right;
	float y1 = pTarget->Get_Rect().bottom - (pTarget->Get_Info().fCY / 2) - fFrameOffset;
	float y2 = pTarget->Get_Rect().bottom - (pTarget->Get_Info().fCY / 2) - fFrameOffset;

	*pY = ((y2 - y1) / (x2 - x1)) * (fX - x1) + y1;

	return true;
}

void TileManager::Pick_Tile(POINT & _pt)
{
	int	iX = _pt.x / TILECX;
	int	iY = _pt.y / TILECY;

	int	iIndex = iY * TILEX + iX;

	if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
		return;

	Tile* pTile = static_cast<Tile*>(m_vecTile[iIndex]);
	
	if (pTile->Get_DrawID() < 3)
		pTile->Set_DrawID(pTile->Get_DrawID() + 1);
	else 
		pTile->Set_DrawID(0);
		
	// DrawID 1 and 2 are Collision Tiles
	// DrawID 3 and 4 are Normal Tiles
	if (pTile->Get_DrawID() == 0 || pTile->Get_DrawID() == 1)
		pTile->Set_Option(1);
	else
		pTile->Set_Option(0);
}

void TileManager::Reset_Tile(POINT & _pt)
{
	int	iX = _pt.x / TILECX;
	int	iY = _pt.y / TILECY;

	int	iIndex = iY * TILEX + iX;

	if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
		return;

	Tile* pTile = static_cast<Tile*>(m_vecTile[iIndex]);
	pTile->Set_DrawID(-1);
	pTile->Set_Option(0);
}

void TileManager::Save_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Tile.dat", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return;

	int iDrawID, iOption = 0;
	DWORD dwByte = 0;

	for (auto& iter : m_vecTile)
	{
		iDrawID = static_cast<Tile*>(iter)->Get_DrawID();
		iOption = static_cast<Tile*>(iter)->Get_Option();

		WriteFile(hFile, &(iter->Get_Info()), sizeof(INFO), &dwByte, nullptr);
		WriteFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);
	}

	CloseHandle(hFile);
}

void TileManager::Load_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Tile.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return;

	INFO tInfo{};
	int	iDrawID, iOption = 0;
	DWORD dwByte = 0;

	Release();

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
		ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);

		if (dwByte == 0)
			break;

		Obj* pTile = AbstractFactory<Tile>::Create(tInfo.fX, tInfo.fY);
		static_cast<Tile*>(pTile)->Set_DrawID(iDrawID);
		static_cast<Tile*>(pTile)->Set_Option(iOption);

		m_vecTile.push_back(pTile);
	}
	
	CloseHandle(hFile);
}