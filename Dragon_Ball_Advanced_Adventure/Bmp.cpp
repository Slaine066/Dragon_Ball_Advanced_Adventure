#include "stdafx.h"
#include "Bmp.h"
#include "Define.h"

Bmp::Bmp()
{
}

Bmp::~Bmp()
{
	Release();
}

void Bmp::Load_Bmp(const TCHAR * pFilePath)
{
	HDC	hDC = GetDC(g_hWnd);
	m_hMemDC = CreateCompatibleDC(hDC);

	ReleaseDC(g_hWnd, hDC);

	m_hBitMap = (HBITMAP)LoadImage(NULL, pFilePath,	IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	m_hOldMap = (HBITMAP)SelectObject(m_hMemDC, m_hBitMap);
}

void Bmp::Release(void)
{
	SelectObject(m_hMemDC, m_hOldMap);
	DeleteObject(m_hBitMap);
	DeleteDC(m_hMemDC);
}