#pragma once

#include "Obj.h"

class Character abstract : public Obj
{
public:
	Character() { ZeroMemory(&m_tStats, sizeof(m_tStats)); ZeroMemory(&m_tCollisionRect, sizeof(m_tCollisionRect)); };
	virtual ~Character() {};

	STATS Get_Stats() { return m_tStats; }

	void Update_Collision_Rect(int iColRectStart, int iColRectSize)
	{
		m_tCollisionRect.left = m_eDir == DIR_RIGHT ? m_tInfo.fX + iColRectStart : m_tInfo.fX - iColRectStart - iColRectSize;
		m_tCollisionRect.top = m_tRect.top;
		m_tCollisionRect.right = m_eDir == DIR_RIGHT ? m_tInfo.fX + iColRectStart + iColRectSize : m_tInfo.fX - iColRectStart;
		m_tCollisionRect.bottom = m_tRect.bottom;
	}

protected:
	STATS m_tStats;
	RECT m_tCollisionRect;

	virtual int Get_ColSize() PURE;
};