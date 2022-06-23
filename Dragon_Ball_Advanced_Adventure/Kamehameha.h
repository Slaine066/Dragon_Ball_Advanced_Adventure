#pragma once

#include "Projectile.h"

class Kamehameha :
	public Projectile
{
public:
	Kamehameha();
	~Kamehameha();

	void Initialize() override;
	void Release() override;

	// Functions which run every frame
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;

	void Set_IsBig() { m_bIsBig = true; m_pFrameKey = L"Kamehameha"; m_iDamage = 20; }

private:
	// Obj overrides
	void Change_Motion() override;

	// Projectile Overrides
	bool Get_CanDamage() override;

	bool m_bIsBig;

	bool m_bFirstTick;
	DWORD m_dwDamageTime;
	DWORD m_dwDuration;
	bool m_bIsLimit;
	bool m_bShouldDestroy;

	void Check_DamageTime();
};