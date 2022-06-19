#include "stdafx.h"
#include "KeyManager.h"

KeyManager* KeyManager::m_pInstance = nullptr;

KeyManager::KeyManager()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}

KeyManager::~KeyManager()
{
}

bool KeyManager::Key_Pressing(int _Key)
{
	if (GetAsyncKeyState(_Key) & 0x8000)
		return true;

	return false;
}

bool KeyManager::Key_Up(int _Key)
{
	if (m_bKeyState[_Key] && !(GetAsyncKeyState(_Key) & 0x8000))
	{
		m_bKeyState[_Key] = !m_bKeyState[_Key];
		return true;
	}

	for (int i = 0; i < VK_MAX; ++i)
	{
		if (!m_bKeyState[_Key] && (GetAsyncKeyState(_Key) & 0x8000))
			m_bKeyState[_Key] = !m_bKeyState[_Key];
	}

	return false;
}

bool KeyManager::Key_Down(int _Key)
{
	if (!m_bKeyState[_Key] && (GetAsyncKeyState(_Key) & 0x8000))
	{
		m_bKeyState[_Key] = !m_bKeyState[_Key];
		return true;
	}

	for (int i = 0; i < VK_MAX; ++i)
	{
		if (m_bKeyState[_Key] && !(GetAsyncKeyState(_Key) & 0x8000))
			m_bKeyState[_Key] = !m_bKeyState[_Key];
	}

	return false;
}