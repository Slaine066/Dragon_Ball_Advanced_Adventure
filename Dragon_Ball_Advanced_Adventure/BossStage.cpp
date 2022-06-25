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
#include "SoundManager.h"
#include "Turtle.h"
#include "KeyManager.h"

extern float g_fSound;

BossStage::BossStage() : m_bBossSpawned(false), m_bStageClear(false)
{
}

BossStage::~BossStage()
{
	Release();
}

void BossStage::Initialize()
{
	ScrollManager::Get_Instance()->Reset_Scroll();
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Background.bmp", L"Background");
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Stage_Clear.bmp", L"Stage_Clear");

	TileManager::Get_Instance()->Set_EditorId(STAGE_BOSS);
	TileManager::Get_Instance()->Load_Tile();

	// Bring Player over from previous Stage
	Player* pPlayer = SceneManager::Get_Instance()->Get_Player();
	ObjManager::Get_Instance()->Add_Object(OBJ_PLAYER, AbstractFactory<Player>::Create(0, 0));

	// Set previous Health
	Character* pCharacter = nullptr;
	if (pPlayer)
		pCharacter = dynamic_cast<Character*>(ObjManager::Get_Instance()->Get_Player().front());
	if (pCharacter && pPlayer)
	{
		pCharacter->Set_Health(pPlayer->Get_Stats().iHealthMax - pPlayer->Get_Stats().iHealth);
		pCharacter->Set_EnergyMax(pPlayer->Get_Stats().iEnergyMax);
		pCharacter->Refull_Energy();
	}

	// Turtle
	ObjManager::Get_Instance()->Add_Object(OBJ_NPC, AbstractFactory<Turtle>::Create(600, 0));

	// Sound
	SoundManager::Get_Instance()->PlayBGM(L"Stage_Boss.mp3", g_fSound / 2);
}

void BossStage::Release()
{
	SoundManager::Get_Instance()->StopSound(CHANNEL_BGM);
	TileManager::Get_Instance()->Destroy_Instance();
	ObjManager::Get_Instance()->Destroy_Instance();
	UIManager::Get_Instance()->Destroy_Instance();
}

int BossStage::Update()
{
	TileManager::Get_Instance()->Update();
	ObjManager::Get_Instance()->Update();
	UIManager::Get_Instance()->Update();

	if (KeyManager::Get_Instance()->Key_Down(VK_RETURN) && m_bStageClear)
		SceneManager::Get_Instance()->Change_Scene(SCENE_ENDING);
		
	return 0;
}

void BossStage::Late_Update()
{
	TileManager::Get_Instance()->Late_Update();
	ObjManager::Get_Instance()->Late_Update();
	UIManager::Get_Instance()->Late_Update();
	ScrollManager::Get_Instance()->Reset_Scroll();

	// Spawn Boss after Turtle disappears
	if (ObjManager::Get_Instance()->Get_NPCs().empty() && !m_bBossSpawned)
	{
		ObjManager::Get_Instance()->Add_Object(OBJ_ENEMY, AbstractFactory<BearThief>::Create(900, 0));
		m_bBossSpawned = true;
	}
		
	// Stage Clear when Boss dies
	if (!ObjManager::Get_Instance()->Get_Enemies().empty())
	{
		if (ObjManager::Get_Instance()->Get_Enemies().front()->Get_Dead() && !m_bStageClear)
		{
			SoundManager::Get_Instance()->StopSound(CHANNEL_BGM);
			SoundManager::Get_Instance()->PlaySound(L"Stage_Clear.mp3", CHANNEL_SYSTEM, g_fSound / 2);
			SoundManager::Get_Instance()->PlaySound(L"Goku_Clear.wav", CHANNEL_VOICE, g_fSound / 2);

			m_bStageClear = true;
		}
	}
}

void BossStage::Render(HDC hDC)
{
	HDC	hGroundDC = BmpManager::Get_Instance()->Find_Bmp(L"Background");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hGroundDC, 0, 0, SRCCOPY);

	TileManager::Get_Instance()->Render(hDC);
	ObjManager::Get_Instance()->Render(hDC);
	UIManager::Get_Instance()->Render(hDC);

	if (m_bStageClear)
	{
		HDC	hMemDC = BmpManager::Get_Instance()->Find_Bmp(L"Stage_Clear");
		GdiTransparentBlt(
			hDC, 400 - 797 / 4, 200 - 163 / 4, 797 / 2, 163 / 2,
			hMemDC, 0, 0, 797, 163, RGB(132, 0, 132));
	}
}