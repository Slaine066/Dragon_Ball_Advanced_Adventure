#pragma once

#include "Obj.h"

class EnergyBar :
	public Obj
{
public:
	EnergyBar();
	~EnergyBar();

	void Initialize() override;
	void Release() override;

	// Functions which run every frame
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;

private:
	RECT m_tBackgroundBar;
	RECT m_tEnergyBar;
	RECT m_tChargingBar;
	
	float m_fBarLength;

	int m_iMaxEnergy;
	int m_iCurrentEnergy;
	int m_iCurrentCharge;

	void Update_Bar_Type();
	void Update_Bars();
};