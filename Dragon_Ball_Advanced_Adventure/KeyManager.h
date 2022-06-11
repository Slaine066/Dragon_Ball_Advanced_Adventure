#pragma once

#include "Define.h"

class KeyManager
{
private:
	KeyManager();
	~KeyManager();

	static KeyManager* m_pInstance;
	bool m_bKeyState[VK_MAX];

public:
	bool Key_Pressing(int _Key);
	bool Key_Up(int _Key);
	bool Key_Down(int _Key);

	static KeyManager* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new KeyManager();

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (nullptr != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
};