#include "stdafx.h"
#include "Chat.h"
#include "BmpManager.h"
#include "Define.h"
#include "Function.h"

Chat::Chat() : m_pText(L""), m_pSilhouetteKey(L""), m_eSilhouetteDir(DIR_END), m_pRewardItem(nullptr), m_bDead(false)
{
	ZeroMemory(&m_tChatInfo, sizeof(m_tChatInfo));
	ZeroMemory(&m_tRectChat, sizeof(m_tRectChat));
	ZeroMemory(&m_tSilhouetteInfo, sizeof(m_tSilhouetteInfo));
	ZeroMemory(&m_tRectSilhouette, sizeof(m_tRectSilhouette));
}

Chat::Chat(TCHAR* pText, TCHAR * pSilhouetteKey, DIRID eSilhouetteDir, Item * pRewardItem) 
	: m_pText(pText), m_pSilhouetteKey(pSilhouetteKey), m_eSilhouetteDir(eSilhouetteDir), m_pRewardItem(pRewardItem)
{
	// Chat Size
	m_tChatInfo.fCX = 800.f;
	m_tChatInfo.fCY = 150.f;
	// Chat Rect
	m_tRectChat.left = 0;
	m_tRectChat.top = WINCY - m_tChatInfo.fCY;
	m_tRectChat.right = WINCX;
	m_tRectChat.bottom = WINCY;

	// Silhouette Size
	m_tSilhouetteInfo.fCX = 130.f;
	m_tSilhouetteInfo.fCY = 96.f;
	// Silhouete Rect
	m_tRectSilhouette.left = m_eSilhouetteDir == DIR_LEFT ? 30 : WINCX - m_tSilhouetteInfo.fCX - 30;
	m_tRectSilhouette.top = WINCY - m_tChatInfo.fCY - m_tSilhouetteInfo.fCY + 2;
	m_tRectSilhouette.right = m_eSilhouetteDir == DIR_LEFT ? m_tSilhouetteInfo.fCX + 30 : 30;
	m_tRectSilhouette.bottom = WINCY - m_tChatInfo.fCY + 2;	
}

Chat::~Chat()
{
	Release();
}

void Chat::Release()
{
}

int Chat::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void Chat::Late_Update()
{
}

void Chat::Render(HDC hDC)
{
	HDC hChatDC = BmpManager::Get_Instance()->Find_Bmp(L"Chat_Box");
	HDC	hSilhouetteDC = BmpManager::Get_Instance()->Find_Bmp(m_pSilhouetteKey);
	HDC hTextDC = BmpManager::Get_Instance()->Find_Bmp(m_pText);

	// Test Chat Rectangle
	//Rectangle(hDC, m_tRectChat.left, m_tRectChat.top, m_tRectChat.right, m_tRectChat.bottom);
	// Test Silhouette Rectangle
	//Rectangle(hDC, m_tRectSilhouette.left, m_tRectSilhouette.top, m_tRectSilhouette.right, m_tRectSilhouette.bottom);

	// Chat
	GdiTransparentBlt(
		hDC, m_tRectChat.left, m_tRectChat.top, m_tChatInfo.fCX, m_tChatInfo.fCY,
		hChatDC, 0, 0, m_tChatInfo.fCX, 180, RGB(132, 0, 132));
	// Silhouette
	GdiTransparentBlt(
		hDC, m_tRectSilhouette.left, m_tRectSilhouette.top, m_tSilhouetteInfo.fCX, m_tSilhouetteInfo.fCY,
		hSilhouetteDC, 0, 0, m_tSilhouetteInfo.fCX, m_tSilhouetteInfo.fCY, RGB(132, 0, 132));
	// Text
	GdiTransparentBlt(
		hDC, m_tRectChat.left, m_tRectChat.top, m_tChatInfo.fCX, m_tChatInfo.fCY,
		hTextDC, 0, 0, 400, 90, RGB(132, 0, 132));
}