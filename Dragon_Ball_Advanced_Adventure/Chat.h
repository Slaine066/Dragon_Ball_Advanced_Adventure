#pragma once

#include "Item.h"

class Chat
{
public:
	Chat();
	Chat(TCHAR* m_pText, TCHAR* pSilhouetteKey, DIRID eSilhouetteDir, Item* pRewardItem = nullptr);
	~Chat();

	void Release();

	int Update();
	void Late_Update();
	void Render(HDC hDC);

	Item* Get_RewardItem() { return m_pRewardItem; }
	void Set_Dead() { m_bDead = true; }

private:
	TCHAR* m_pText;
	TCHAR* m_pSilhouetteKey;
	DIRID m_eSilhouetteDir;
	Item* m_pRewardItem;

	INFO m_tChatInfo;
	RECT m_tRectChat;
	INFO m_tSilhouetteInfo;
	RECT m_tRectSilhouette;

	bool m_bDead;
};