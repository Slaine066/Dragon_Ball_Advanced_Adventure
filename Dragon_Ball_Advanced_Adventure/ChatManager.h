#pragma once

#include "Chat.h"

class ChatManager
{
private:
	ChatManager();
	~ChatManager();

	static ChatManager* m_pInstance;
	list<Chat*> m_ChatList;

public:
	void Add_Chat(list<Chat*> pChats);
	Chat* Get_Chat(); // Returns the first available Chat
	void Read_Chat();

	void Release();

	// Functions which run every frame
	int	Update();
	void Update_Editor();
	void Late_Update();
	void Render(HDC hDC);

	static ChatManager* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new ChatManager();

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