#include "stdafx.h"
#include "ComboCounter.h"
#include "ObjManager.h"
#include "Player.h"
#include "BmpManager.h"

ComboCounter::ComboCounter() : m_iComboCounter(0)
{
	ZeroMemory(&m_tNumberFrame, sizeof(m_tNumberFrame));
	ZeroMemory(&m_tNumberFrameRender, sizeof(m_tNumberFrameRender));
}

ComboCounter::~ComboCounter()
{
}

void ComboCounter::Initialize()
{
	// Rect Size
	/*m_tInfo.fCX = 115.f;
	m_tInfo.fCY = 45.f;*/

	// HITS
	// Sprite REAL Size
	m_tFrameInfo.fCX = 115.f;
	m_tFrameInfo.fCY = 45.f;
	// Sprite RENDER Size
	m_tFrameInfoRender.fCX = 115.f;
	m_tFrameInfoRender.fCY = 45.f;

	//NUMBERS
	//Sprite REAL Size
	m_tNumberFrame.fCX = 60.f;
	m_tNumberFrame.fCY = 60.f;
	// Sprite RENDER Size
	m_tNumberFrameRender.fCX = 60.f;
	m_tNumberFrameRender.fCY = 60.f;

	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Player/Hits.bmp", L"Hits");
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Player/Combo_Numbers.bmp", L"Combo_Numbers");

	m_pFrameKey = L"Combo_Numbers";

	// Frame (Numbers)
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 9;
	m_tFrame.iMotion = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();
}

void ComboCounter::Release()
{
}

int ComboCounter::Update()
{
	Update_Rect();

	return 0;
}

void ComboCounter::Late_Update()
{
	Player* pPlayer = nullptr;
	if (!ObjManager::Get_Instance()->Get_Player().empty())
		pPlayer = static_cast<Player*>(ObjManager::Get_Instance()->Get_Player().front());

	if (pPlayer)
	{
		m_iComboCounter = pPlayer->Get_ComboCounter();
		m_Numbers.clear();
		if (m_iComboCounter != 0)
			Get_Digit(m_iComboCounter); // Populate Vector
	}
}

void ComboCounter::Render(HDC hDC)
{
	if (m_iComboCounter > 1)
	{
		// NUMBERS
		HDC	hNumberDC = BmpManager::Get_Instance()->Find_Bmp(m_pFrameKey);

		for (int i = 0; i < m_Numbers.size(); i++)
		{
			float fRectFrameDiffX = (m_tNumberFrameRender.fCX - m_tInfo.fCX) / 2;
			float fRectFrameDiffY = (m_tNumberFrameRender.fCY - m_tInfo.fCY) / 2;

			GdiTransparentBlt(hDC, m_tRect.left - fRectFrameDiffX - 100 - (i * m_tNumberFrame.fCX), m_tRect.top - fRectFrameDiffY - 10, m_tNumberFrameRender.fCX, m_tNumberFrameRender.fCY,
				hNumberDC, m_Numbers[i] * m_tNumberFrame.fCY, 0, m_tNumberFrame.fCX, m_tNumberFrame.fCY, RGB(132, 0, 132));
		}

		// HITS
		HDC	hHitsDC = BmpManager::Get_Instance()->Find_Bmp(L"Hits");

		// Test Rectangle
		//Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

		float fRectFrameDiffX = (m_tFrameInfoRender.fCX - m_tInfo.fCX) / 2;
		float fRectFrameDiffY = (m_tFrameInfoRender.fCY - m_tInfo.fCY) / 2;

		GdiTransparentBlt(hDC, m_tRect.left - fRectFrameDiffX, m_tRect.top - fRectFrameDiffY, m_tFrameInfoRender.fCX, m_tFrameInfoRender.fCY,
			hHitsDC, 0, 0 * m_tFrameInfo.fCY, m_tFrameInfo.fCX, m_tFrameInfo.fCY, RGB(132, 0, 132));
	}
}

void ComboCounter::Change_Frame()
{	
}

void ComboCounter::Get_Digit(int iNumber)
{
	if (iNumber >= 10)
		Get_Digit(iNumber / 10);

	int iDigit = iNumber % 10;
	m_Numbers.push_front(iDigit);
}