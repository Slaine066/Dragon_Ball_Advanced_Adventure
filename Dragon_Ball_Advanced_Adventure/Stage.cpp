#include "stdafx.h"
#include "Stage.h"
#include "ObjManager.h"
#include "AbstractFactory.h"
#include "Player.h"
#include "BmpManager.h"
#include "ScrollManager.h"
#include "TileManager.h"
#include "SoundManager.h"
#include "UIManager.h"
#include "PigWarrior.h"
#include "PigGunner.h"
#include "BearThief.h"
#include "HealthBar.h"
#include "EnergyBar.h"
#include "ComboCounter.h"

extern float g_fSound;

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
	
	// Player
	ObjManager::Get_Instance()->Add_Object(OBJ_PLAYER, AbstractFactory<Player>::Create(100, 0));

	// UI
	UIManager::Get_Instance()->Add_Object(UI_HEALTH_BAR, AbstractFactory<HealthBar>::Create(150, 43));
	UIManager::Get_Instance()->Add_Object(UI_ENERGY_BAR, AbstractFactory<EnergyBar>::Create(130, 560));
	UIManager::Get_Instance()->Add_Object(UI_COMBO_COUNTER, AbstractFactory<ComboCounter>::Create(700, 200));
	UIManager::Get_Instance()->Initialize();

	SoundManager::Get_Instance()->PlaySound(L"Stage.mp3", CHANNEL_BGM, g_fSound);
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

	//TODO: If End Stage reached: Go To Boss Stage
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