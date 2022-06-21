#pragma once

#include "Obj.h"

class ComboCounter :
	public Obj
{
public:
	ComboCounter();
	~ComboCounter();

	virtual void Initialize() override;
	virtual void Release() override;

	// Functions which run every frame
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;\

private:
	void Change_Frame() override;
	void Get_Digit(int iNumber);

	FRAMEINFO m_tNumberFrame;
	FRAMEINFO m_tNumberFrameRender;

	deque<int> m_Numbers;
	int m_iComboCounter;
};