#pragma once

#include "Define.h"

class UI
{
public:
	UI();
	virtual ~UI();

	virtual void Initialize() PURE;
	virtual void Release() PURE;

	// Functions which run every frame
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC hDC) PURE;

protected:
	void Update_Rect();

	INFO m_tInfo;
	RECT m_tRect;
};

