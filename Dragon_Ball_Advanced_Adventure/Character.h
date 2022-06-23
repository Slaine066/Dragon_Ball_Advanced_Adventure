#pragma once

#include "Obj.h"

class Character abstract : public Obj
{
public:
	Character() : m_bIsAttacking(false), m_bCanDamage(false), m_bMotionAlreadyDamaged(false), m_bIsHit(false), m_bCanPlaySound(false)
	{ 
		ZeroMemory(&m_tStats, sizeof(m_tStats)); 
		ZeroMemory(&m_tCollisionRect, sizeof(m_tCollisionRect));
		srand(time(NULL));
	};
	virtual ~Character() {};

	// Getters
	STATS Get_Stats() { return m_tStats; }
	RECT Get_CollisionRect() { return m_tCollisionRect; }
	bool Get_CanDamage() { return m_bCanDamage; }
	bool Get_IsHit() { return  m_bIsHit; }
	
	// Setters
	void Set_Health(int iDamage) { m_tStats.iHealth -= iDamage; };
	void Increase_Health(int iAmount) 
	{ 
		if ((m_tStats.iHealth + iAmount) > m_tStats.iHealthMax)
			m_tStats.iHealth = m_tStats.iHealthMax;
		else
			m_tStats.iHealth += iAmount;
	}

	void Set_EnergyMax(int iEnergy) { m_tStats.iEnergyMax = iEnergy; }
	void Refull_Energy() { m_tStats.iEnergy = m_tStats.iEnergyMax; }
	void Increase_Energy(int iAmount) { m_tStats.iEnergyMax += iAmount; }

	void Set_IsAttacking(bool bIsAttacking) { m_bIsAttacking = bIsAttacking; }
	void Set_MotionAlreadyDamaged(bool bAlreadyDamaged) { m_bMotionAlreadyDamaged = bAlreadyDamaged; }
	void Set_IsHit(bool bIsHit = true) { m_bIsHit = bIsHit; }

	int Calculate_Damage(int iDamage)
	{
		bool bAddSub = rand() % 2;
		
		if (bAddSub)
			return iDamage + (rand() % m_tStats.iDamageOffset + 1);
		else
			return iDamage - (rand() % m_tStats.iDamageOffset + 1);
	}

	void Update_Collision_Rect(int iColRectStart, int iColRectSize)
	{
		m_tCollisionRect.left = m_eDir == DIR_RIGHT ? m_tInfo.fX + iColRectStart : m_tInfo.fX - iColRectStart - iColRectSize;
		m_tCollisionRect.top = m_tRect.top;
		m_tCollisionRect.right = m_eDir == DIR_RIGHT ? m_tInfo.fX + iColRectStart + iColRectSize : m_tInfo.fX - iColRectStart;
		m_tCollisionRect.bottom = m_tRect.bottom;
	}

protected:
	virtual void Gravity() PURE;
	virtual bool Die() PURE;
	virtual int Get_ColSize() PURE;
	virtual void Can_Damage() PURE;
	virtual void Reset_Animation() PURE;
	virtual void Sound_On_Animation() PURE;

	STATS m_tStats;
	RECT m_tCollisionRect;

	bool m_bIsAttacking;
	bool m_bCanDamage;
	bool m_bMotionAlreadyDamaged;
	bool m_bIsHit;
	bool m_bCanPlaySound;
};