#include "stdafx.h"
#include "Editor.h"
#include "TileManager.h"
#include "KeyManager.h"
#include "ScrollManager.h"
#include "BmpManager.h"
#include "ObjManager.h"

Editor::Editor()
{
}

Editor::~Editor()
{
	Release();
}

void Editor::Initialize()
{
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Background.bmp", L"Background");

	TileManager::Get_Instance()->Set_EditorId(STAGE1_1); // Change this parameter to Edit Stages
	TileManager::Get_Instance()->Initialize();
}

void Editor::Release()
{
	TileManager::Get_Instance()->Destroy_Instance();
}

int Editor::Update()
{
	TileManager::Get_Instance()->Update();
	ObjManager::Get_Instance()->Update_Editor();

	Key_Input();

	return 0;
}

void Editor::Late_Update()
{
	TileManager::Get_Instance()->Late_Update();
}

void Editor::Render(HDC hDC)
{
	HDC	hGroundDC = BmpManager::Get_Instance()->Find_Bmp(L"Background");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hGroundDC, 0, 0, SRCCOPY);

	TileManager::Get_Instance()->Render(hDC);
	ObjManager::Get_Instance()->Render(hDC);
}

void Editor::Key_Input()
{
	if (KeyManager::Get_Instance()->Key_Pressing(VK_LEFT))
		ScrollManager::Get_Instance()->Set_ScrollX(5.f);

	if (KeyManager::Get_Instance()->Key_Pressing(VK_RIGHT))
		ScrollManager::Get_Instance()->Set_ScrollX(-5.f);

	if (KeyManager::Get_Instance()->Key_Pressing(VK_UP))
		ScrollManager::Get_Instance()->Set_ScrollY(5.f);

	if (KeyManager::Get_Instance()->Key_Pressing(VK_DOWN))
		ScrollManager::Get_Instance()->Set_ScrollY(-5.f);

	if (KeyManager::Get_Instance()->Key_Down(VK_LBUTTON))
	{	
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= (int)ScrollManager::Get_Instance()->Get_ScrollX();
		pt.y -= (int)ScrollManager::Get_Instance()->Get_ScrollY();

		TileManager::Get_Instance()->Pick_Tile(pt);
	}

	if (KeyManager::Get_Instance()->Key_Down(VK_RBUTTON))
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= (int)ScrollManager::Get_Instance()->Get_ScrollX();
		pt.y -= (int)ScrollManager::Get_Instance()->Get_ScrollY();

		TileManager::Get_Instance()->Reset_Tile(pt);
	}

	if (KeyManager::Get_Instance()->Key_Down('1'))
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= (int)ScrollManager::Get_Instance()->Get_ScrollX();
		pt.y -= (int)ScrollManager::Get_Instance()->Get_ScrollY();

		TileManager::Get_Instance()->Pick_Enemy(pt, 1);
	}
	if (KeyManager::Get_Instance()->Key_Down('2'))
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= (int)ScrollManager::Get_Instance()->Get_ScrollX();
		pt.y -= (int)ScrollManager::Get_Instance()->Get_ScrollY();

		TileManager::Get_Instance()->Pick_Enemy(pt, 2);
	}
	if (KeyManager::Get_Instance()->Key_Down('3'))
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= (int)ScrollManager::Get_Instance()->Get_ScrollX();
		pt.y -= (int)ScrollManager::Get_Instance()->Get_ScrollY();

		TileManager::Get_Instance()->Pick_Enemy(pt, 3);
	}

	// Save
	if (KeyManager::Get_Instance()->Key_Down('S'))
		TileManager::Get_Instance()->Save_Tile();

	// Load
	if (KeyManager::Get_Instance()->Key_Down('L'))
		TileManager::Get_Instance()->Load_Tile();
}