#include "stdafx.h"
#include "Stage.h"
#include "ObjManager.h"
#include "AbstractFactory.h"
#include "Player.h"
#include "BmpManager.h"
#include "ScrollManager.h"
#include "TileManager.h"
#include "UIManager.h"

// Test Includes
#include "PigWarrior.h"
#include "PigGunner.h"
#include "BearThief.h"

Stage::Stage()
{
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Background.bmp", L"Background");

	TileManager::Get_Instance()->Load_Tile();
	UIManager::Get_Instance()->Initialize();
	
	// Test Player
	ObjManager::Get_Instance()->Add_Object(OBJ_PLAYER, AbstractFactory<Player>::Create(100, 0));

	// Test Enemy Warrior
	//ObjManager::Get_Instance()->Add_Object(OBJ_ENEMY, AbstractFactory<PigWarrior>::Create(400, 0));

	// Test Enemy Gunner
	//ObjManager::Get_Instance()->Add_Object(OBJ_ENEMY, AbstractFactory<PigGunner>::Create(600, 0));

	// Test Enemy Bear Thief (Boss)
	ObjManager::Get_Instance()->Add_Object(OBJ_ENEMY, AbstractFactory<BearThief>::Create(600, 0));
}

void Stage::Release()
{
	TileManager::Get_Instance()->Destroy_Instance();
	UIManager::Get_Instance()->Destroy_Instance();
}

int Stage::Update()
{
	TileManager::Get_Instance()->Update();
	ObjManager::Get_Instance()->Update();
	UIManager::Get_Instance()->Update();

	return 0;
}

void Stage::Late_Update()
{
	TileManager::Get_Instance()->Late_Update();
	ObjManager::Get_Instance()->Late_Update();
	UIManager::Get_Instance()->Late_Update();
}

void Stage::Render(HDC hDC)
{
	// TODO: Fix Image Resolution
	HDC	hGroundDC = BmpManager::Get_Instance()->Find_Bmp(L"Background");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hGroundDC, 0, 0, SRCCOPY);

	TileManager::Get_Instance()->Render(hDC);
	ObjManager::Get_Instance()->Render(hDC);
	UIManager::Get_Instance()->Render(hDC);
}