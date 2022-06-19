#include "stdafx.h"
#include "ScrollManager.h"
#include "Define.h"

ScrollManager* ScrollManager::m_pInstance = nullptr;

ScrollManager::ScrollManager() : m_fScrollX(0.f), m_fScrollY(0.f)
{
}

ScrollManager::~ScrollManager()
{
}

void ScrollManager::Scroll_Lock()
{
	// Lock X
	if (m_fScrollX > 0.f)
		m_fScrollX = 0.f;

	//if (m_fScrollX < WINCX - 1920)
	//	m_fScrollX = WINCX - 1920;

	//// Lock Y
	//if (m_fScrollY > 0.f)
	//	m_fScrollY = 0.f;

	//if (m_fScrollY < WINCY - 1280)
	//	m_fScrollY = WINCY - 1280;
}