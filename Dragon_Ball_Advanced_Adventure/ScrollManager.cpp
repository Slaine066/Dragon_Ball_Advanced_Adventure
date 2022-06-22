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
	// Lock X Left
	if (m_fScrollX > 0.f)
		m_fScrollX = 0.f;
	// Lock X Right
	if (m_fScrollX < -3800.f)
		m_fScrollX = -3800.f;

	// Lock Y Top
	if (m_fScrollY > 0.f)
		m_fScrollY = 0.f;
	// Lock Y Bottom
	if (m_fScrollY < -150.f)
		m_fScrollY = -150.f;
}