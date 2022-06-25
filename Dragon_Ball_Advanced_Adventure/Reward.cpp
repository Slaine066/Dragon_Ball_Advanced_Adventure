#include "stdafx.h"
#include "Reward.h"
#include "KeyManager.h"
#include "BmpManager.h"
#include "ObjManager.h"
#include "Character.h"
#include "Item.h"

Reward::Reward() : m_pRewardItem(nullptr)
{
}

Reward::~Reward()
{
}

void Reward::Initialize()
{
	// Bullet Rect Size
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;

	// Sprite REAL Size
	m_tFrameInfo.fCX = 30.f;
	m_tFrameInfo.fCY = 30.f;

	// Sprite RENDER Size
	m_tFrameInfoRender.fCX = m_tFrameInfo.fCX * 2;
	m_tFrameInfoRender.fCY = m_tFrameInfo.fCY * 2;

	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Item/Reward.bmp", L"Reward");
}

void Reward::Release()
{
}

int Reward::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void Reward::Late_Update()
{
	// Get Reward Item
	if (KeyManager::Get_Instance()->Key_Down('E'))
	{
		if (!ObjManager::Get_Instance()->Get_Player().empty())
		{
		Character* pCharacter = static_cast<Character*>(ObjManager::Get_Instance()->Get_Player().front());
		m_pRewardItem->Item_Effect(pCharacter);
		}

		m_bDead = true;
	}
}

void Reward::Render(HDC hDC)
{
	// Reward
	HDC	hRewardDC = BmpManager::Get_Instance()->Find_Bmp(L"Reward");

	float fRectFrameDiffX = (m_tFrameInfoRender.fCX - m_tInfo.fCX) / 2;
	float fRectFrameDiffY = (m_tFrameInfoRender.fCY - m_tInfo.fCY) / 2;

	GdiTransparentBlt(
		hDC, (WINCX / 2) - (m_tFrameInfoRender.fCX / 2), (WINCY / 2) - (m_tFrameInfoRender.fCY / 2), m_tFrameInfoRender.fCX, m_tFrameInfoRender.fCY,
		hRewardDC, 0, 0, m_tFrameInfo.fCX, m_tFrameInfo.fCY, RGB(89, 5, 167));

	// Item
	HDC hItemDC = BmpManager::Get_Instance()->Find_Bmp(m_pRewardItem->Get_FrameKey());

	GdiTransparentBlt(
		hDC, (WINCX / 2) - (m_pRewardItem->Get_FrameInfoRender().fCX / 2), (WINCY / 2) - (m_pRewardItem->Get_FrameInfoRender().fCY / 2), m_pRewardItem->Get_FrameInfoRender().fCX, m_pRewardItem->Get_FrameInfoRender().fCY,
		hItemDC, 0, 0, m_pRewardItem->Get_FrameInfo().fCX, m_pRewardItem->Get_FrameInfo().fCY, RGB(89, 5, 167));
}