#include "stdafx.h"
#include "PigGunner.h"
#include "BmpManager.h"

PigGunner::PigGunner() : m_ePreState(END), m_eCurState(IDLE), m_bIsAttacking(false)
{
}

PigGunner::~PigGunner()
{
}

void PigGunner::Initialize()
{
	/*m_tInfo.fCX = 100;
	m_tInfo.fCY = 60;*/

	m_fSpeed = 5.f;

	m_pFrameKey = L"Pig_Gunner_RIGHT";

	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Enemy/Pig_Gunner_LEFT.bmp", L"Pig_Gunner_LEFT");
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Enemy/Pig_Gunner_RIGHT.bmp", L"Pig_Gunner_RIGHT");

	Change_Motion();
}

void PigGunner::Release()
{
}

int PigGunner::Update()
{
	return 0;
}

void PigGunner::Late_Update()
{
}

void PigGunner::Render(HDC hDC)
{
}

void PigGunner::Change_Motion()
{
}

void PigGunner::Change_Frame()
{
}

void PigGunner::Gravity()
{
}
