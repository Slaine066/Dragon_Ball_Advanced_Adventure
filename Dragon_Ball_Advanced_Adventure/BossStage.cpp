#include "stdafx.h"
#include "BossStage.h"
#include "BmpManager.h"
#include "TileManager.h"
#include "UIManager.h"
#include "AbstractFactory.h"
#include "HealthBar.h"
#include "EnergyBar.h"
#include "ComboCounter.h"
#include "ObjManager.h"
#include "SceneManager.h"
#include "ScrollManager.h"
#include "BearThief.h"
#include "BossHealthBar.h"

BossStage::BossStage() : m_bStageClear(false)
{
}

BossStage::~BossStage()
{
	Release();
}

void BossStage::Initialize()
{
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Background.bmp", L"Background");

	TileManager::Get_Instance()->Set_BossStage();
	TileManager::Get_Instance()->Load_Tile();

	// Player
	Player* pPlayer = SceneManager::Get_Instance()->Get_Player();
	ObjManager::Get_Instance()->Add_Object(OBJ_PLAYER, AbstractFactory<Player>::Create(0, 0));

	Character* pCharacter = dynamic_cast<Character*>(ObjManager::Get_Instance()->Get_Player().front());
	if (pCharacter && pPlayer)
		pCharacter->Set_Health(100 - pPlayer->Get_Stats().iHealth);

	// Boss
	ObjManager::Get_Instance()->Add_Object(OBJ_ENEMY, AbstractFactory<BearThief>::Create(500, 0));

	// UI
	UIManager::Get_Instance()->Add_Object(UI_HEALTH_BAR, AbstractFactory<HealthBar>::Create(150, 43));
	UIManager::Get_Instance()->Add_Object(UI_ENERGY_BAR, AbstractFactory<EnergyBar>::Create(130, 560));
	UIManager::Get_Instance()->Add_Object(UI_COMBO_COUNTER, AbstractFactory<ComboCounter>::Create(700, 200));
	UIManager::Get_Instance()->Add_Object(UI_HEALTH_BAR, AbstractFactory<BossHealthBar>::Create(650, 55));
	UIManager::Get_Instance()->Initialize();
}

void BossStage::Release()
{
	TileManager::Get_Instance()->Destroy_Instance();
	UIManager::Get_Instance()->Destroy_Instance();
}

int BossStage::Update()
{
	TileManager::Get_Instance()->Update();
	ObjManager::Get_Instance()->Update();
	UIManager::Get_Instance()->Update();

	return 0;
}

void BossStage::Late_Update()
{
	TileManager::Get_Instance()->Late_Update();
	ObjManager::Get_Instance()->Late_Update();
	UIManager::Get_Instance()->Late_Update();
	ScrollManager::Get_Instance()->Reset_Scroll();

	//if (m_bStageClear)
		// TODO: Show Stage Clear
}

void BossStage::Render(HDC hDC)
{
	HDC	hGroundDC = BmpManager::Get_Instance()->Find_Bmp(L"Background");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hGroundDC, 0, 0, SRCCOPY);

	TileManager::Get_Instance()->Render(hDC);
	ObjManager::Get_Instance()->Render(hDC);
	UIManager::Get_Instance()->Render(hDC);
}