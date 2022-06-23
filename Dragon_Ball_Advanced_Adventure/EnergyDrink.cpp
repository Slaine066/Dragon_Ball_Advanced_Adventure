#include "stdafx.h"
#include "EnergyDrink.h"
#include "BmpManager.h"
#include "ScrollManager.h"

EnergyDrink::EnergyDrink() : m_iIncreaseAmount(50)
{
}

EnergyDrink::~EnergyDrink()
{
}

void EnergyDrink::Initialize()
{
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;

	m_tFrameInfo.fCX = m_tInfo.fCX;
	m_tFrameInfo.fCY = m_tInfo.fCY;

	m_tFrameInfoRender.fCX = m_tFrameInfo.fCX * 1.5;
	m_tFrameInfoRender.fCY = m_tFrameInfo.fCY * 1.5;

	// Frame
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iMotion = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();

	m_pFrameKey = L"Energy_Drink";
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Item/Energy_Drink.bmp", m_pFrameKey);
}

void EnergyDrink::Release()
{
}

int EnergyDrink::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Gravity();
	Update_Rect();

	return OBJ_NOEVENT;
}

void EnergyDrink::Late_Update()
{
}

void EnergyDrink::Render(HDC hDC)
{
	int iScrollX = (int)ScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)ScrollManager::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = BmpManager::Get_Instance()->Find_Bmp(m_pFrameKey);

	// Test Enemy Rectangle
	//Rectangle(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

	float fRectFrameDiffX = (m_tFrameInfoRender.fCX - m_tInfo.fCX) / 2;
	float fRectFrameDiffY = (m_tFrameInfoRender.fCY - m_tInfo.fCY) / 2;

	GdiTransparentBlt(
		hDC, m_tRect.left - fRectFrameDiffX + iScrollX, m_tRect.top - fRectFrameDiffY + iScrollY, m_tFrameInfoRender.fCX, m_tFrameInfoRender.fCY,
		hMemDC, m_tFrame.iFrameStart * m_tFrameInfo.fCX, m_tFrame.iMotion * m_tFrameInfo.fCY, m_tFrameInfo.fCX, m_tFrameInfo.fCY, RGB(89, 5, 167));
}

void EnergyDrink::Item_Effect(Character * pPlayer)
{
	pPlayer->Increase_Energy(m_iIncreaseAmount);
	SoundManager::Get_Instance()->PlaySound(L"Goku_Joy.wav", CHANNEL_VOICE, .5f);
}