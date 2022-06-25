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

void ScrollManager::Scroll_Lock(SCENEID eId)
{
	// Lock X Left
	if (m_fScrollX > 0.f)
		m_fScrollX = 0.f;

	// Lock X Right
	/*if (m_fScrollX < -2200.f)
		m_fScrollX = -2200.f;*/

	switch (eId)
	{
		case SCENE_STAGE1_1:
		{
			// Lock X Right
			if (m_fScrollX < -2200.f)
				m_fScrollX = -2200.f;
			break;
		}
		case SCENE_STAGE1_2:
		{
			// Lock X Right
			if (m_fScrollX < -4700.f)
				m_fScrollX = -4700.f;
			break;
		}
	}

	// Lock Y Top
	/*if (m_fScrollY > 0.f)
		m_fScrollY = 0.f;*/
	// Lock Y Bottom
	if (m_fScrollY < -150.f)
		m_fScrollY = -150.f;
}