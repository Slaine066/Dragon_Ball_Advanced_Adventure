#pragma once

class Bmp
{
public:
	Bmp();
	~Bmp();

	HDC Get_MemDC() { return m_hMemDC; }
	void Load_Bmp(const TCHAR* pFilePath);

	void Release();

private:
	HDC	m_hMemDC;
	HBITMAP m_hBitMap;
	HBITMAP	m_hOldMap;
};