#pragma once

#include "Enemy.h"

class Boss :
	public Enemy
{
public:
	Boss() : m_iStaggerDamage(0), m_iStaggerLimit(0), m_iPattern(0) {};
	virtual ~Boss() {};

	void Increase_StaggerDamage(int iDamage) { m_iStaggerDamage += iDamage; }
	virtual bool Can_Stagger() PURE;

protected:
	int m_iStaggerDamage;
	int m_iStaggerLimit;
	int m_iPattern;
};