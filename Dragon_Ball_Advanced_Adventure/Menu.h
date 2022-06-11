#pragma once
#include "Scene.h"
class Menu :
	public Scene
{
public:
	Menu();
	~Menu();

	void Initialize() override;
	void Release() override;

	// Functions which run every frame
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
};

