#pragma once

#include "Obj.h"

class Button : public Obj
{
public:
	Button();
	~Button();

	void Initialize() override;
	void Release() override;

	// Functions which run every frame
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;

	void Set_Blink(bool bShouldBlink) { m_bShouldBlink = bShouldBlink; }

	void Change_Motion() override;

private:
	bool m_bShouldBlink;
	DWORD m_dwTime;
	bool m_bIsVisible;
};