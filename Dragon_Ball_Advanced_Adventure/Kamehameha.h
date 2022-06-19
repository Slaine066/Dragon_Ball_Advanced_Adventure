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

private:
	// Obj overrides
	void Change_Motion() override;

	// Projectile Overrides
	bool Get_CanDamage() override;

	bool m_bFirstTick;
	DWORD m_dwDamageTime;
	DWORD m_dwDuration;
	bool m_bIsLimit;
	bool m_bShouldDestroy;

	void Check_DamageTime();
};