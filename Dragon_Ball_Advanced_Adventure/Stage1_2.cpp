#include "stdafx.h"
#include "Stage1_2.h"
#include "BmpManager.h"
#include "TileManager.h"
#include "Enum.h"
#include "SceneManager.h"
#include "ObjManager.h"
#include "AbstractFactory.h"
#include "SoundManager.h"
#include "Roshi.h"
#include "ChatManager.h"
#include "ScrollManager.h"

Stage1_2::Stage1_2()
{
}

Stage1_2::~Stage1_2()
{
	Release();
}

void Stage1_2::Initialize()
{
	ScrollManager::Get_Instance()->Reset_Scroll();
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Background.bmp", L"Background");

	TileManager::Get_Instance()->Set_EditorId(STAGE1_2);
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

	// Roshi
	ObjManager::Get_Instance()->Add_Object(OBJ_NPC, AbstractFactory<Roshi>::Create(300, 0));

	// Sound
	SoundManager::Get_Instance()->PlayBGM(L"Stage.mp3", .5f);
}

void Stage1_2::Release()
{
	SoundManager::Get_Instance()->StopSound(CHANNEL_BGM);
	TileManager::Get_Instance()->Destroy_Instance();
	ObjManager::Get_Instance()->Destroy_Instance();
	UIManager::Get_Instance()->Destroy_Instance();
	ChatManager::Get_Instance()->Destroy_Instance();
}

int Stage1_2::Update()
{
	TileManager::Get_Instance()->Update();
	ObjManager::Get_Instance()->Update();
	UIManager::Get_Instance()->Update();
	ChatManager::Get_Instance()->Update();

	return 0;
}

void Stage1_2::Late_Update()
{
	TileManager::Get_Instance()->Late_Update();
	ObjManager::Get_Instance()->Late_Update();
	UIManager::Get_Instance()->Late_Update();
	ChatManager::Get_Instance()->Late_Update();

	// If End Stage reached: Go To Boss Stage
	if (!ObjManager::Get_Instance()->Get_Player().empty())
	{
		if (ObjManager::Get_Instance()->Get_Player().front()->Get_Info().fX >= 5500)
		{
			Player* pPlayer = static_cast<Player*>(ObjManager::Get_Instance()->Get_Player().front());
			SceneManager::Get_Instance()->Set_Player(pPlayer);
			SceneManager::Get_Instance()->Change_Scene(SCENE_BOSS_STAGE);
		}
	}
}

void Stage1_2::Render(HDC hDC)
{
	HDC	hGroundDC = BmpManager::Get_Instance()->Find_Bmp(L"Background");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hGroundDC, 0, 0, SRCCOPY);

	TileManager::Get_Instance()->Render(hDC);
	ObjManager::Get_Instance()->Render(hDC);
	UIManager::Get_Instance()->Render(hDC);
	ChatManager::Get_Instance()->Render(hDC);
}