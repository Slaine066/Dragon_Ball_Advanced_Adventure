#include "stdafx.h"
#include "ObjManager.h"
#include "Function.h"
#include "Define.h"
#include "CollisionManager.h"
#include "Enemy.h"
#include "AbstractFactory.h"
#include "Milk.h"

ObjManager* ObjManager::m_pInstance = nullptr;

ObjManager::ObjManager()
{
	srand(time(NULL));
}

ObjManager::~ObjManager()
{
	Release();
}

void ObjManager::Spawn_Item(Obj* pSource)
{
	// TODO: Should Check pSource for droppable Items

	if (!Get_Player().empty())
	{
		Player* pPlayer = static_cast<Player*>(Get_Player().front());

		if (pPlayer->Get_Stats().iHealth < pPlayer->Get_Stats().iHealthMax)
		{
			Obj* pItem = AbstractFactory<Milk>::Create(pSource->Get_Info().fX, pSource->Get_Info().fY);
			Item* pMilk = static_cast<Item*>(pItem);

			if ((rand() % 10 + 1) <= pMilk->Get_SpawnRate())
				Add_Object(OBJ_ITEM, pItem);
		}
	}
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
				// If OBJ_DEAD source is an Enemy
				Enemy* pEnemy = dynamic_cast<Enemy*>(*iter);
				if (pEnemy)
					Spawn_Item(pEnemy);

				Safe_Delete(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}

	return iEvent;
}

void ObjManager::Update_Editor()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter : m_ObjList[i])
			iter->Update_Editor();
	}
}

void ObjManager::Late_Update()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter : m_ObjList[i])
		{
			iter->Late_Update();

			if (m_ObjList[i].empty())
				break;
		}
	}

	// Collision Handling Methods
	CollisionManager::Collision_Damage(Get_Player(), Get_Enemies()); // Player Interects Enemies
	CollisionManager::Collision_Damage(Get_Enemies(), Get_Player()); // Enemies Interects Player
	CollisionManager::Collision_Item(Get_Player(), Get_Items()); // Player Intersects Item
	CollisionManager::Collision_Projectile(Get_Projectiles(), Get_Player()); // Projectiles Interects Player
	CollisionManager::Collision_Projectile(Get_Projectiles(), Get_Enemies()); // Projectiles Interects Enemies
}

void ObjManager::Render(HDC hDC)
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter : m_ObjList[i])
			iter->Render(hDC);
	}
}