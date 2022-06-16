#include "stdafx.h"
#include "Kamehameha.h"
#include "ScrollManager.h"
#include "BmpManager.h"

Kamehameha::Kamehameha()
{
}

Kamehameha::~Kamehameha()
{
}

void Kamehameha::Initialize()
{
	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 40.f;
	
	m_fSpeed = 10.f;
}

void Kamehameha::Release()
{
}

int Kamehameha::Update()
{
	Update_Rect();

	return OBJ_NOEVENT;
}

void Kamehameha::Late_Update()
{
}

void Kamehameha::Render(HDC hDC)
{
	int iScrollX = (int)ScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)ScrollManager::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = BmpManager::Get_Instance()->Find_Bmp(m_pFrameKey);

	GdiTransparentBlt(
		hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tFrameInfo.fCX, m_tFrameInfo.fCY,
		hMemDC, m_tFrame.iFrameStart * m_tFrameInfo.fCX, m_tFrame.iMotion * m_tFrameInfo.fCY, m_tFrameInfo.fCX, m_tFrameInfo.fCY, RGB(132, 0, 132));
}