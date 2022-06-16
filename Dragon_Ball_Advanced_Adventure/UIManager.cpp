#include "stdafx.h"
#include "UIManager.h"
#include "Function.h"

UIManager* UIManager::m_pInstance = nullptr;

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
	Release();
}

void UIManager::Initialize()
{
	for (size_t i = 0; i < UI_END; ++i)
	{
		for (auto& iter : m_pUIList[i])
			iter->Initialize();
	}
}

void UIManager::Release()
{
	for (size_t i = 0; i < UI_END; ++i)
	{
		for (auto& iter : m_pUIList[i])
			Safe_Delete(iter);

		m_pUIList[i].clear();
	}
}

void UIManager::Update()
{	
	int	iEvent = 0;
	for (size_t i = 0; i < UI_END; ++i)
	{
		for (auto& iter = m_pUIList[i].begin(); iter != m_pUIList[i].end(); )
		{
			iEvent = (*iter)->Update();

			if (iEvent == OBJ_DEAD)
			{
				Safe_Delete(*iter);
				iter = m_pUIList[i].erase(iter);
			}
			else
				++iter;
		}
	}
}

void UIManager::Late_Update()
{
	for (size_t i = 0; i < UI_END; ++i)
	{
		for (auto& iter : m_pUIList[i])
		{
			iter->Late_Update();

			if (m_pUIList[i].empty())
				break;
		}
	}
}

void UIManager::Render(HDC hDC)
{
	for (size_t i = 0; i < UI_END; ++i)
	{
		for (auto& iter : m_pUIList[i])
			iter->Render(hDC);
	}
}

void UIManager::Add_Object(UIID eID, Obj * pObj)
{
	if (pObj == nullptr || eID >= UI_END)
		return;

	m_pUIList[eID].push_back(pObj);
}

void UIManager::Delete_Objects(UIID eID)
{
	for (auto& iter : m_pUIList[eID])
		Safe_Delete(iter);

	m_pUIList[eID].clear();
}