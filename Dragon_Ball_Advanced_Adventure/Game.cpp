#include "stdafx.h"
#include "Game.h"
#include "ObjManager.h"
#include "AbstractFactory.h"
#include "Player.h"
#include "BmpManager.h"
#include "ScrollManager.h"
#include "PigWarrior.h"
#include "TileManager.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Initialize()
{
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Background.bmp", L"Background");
	
	TileManager::Get_Instance()->Load_Tile();
	
	// Test Player
	ObjManager::Get_Instance()->Add_Object(OBJ_PLAYER, AbstractFactory<Player>::Create(200, 200));

	// Test Enemy
	//ObjManager::Get_Instance()->Add_Object(OBJ_ENEMY, AbstractFactory<PigWarrior>::Create(400, 200));
}

void Game::Release()
{
	TileManager::Get_Instance()->Destroy_Instance();
}

int Game::Update()
{
	ObjManager::Get_Instance()->Update();
	TileManager::Get_Instance()->Update();

	return 0;
}

void Game::Late_Update()
{
	ObjManager::Get_Instance()->Late_Update();
	TileManager::Get_Instance()->Late_Update();
}

void Game::Render(HDC hDC)
{
	HDC	hGroundDC = BmpManager::Get_Instance()->Find_Bmp(L"Background");

	int iScrollX = (int)ScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)ScrollManager::Get_Instance()->Get_ScrollY();

	BitBlt(hDC, 0, 0, WINCX, WINCY, hGroundDC, 0, 0, SRCCOPY);

	TileManager::Get_Instance()->Render(hDC);
	ObjManager::Get_Instance()->Render(hDC);
}