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

bool TileManager::Tile_Collision(float fX, float fY, float fFrameOffset, float* pTargetY)
{
	if (m_vecTile.empty())
		return false;

	Tile* pTarget = nullptr;

	for (auto& iter : m_vecTile)
	{
		// Obj is in between a Tile (X Axis oriented) and has a Tile below (Y Axis oriented)
		if (fX >= iter->Get_Rect().left && fX <= iter->Get_Rect().right && fY < iter->Get_Rect().bottom)
		{
			Tile* pTile = static_cast<Tile*>(iter);

			// If Collision Tile
			if (pTile->Get_Option() == 1)
			{
				if (pTarget)
				{
					if (pTile->Get_Rect().bottom < pTarget->Get_Rect().bottom)
						pTarget = pTile;
				}
				else
					pTarget = pTile;
			}
		}
	}

	if (!pTarget)
		return false;

	float x1 = pTarget->Get_Rect().left;
	float x2 = pTarget->Get_Rect().right;
	float y1 = pTarget->Get_Rect().bottom;
	float y2 = pTarget->Get_Rect().bottom;

	float fTargetY = ((y2 - y1) / (x2 - x1)) * (fX - x1) + y1;
	*pTargetY = fTargetY - (pTarget->Get_Info().fCY / 2) - fFrameOffset;

	return true;
}

bool TileManager::Wall_Collision(Obj* pObj)
{
	if (m_vecTile.empty())
		return false;

	RECT Rect{};

	for (auto& iter : m_vecTile)
	{
		Tile* pTile = static_cast<Tile*>(iter);

		if (pTile->Get_Option() != 1)
		{
			if (pObj->Get_Rect().bottom > iter->Get_Rect().top && pObj->Get_Rect().bottom < iter->Get_Rect().bottom)
			{
				if (IntersectRect(&Rect, &pObj->Get_Rect(), &iter->Get_Rect()))
					return true;
			}
		}
	}

	return false;
}

void TileManager::Pick_Tile(POINT & _pt)
{
	int	iX = _pt.x / TILECX;
	int	iY = _pt.y / TILECY;

	int	iIndex = iY * TILEX + iX;

	if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
		return;

	Tile* pTile = static_cast<Tile*>(m_vecTile[iIndex]);
	
	//if (pTile->Get_DrawID() < 5)	// Tile
	if (pTile->Get_DrawID() < 17)	// Boss Tile
		pTile->Set_DrawID(pTile->Get_DrawID() + 1);
	else 
		pTile->Set_DrawID(0);
		
	// DrawID 0, 1, 2 and 3 are Collision Tiles
	// DrawID 4 and 5 are Normal Tiles
	//if (pTile->Get_DrawID() == 0 || pTile->Get_DrawID() == 1 || pTile->Get_DrawID() == 2 || pTile->Get_DrawID() == 3) // Tile
	if (pTile->Get_DrawID() == 0 || pTile->Get_DrawID() == 1 || pTile->Get_DrawID() == 2 || pTile->Get_DrawID() == 3 || pTile->Get_DrawID() == 4) // Boss Tile
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
	//HANDLE hFile = CreateFile(L"../Data/Tile.dat", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hFile = CreateFile(L"../Data/Boss_Tile.dat", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

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
	//HANDLE hFile = CreateFile(L"../Data/Tile.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hFile = CreateFile(L"../Data/Boss_Tile.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

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