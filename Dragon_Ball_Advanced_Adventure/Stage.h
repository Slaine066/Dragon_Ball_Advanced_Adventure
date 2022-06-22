#pragma once

#include "Scene.h"

class Stage :
	public Scene
{
public:
	Stage();
	~Stage();

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