#pragma once

#include "Obj.h"

class HealthBar :
	public Obj
{
public:
	HealthBar();
	~HealthBar();

	void Initialize() override;
	void Release() override;

	// Functions which run every frame
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;

private:
	RECT m_tBackgroundBar;
	RECT m_tBarInfoFirst;
	RECT m_tBarInfoSecond;
	float m_fBarLength;
	int m_iCurrentHealth;
	int m_iMaxHealth;

	void Update_Bars();
};