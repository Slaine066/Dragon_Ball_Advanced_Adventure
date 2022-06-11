#include "stdafx.h"
#include "Logo.h"
#include "BmpManager.h"
#include "KeyManager.h"
#include "SceneManager.h"

Logo::Logo()
{
}

Logo::~Logo()
{
	Release();
}

void Logo::Initialize()
{
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Logo/Logo.bmp", L"Logo");
}

void Logo::Release()
{
}

int Logo::Update()
{
	return 0;
}

void Logo::Late_Update()
{
	// TODO: Should be based on time (NOT on Key pressed)
	if (KeyManager::Get_Instance()->Key_Down(VK_RETURN))
		SceneManager::Get_Instance()->Change_Scene(SCENE_MENU);
}

void Logo::Render(HDC hDC)
{
	HDC	hLogoDC = BmpManager::Get_Instance()->Find_Bmp(L"Logo");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hLogoDC, 0, 0, SRCCOPY);
}