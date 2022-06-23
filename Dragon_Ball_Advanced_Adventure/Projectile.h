#pragma once

#include "Obj.h"

class Projectile :
	public Obj
{
public:
	Projectile() : m_iDamage(0), m_bCanDamage(true) {};
	virtual ~Projectile() {};

	int Get_Damage() { return m_iDamage; }

	virtual bool Get_CanDamage() PURE;
	void Set_CanDamage(bool bCanDamage) { m_bCanDamage = bCanDamage; };

protected:
	int m_iDamage;
	bool m_bCanDamage;
};