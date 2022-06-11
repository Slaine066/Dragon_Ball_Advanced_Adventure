#include "stdafx.h"
#include "BmpManager.h"
#include "Function.h"

BmpManager* BmpManager::m_pInstance = nullptr;

BmpManager::BmpManager()
{
}

BmpManager::~BmpManager()
{
	Release();
}

void BmpManager::Insert_Bmp(const TCHAR * pFilePath, const TCHAR * pImageKey)
{
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), CTag_Finder(pImageKey));

	if (iter == m_mapBit.end())
	{
		Bmp* pBmp = new Bmp;
		pBmp->Load_Bmp(pFilePath);

		m_mapBit.emplace(pImageKey, pBmp);
	}
}

HDC BmpManager::Find_Bmp(const TCHAR* pImageKey)
{
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), CTag_Finder(pImageKey));

	if (iter == m_mapBit.end())
		return nullptr;

	return iter->second->Get_MemDC();
}

void BmpManager::Release()
{
	for_each(m_mapBit.begin(), m_mapBit.end(), CDeleteMap());
	m_mapBit.clear();
}