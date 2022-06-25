#include "stdafx.h"
#include "Stage.h"
#include "ObjManager.h"
#include "AbstractFactory.h"
#include "Player.h"
#include "Roshi.h"
#include "EnergyDrink.h"
#include "BmpManager.h"
#include "ScrollManager.h"
#include "TileManager.h"
#include "SoundManager.h"
#include "UIManager.h"
#include "SceneManager.h"
#include "ChatManager.h"
#include "Chat.h"

extern float g_fSound;

Stage::Stage() : m_dwGoTime(GetTickCount()), m_bShowGo(true)
{
}

Stage::~Stage()
{
	Release();
}

void Stage::Initialize()
{
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Background.bmp", L"Background");
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Go.bmp", L"Go");

	TileManager::Get_Instance()->Load_Tile();
	
	// Player
	ObjManager::Get_Instance()->Add_Object(OBJ_PLAYER, AbstractFactory<Player>::Create(100, 0));

	// Roshi
	ObjManager::Get_Instance()->Add_Object(OBJ_NPC, AbstractFactory<Roshi>::Create(2800, 0));

	// Sound
	SoundManager::Get_Instance()->PlayBGM(L"Stage.mp3", g_fSound / 2);
}

void Stage::Release()
{
	SoundManager::Get_Instance()->StopSound(CHANNEL_BGM);
	TileManager::Get_Instance()->Destroy_Instance();
	ObjManager::Get_Instance()->Destroy_Instance();
	UIManager::Get_Instance()->Destroy_Instance();
	ChatManager::Get_Instance()->Destroy_Instance();
}

int Stage::Update()
{
	TileManager::Get_Instance()->Update();
	ObjManager::Get_Instance()->Update();
	UIManager::Get_Instance()->Update();
	ChatManager::Get_Instance()->Update();

	return 0;
}

void Stage::Late_Update()
{
	TileManager::Get_Instance()->Late_Update();
	ObjManager::Get_Instance()->Late_Update();
	UIManager::Get_Instance()->Late_Update();
	ChatManager::Get_Instance()->Late_Update();

	if (GetTickCount() > m_dwGoTime + 1500)
		m_bShowGo = false;

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

void Stage::Render(HDC hDC)
{
	HDC	hGroundDC = BmpManager::Get_Instance()->Find_Bmp(L"Background");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hGroundDC, 0, 0, SRCCOPY);

	TileManager::Get_Instance()->Render(hDC);
	ObjManager::Get_Instance()->Render(hDC);
	UIManager::Get_Instance()->Render(hDC);
	ChatManager::Get_Instance()->Render(hDC);

	if (m_bShowGo)
	{
		HDC	hMemDC = BmpManager::Get_Instance()->Find_Bmp(L"Go");

		GdiTransparentBlt(
			hDC, 400 - 320 / 4, 200 - 177 / 4, 320 / 2, 177 / 2,
			hMemDC, 0, 0, 320, 177, RGB(132, 0, 132));
	}
}