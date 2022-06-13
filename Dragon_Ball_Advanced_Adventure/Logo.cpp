#include "stdafx.h"
#include "Logo.h"
#include "BmpManager.h"
#include "KeyManager.h"
#include "SceneManager.h"
#include "Obj.h"
#include "AbstractFactory.h"
#include "Button.h"
#include "ObjManager.h"
#include "Define.h"

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
	ObjManager::Get_Instance()->Add_Object(OBJ_UI, pObj);
}

void Logo::Release()
{
	ObjManager::Get_Instance()->Delete_Objects(OBJ_UI);
}

int Logo::Update()
{
	ObjManager::Get_Instance()->Update();

	return 0;
}

void Logo::Late_Update()
{
	ObjManager::Get_Instance()->Late_Update();
}

void Logo::Render(HDC hDC)
{
	HDC	hBackDC = BmpManager::Get_Instance()->Find_Bmp(L"Background");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);

	ObjManager::Get_Instance()->Render(hDC);
}