#include "stdafx.h"
#include "ChatManager.h"
#include "Function.h"
#include "Define.h"
#include "BmpManager.h"
#include "ObjManager.h"

ChatManager* ChatManager::m_pInstance = nullptr;

ChatManager::ChatManager()
{
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Font.bmp", L"Font");
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Chat_Box.bmp", L"Chat_Box");
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Goku_Silhouette.bmp", L"Goku_Silhouette");
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Roshi_Silhouette.bmp", L"Roshi_Silhouette");
}

ChatManager::~ChatManager()
{
	Release();
}

void ChatManager::Add_Chat(list<Chat*> pChats)
{
	for (auto& pChat : pChats)
		m_ChatList.push_back(pChat);
}

Chat* ChatManager::Get_Chat()
{
	if (m_ChatList.empty())
		return nullptr;

	return m_ChatList.front();
}

void ChatManager::Read_Chat()
{
	m_ChatList.front()->Set_Dead();
}

void ChatManager::Release()
{
	for (auto& pChat : m_ChatList)
		Safe_Delete(pChat);

	m_ChatList.clear();
}

int ChatManager::Update()
{
	if (m_ChatList.empty())
		return 0;

	int	iEvent = 0;
	iEvent = m_ChatList.front()->Update();
	if (iEvent == OBJ_DEAD)
	{
		// If Chat* has Reward Item
		if (m_ChatList.front()->Get_RewardItem())
		{
			// Reward
			Obj* pObj = AbstractFactory<Reward>::Create();
			UIManager::Get_Instance()->Add_Object(UI_REWARD, pObj);
			Reward* pReward = static_cast<Reward*>(pObj);
			pReward->Set_RewardItem(m_ChatList.front()->Get_RewardItem());
			SoundManager::Get_Instance()->PlaySound(L"Reward.wav", CHANNEL_EFFECT, .5);
		}

		Safe_Delete(m_ChatList.front());
		m_ChatList.pop_front();
	}

	return iEvent;
}

void ChatManager::Update_Editor()
{
}

void ChatManager::Late_Update()
{
	if (m_ChatList.empty())
		return;

	m_ChatList.front()->Late_Update();
}

void ChatManager::Render(HDC hDC)
{
	if (m_ChatList.empty())
		return;

	m_ChatList.front()->Render(hDC);
}