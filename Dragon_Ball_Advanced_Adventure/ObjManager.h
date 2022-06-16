#pragma once

#include "stdafx.h"
#include "Enum.h"
#include "Obj.h"

class ObjManager
{
private:
	ObjManager();
	~ObjManager();

	static ObjManager* m_pInstance;
	list<Obj*> m_ObjList[OBJ_END];

public:
	void Add_Object(OBJID eID, Obj* pObj);
	void Delete_Objects(OBJID eID);

	list<Obj*> Get_Player() { return m_ObjList[OBJ_PLAYER]; }
	list<Obj*> Get_Enemies() { return m_ObjList[OBJ_ENEMY]; }
	list<Obj*> Get_Projectiles() { return m_ObjList[OBJ_PROJECTILE]; };
	Obj* Get_Target(OBJID eID, Obj* pObj);

	void Release();

	// Functions which run every frame
	int	Update();
	void Late_Update();
	void Render(HDC hDC);

	static ObjManager* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new ObjManager();

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
};