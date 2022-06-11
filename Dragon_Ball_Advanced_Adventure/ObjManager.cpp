#include "stdafx.h"
#include "ObjManager.h"
#include "Function.h"
#include "Define.h"

ObjManager* ObjManager::m_pInstance = nullptr;

ObjManager::ObjManager()
{
}

ObjManager::~ObjManager()
{
	Release();
}

void ObjManager::Add_Object(OBJID eID, Obj * pObj)
{
	if (pObj == nullptr || eID >= OBJ_END)
		return;

	m_ObjList[eID].push_back(pObj);
}

void ObjManager::Delete_Objects(OBJID eID)
{
	for (auto& iter : m_ObjList[eID])
		Safe_Delete(iter);

	m_ObjList[eID].clear();
}

Obj* ObjManager::Get_Target(OBJID eID, Obj * pObj)
{
	return nullptr;
}

void ObjManager::Release()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter : m_ObjList[i])
			Safe_Delete(iter);

		m_ObjList[i].clear();
	}
}

int ObjManager::Update()
{
	int	iEvent = 0;

	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter = m_ObjList[i].begin(); iter != m_ObjList[i].end(); )
		{
			iEvent = (*iter)->Update();

			if (iEvent == OBJ_DEAD)
			{
				Safe_Delete(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}

	return iEvent;
}

void ObjManager::Late_Update()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter : m_ObjList[i])
			iter->Late_Update();
	}

	// TODO: Add Collision Methods here.
}

void ObjManager::Render(HDC hDC)
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter : m_ObjList[i])
			iter->Render(hDC);
	}
}