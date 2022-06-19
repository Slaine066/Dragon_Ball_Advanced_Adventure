#pragma once

#include "Obj.h"

class Character abstract : public Obj
{
public:
	Character() : m_bIsAttacking(false), m_bCanDamage(false), m_bMotionAlreadyDamaged(false), m_bIsHit(false)
	{ 
		ZeroMemory(&m_tStats, sizeof(m_tStats)); 
		ZeroMemory(&m_tCollisionRect, sizeof(m_tCollisionRect));
	};
	virtual ~Character() {};

	// Getters
	STATS Get_Stats() { return m_tStats; }
	RECT Get_CollisionRect() { return m_tCollisionRect; }
	bool Get_CanDamage() { return m_bCanDamage; }
	bool Get_IsHit() { return  m_bIsHit; }
	
	// Setters
	void Set_Health(int iDamage) { m_tStats.iHealth -= iDamage; };
	void Set_IsAttacking(bool bIsAttacking) { m_bIsAttacking = bIsAttacking; }
	void Set_MotionAlreadyDamaged(bool bAlreadyDamaged) { m_bMotionAlreadyDamaged = bAlreadyDamaged; }
	void Set_IsHit(bool bIsHit = true) { m_bIsHit = bIsHit; }

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

	STATS m_tStats;
	RECT m_tCollisionRect;

	bool m_bIsAttacking;
	bool m_bCanDamage;
	bool m_bMotionAlreadyDamaged;
	bool m_bIsHit;
};