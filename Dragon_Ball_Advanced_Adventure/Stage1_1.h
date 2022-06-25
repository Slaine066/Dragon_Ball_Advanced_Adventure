#pragma once

#include "Scene.h"

class Stage1_1 :
	public Scene
{
public:
	Stage1_1();
	~Stage1_1();

	void Initialize() override;
	void Release() override;

	// Functions which run every frame
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;

private:
	DWORD m_dwGoTime;
	bool m_bShowGo;
};