#pragma once

#include "Projectile.h"

class Bullet :
	public Projectile
{
public:
	Bullet();
	~Bullet();

	void Initialize() override;
	void Release() override;

	// Functions which run every frame
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;

	// Projectile Overrides
	bool Get_CanDamage() override;
};