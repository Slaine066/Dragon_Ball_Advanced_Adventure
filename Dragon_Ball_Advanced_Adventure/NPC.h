#pragma once

#include "Character.h"
#include "Chat.h"
#include "Item.h"

class NPC : public Character
{
public:
	NPC() : m_iDialogRadius(100) { ZeroMemory(&m_tDialogRect, sizeof(m_tDialogRect)); };
	~NPC() {};

	RECT Get_DialogRect() { return m_tDialogRect; }
	int Get_DialogRadius() { return m_iDialogRadius; }
	list<Chat*> Get_Chats() { return m_Chats; }
	void Read_Chat() { m_Chats.pop_front(); }

protected:
	RECT m_tDialogRect;
	int m_iDialogRadius;

	list<Chat*> m_Chats;
};