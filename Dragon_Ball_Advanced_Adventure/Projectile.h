#pragma once

#include "Obj.h"

class Projectile :
	public Obj
{
public:
	Projectile();
	virtual ~Projectile();

	void Initialize() override;
	void Release() override;

	// Functions which run every frame
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;

	virtual bool Get_CanDamage() PURE;
	void Set_CanDamage(bool bCanDamage) { m_bCanDamage = bCanDamage; };

protected:
	bool m_bCanDamage;
};