#include "stdafx.h"
#include "EnergyBar.h"
#include "Character.h"
#include "ObjManager.h"
#include "BmpManager.h"

EnergyBar::EnergyBar() : m_iMaxEnergy(0), m_iCurrentEnergy(0), m_iCurrentCharge(0), m_fBarLength(0.f)
{
	ZeroMemory(&m_tBackgroundBar, sizeof(m_tBackgroundBar));
	ZeroMemory(&m_tEnergyBar, sizeof(m_tEnergyBar));
	ZeroMemory(&m_tChargingBar, sizeof(m_tChargingBar));
}

EnergyBar::~EnergyBar()
{
}

void EnergyBar::Initialize()
{
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Player/Energy_Bar.bmp", L"Energy_Bar");
	BmpManager::Get_Instance()->Insert_Bmp(L"../Image/Game/Player/Energy_Bar_Small.bmp", L"Energy_Bar_Small");
}

void EnergyBar::Release()
{
}

int EnergyBar::Update()
{
	Update_Bar_Type();
	Update_Bars();
	Update_Rect();

	return 0;
}

void EnergyBar::Late_Update()
{
}

void EnergyBar::Render(HDC hDC)
{
	
	HDC	hMemDC = BmpManager::Get_Instance()->Find_Bmp(m_iMaxEnergy <= 50 ? L"Energy_Bar_Small" : L"Energy_Bar");

	// Test Rectangle
	//Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

	{
		// Background Bar
		HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, myBrush);
		Rectangle(hDC, m_tBackgroundBar.left, m_tBackgroundBar.top, m_tBackgroundBar.right, m_tBackgroundBar.bottom);
		SelectObject(hDC, oldBrush);
		DeleteObject(myBrush);
	}

	{
		// Energy Bar
		HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(248, 248, 0));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, myBrush);
		Rectangle(hDC, m_tEnergyBar.left, m_tEnergyBar.top, m_tEnergyBar.right, m_tEnergyBar.bottom);
		SelectObject(hDC, oldBrush);
		DeleteObject(myBrush);
	}

	{
		// Loading Bar
		HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(176, 22, 42));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, myBrush);
		Rectangle(hDC, m_tChargingBar.left, m_tChargingBar.top, m_tChargingBar.right, m_tChargingBar.bottom);
		SelectObject(hDC, oldBrush);
		DeleteObject(myBrush);
	}

	GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, hMemDC, 0, 0, m_tInfo.fCX, m_tInfo.fCY, RGB(248, 248, 0));
}

void EnergyBar::Update_Bar_Type()
{
	if (!ObjManager::Get_Instance()->Get_Player().empty())
		m_iMaxEnergy = static_cast<Character*>(ObjManager::Get_Instance()->Get_Player().front())->Get_Stats().iEnergyMax;

	if (m_iMaxEnergy == 50)
	{
		m_tInfo.fX = 87.f;
		m_tInfo.fCX = 150.f;
		m_tInfo.fCY = 51.f;

		// Background Bar
		m_tBackgroundBar.left = 60.f;
		m_tBackgroundBar.top = 560.f;
		m_tBackgroundBar.right = 150.f;
		m_tBackgroundBar.bottom = 570.f;

		// Energy Bar
		m_tEnergyBar.left = 60.f;
		m_tEnergyBar.top = 560.f;
		m_tEnergyBar.right = 150.f;
		m_tEnergyBar.bottom = 570.f;

		// Charging Bar
		m_tChargingBar.left = 60.f;
		m_tChargingBar.top = 560.f;
		m_tChargingBar.right = 60.f;
		m_tChargingBar.bottom = 570.f;		
	}
	else if (m_iMaxEnergy == 100)
	{
		m_tInfo.fX = 130.f;
		m_tInfo.fCX = 236.f;
		m_tInfo.fCY = 51.f;

		// Background Bar
		m_tBackgroundBar.left = 60.f;
		m_tBackgroundBar.top = 560.f;
		m_tBackgroundBar.right = 240.f;
		m_tBackgroundBar.bottom = 570.f;

		// Energy Bar
		m_tEnergyBar.left = 60.f;
		m_tEnergyBar.top = 560.f;
		m_tEnergyBar.right = 240.f;
		m_tEnergyBar.bottom = 570.f;

		// Charging Bar
		m_tChargingBar.left = 60.f;
		m_tChargingBar.top = 560.f;
		m_tChargingBar.right = 60.f;
		m_tChargingBar.bottom = 570.f;
	}

	m_fBarLength = m_tEnergyBar.right - m_tChargingBar.left;
}

void EnergyBar::Update_Bars()
{
	// ENERGY
	if (!ObjManager::Get_Instance()->Get_Player().empty())
		m_iCurrentEnergy = static_cast<Character*>(ObjManager::Get_Instance()->Get_Player().front())->Get_Stats().iEnergy;
	if (m_iCurrentEnergy > 0 && m_iCurrentEnergy < m_iMaxEnergy)
		m_tEnergyBar.right = 60.f + (m_fBarLength * m_iCurrentEnergy / m_iMaxEnergy); // Reload the Energy Bar
	else if (m_iCurrentEnergy <= 0)
		m_tEnergyBar.right = 60.f; // Empty the Energy Bar

	// CHARGING
	if (!ObjManager::Get_Instance()->Get_Player().empty())
		m_iCurrentCharge = static_cast<Character*>(ObjManager::Get_Instance()->Get_Player().front())->Get_Stats().iCharge;
	if (m_iCurrentCharge > 0 && m_iCurrentCharge <= m_iMaxEnergy)
		m_tChargingBar.right = 60.f + (m_fBarLength * m_iCurrentCharge / m_iMaxEnergy); // Reload the Charge Bar
	else if (m_iCurrentCharge <= 0)
		m_tChargingBar.right = 60.f; // Empty the Charge Bar
}