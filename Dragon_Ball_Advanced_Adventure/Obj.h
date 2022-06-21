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
	virtual void Update_Editor();
	virtual void Late_Update() PURE;
	virtual void Render(HDC hDC) PURE;

	// Getters
	const INFO&	Get_Info() const { return m_tInfo; }
	const RECT&	Get_Rect() const { return m_tRect; }
	const FRAMEINFO Get_FrameInfo() const { return m_tFrameInfo; }
	const FRAMEINFO Get_FrameInfoRender() const { return m_tFrameInfoRender; }
	const DIRID Get_Direction() const { return m_eDir; }
	const bool Get_Dead() const { return m_bDead; }
	const Obj* Get_Target() const { return m_pTarget; }
	const OBJID Get_ObjId() const { return m_eObjId; }
	Obj* Get_Owner() const { return m_pOwner; }

	// Setters
	void Set_Position(float fX, float fY) { m_tInfo.fX = fX; m_tInfo.fY = fY; }
	void Set_PositionX(float fX) { m_tInfo.fX += fX; }
	void Set_PositionY(float fY) { m_tInfo.fY += fY; }
	void Set_Direction(DIRID eDir) { m_eDir = eDir; }
	void Set_Dead() { m_bDead = true; }
	void Set_Angle(float fAngle) { m_fAngle = fAngle; }
	void Set_Target(Obj* pTarget) { m_pTarget = pTarget; }
	void Set_FrameKey(TCHAR* pFrameKey) { m_pFrameKey = pFrameKey; }
	void Set_ObjId(OBJID eObjId) { m_eObjId = eObjId; }
	void Set_Owner(Obj* pOwner) { m_pOwner = pOwner; }

protected:
	virtual void Change_Motion();
	virtual void Change_Frame();

	void Update_Rect();

	INFO m_tInfo;
	RECT m_tRect;					// Object Rect 
	FRAMEINFO m_tFrameInfo;			// Frame REAL Size
	FRAMEINFO m_tFrameInfoRender;	// Frame RENDER Size
	FRAME m_tFrame;
	DIRID m_eDir;
	OBJID m_eObjId;
	Obj* m_pOwner;

	bool m_bDead;
	float m_fSpeed;
	float m_fAngle;
	float m_fDistance;
	Obj* m_pTarget;
	TCHAR* m_pFrameKey;
};