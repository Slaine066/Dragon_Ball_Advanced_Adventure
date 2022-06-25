#pragma once

#include "Scene.h"

class Ending :
	public Scene
{
public:
	Ending();
	~Ending();

	void Initialize() override;
	void Release() override;

	// Functions which run every frame
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;

private:
	HWND m_hVideo;
};