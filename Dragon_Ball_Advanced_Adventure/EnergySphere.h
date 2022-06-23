#pragma once

#include "Projectile.h"

class EnergySphere :
	public Projectile
{
public:
	EnergySphere();
	~EnergySphere();

	void Initialize() override;
	void Release() override;

	// Functions which run every frame
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;

public:
	// Obj Overrides
	void Change_Frame() override;

	// Projectile Overrides
	bool Get_CanDamage() override;
};