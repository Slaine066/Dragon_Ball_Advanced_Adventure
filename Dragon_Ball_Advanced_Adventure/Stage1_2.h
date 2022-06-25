#pragma once

#include "Scene.h"

class Stage1_2 :
	public Scene
{
public:
	Stage1_2();
	~Stage1_2();

	void Initialize() override;
	void Release() override;

	// Functions which run every frame
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
};