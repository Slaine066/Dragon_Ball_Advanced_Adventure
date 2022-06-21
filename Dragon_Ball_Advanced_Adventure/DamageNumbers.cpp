#include "stdafx.h"
#include "DamageNumbers.h"
#include "ScrollManager.h"
#include "BmpManager.h"

DamageNumbers::DamageNumbers() : m_iNumber(0), m_dwTime(GetTickCount())
{
}

DamageNumbers::~DamageNumbers()
{
}

void DamageNumbers::Initialize()
{
	//Sprite REAL Size
	m_tFrameInfo.fCX = 60.f;
	m_tFrameInfo.fCY = 60.f;
	// Sprite RENDER Size
	m_tFrameInfoRender.fCX = 22.f;
	m_tFrameInfoRender.fCY = 22.f;

	m_pFrameKey = L"Combo_Numbers";
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Player/Combo_Numbers.bmp", m_pFrameKey);

	// Frame
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 9;
	m_tFrame.iMotion = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();

	m_fSpeed = 1.5f;
}

void DamageNumbers::Release()
{
}

int DamageNumbers::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fY -= m_fSpeed;
	
	Update_Rect();

	return OBJ_NOEVENT;
}

void DamageNumbers::Late_Update()
{
	if (GetTickCount() > m_dwTime + 1000)
		m_bDead = true;
}

void DamageNumbers::Render(HDC hDC)
{
	if (m_iNumber != 0)
	{
		HDC	hNumberDC = BmpManager::Get_Instance()->Find_Bmp(m_pFrameKey);

		int iScrollX = (int)ScrollManager::Get_Instance()->Get_ScrollX();
		int iScrollY = (int)ScrollManager::Get_Instance()->Get_ScrollY();

		for (int i = 0; i < m_Numbers.size(); i++)
		{
			float fRectFrameDiffX = (m_tFrameInfoRender.fCX - m_tInfo.fCX) / 2;
			float fRectFrameDiffY = (m_tFrameInfoRender.fCY - m_tInfo.fCY) / 2;

			GdiTransparentBlt(hDC, m_tRect.left - fRectFrameDiffX - (i * m_tFrameInfoRender.fCX / 1.5) + iScrollX, m_tRect.top - fRectFrameDiffY + iScrollY, m_tFrameInfoRender.fCX, m_tFrameInfoRender.fCY,
				hNumberDC, m_Numbers[i] * m_tFrameInfo.fCY, 0, m_tFrameInfo.fCX, m_tFrameInfo.fCY, RGB(132, 0, 132));
		}
	}
}

void DamageNumbers::Get_Digit(int iNumber)
{
	if (iNumber >= 10)
		Get_Digit(iNumber / 10);

	int iDigit = iNumber % 10;
	m_Numbers.push_front(iDigit);
}