#pragma once

#include "stdafx.h"
#include "Bmp.h"

class BmpManager
{
private:
	BmpManager();
	~BmpManager();

	static BmpManager* m_pInstance;
	map<const TCHAR*, Bmp*> m_mapBit;

public:
	void Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImageKey);
	HDC	Find_Bmp(const TCHAR* pImageKey);

	void Release();

	static BmpManager* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new BmpManager();

		return m_pInstance;
	}

	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
};