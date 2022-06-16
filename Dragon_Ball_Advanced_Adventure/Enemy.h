#pragma once
#include "Character.h"
class Enemy :
	public Character
{
public:
	Enemy() : m_iWalkRange(0), m_iTargetRange(0), m_iAttackRange(0), m_bIsInAttackRange(0), m_bShouldSwitchDir(false), 
		m_dwAttackTime(GetTickCount()), m_dwIdleTime(GetTickCount()), m_dwWalkTime(GetTickCount())
	{

	};
	virtual ~Enemy() {};

protected:
	int m_iWalkRange;
	int m_iTargetRange;
	int m_iAttackRange;
	bool m_bIsInAttackRange;
	bool m_bShouldSwitchDir;
	DWORD m_dwAttackTime;
	DWORD m_dwIdleTime;
	DWORD m_dwWalkTime;

	virtual void Find_Target() PURE;
	virtual void AI_Behavior() PURE;
	virtual void Move_ToTarget() PURE;
	virtual void Patrol() PURE;
};

