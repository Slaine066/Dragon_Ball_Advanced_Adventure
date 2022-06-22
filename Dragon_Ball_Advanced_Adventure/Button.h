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

private:
	bool m_bShouldBlink;
	bool m_bShouldChange;
	DWORD m_dwTime;
	DWORD m_dwSoundTime;
	bool m_bIsVisible;
};