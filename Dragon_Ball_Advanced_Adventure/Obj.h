#pragma once

#include "Define.h"
#include "Struct.h"
#include "Enum.h"

class Obj abstract
{
public:
	Obj();
	virtual ~Obj();

	virtual void Initialize() PURE;
	virtual void Release() PURE;

	// Functions which run every frame
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC hDC) PURE;

	// Getters
	const RECT&	Get_Rect() const { return m_tRect; }
	const INFO&	Get_Info() const { return m_tInfo; }
	bool Get_Dead() { return m_bDead; }

	// Setters
	void Set_Position(float fX, float fY) { m_tInfo.fX = fX; m_tInfo.fY = fY; }
	void Set_PositionX(float fX) { m_tInfo.fX += fX; }
	void Set_PositionY(float fY) { m_tInfo.fY += fY; }
	void Set_Direction(DIRID eDir) { m_eDir = eDir; }
	void Set_Dead() { m_bDead = true; }
	void Set_Angle(float fAngle) { m_fAngle = fAngle; }
	void Set_Target(Obj* pTarget) { m_pTarget = pTarget; }
	void Set_FrameKey(TCHAR* pFrameKey) { m_pFrameKey = pFrameKey; }	

protected:
	virtual void Change_Motion();
	virtual void Change_Frame();

	void Update_Rect();

	INFO m_tInfo;
	RECT m_tRect;
	DIRID m_eDir;
	FRAME m_tFrame;

	bool m_bDead;
	float m_fSpeed;
	float m_fAngle;
	float m_fDistance;
	Obj* m_pTarget;
	TCHAR* m_pFrameKey;
};