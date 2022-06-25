#include "stdafx.h"
#include "Menu.h"
#include "BmpManager.h"
#include "KeyManager.h"
#include "SceneManager.h"

Menu::Menu()
{
}

Menu::~Menu()
{
	Release();
}

void Menu::Initialize()
{
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Menu/Menu.bmp", L"Menu");
}

void Menu::Release()
{
}

int Menu::Update()
{
	return 0;
}

void Menu::Late_Update()
{
	// TODO: Should be on Button pressed (NOT on Key pressed)
	if (KeyManager::Get_Instance()->Key_Down(VK_RETURN))
		SceneManager::Get_Instance()->Change_Scene(SCENE_STAGE1_1);
}

void Menu::Render(HDC hDC)
{
	HDC	hMenuDC = BmpManager::Get_Instance()->Find_Bmp(L"Menu");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hMenuDC, 0, 0, SRCCOPY);
}