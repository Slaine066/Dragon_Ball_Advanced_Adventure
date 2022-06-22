#include "stdafx.h"
#include "Logo.h"
#include "BmpManager.h"
#include "KeyManager.h"
#include "SceneManager.h"
#include "Obj.h"
#include "AbstractFactory.h"
#include "Button.h"
#include "Define.h"
#include "UIManager.h"
#include "SoundManager.h"

extern float g_fSound;

Logo::Logo()
{
}

Logo::~Logo()
{
	Release();
}

void Logo::Initialize()
{
	// TODO:
	// - Add Logo Background Image
	// - Add Logo Title Image
	// - Add Press Start Button Image

	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Logo/Background.bmp", L"Background");
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Logo/Press_Start.bmp", L"Press Start");

	Obj* pObj = AbstractFactory<Button>::Create(400.f, 400.f);
	pObj->Set_FrameKey(L"Press Start");
	static_cast<Button*>(pObj)->Set_Blink(true);
	UIManager::Get_Instance()->Add_Object(UI_BUTTON, pObj);

	SoundManager::Get_Instance()->PlayBGM(L"Logo.mp3", g_fSound / 2);
}

void Logo::Release()
{
	UIManager::Get_Instance()->Delete_Objects(UI_BUTTON);
	SoundManager::Get_Instance()->StopAll();
}

int Logo::Update()
{
	UIManager::Get_Instance()->Update();

	return 0;
}

void Logo::Late_Update()
{
	UIManager::Get_Instance()->Late_Update();
}

void Logo::Render(HDC hDC)
{
	HDC	hBackDC = BmpManager::Get_Instance()->Find_Bmp(L"Background");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);

	UIManager::Get_Instance()->Render(hDC);
}