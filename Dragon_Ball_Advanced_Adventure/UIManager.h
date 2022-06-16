#pragma once

#include "Obj.h"
#include "Enum.h"

class UIManager
{
private:
	UIManager();
	~UIManager();

	static UIManager* m_pInstance;
	list<Obj*> m_pUIList[UI_END];

public:
	void Initialize();
	void Release();

	// Functions which run every frame
	void Update();
	void Late_Update();
	void Render(HDC hDC);

	void Add_Object(UIID eID, Obj * pObj);
	void Delete_Objects(UIID eID);

	static UIManager* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new UIManager;

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