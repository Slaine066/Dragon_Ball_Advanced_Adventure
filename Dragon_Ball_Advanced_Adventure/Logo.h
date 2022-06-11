#pragma once
#include "Scene.h"
class Logo :
	public Scene
{
public:
	Logo();
	~Logo();

	void Initialize() override;
	void Release() override;

	// Functions which run every frame
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
};