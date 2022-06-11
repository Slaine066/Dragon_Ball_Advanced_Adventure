#pragma once

#include "Scene.h"

class Game :
	public Scene
{
public:
	Game();
	~Game();

	void Initialize() override;
	void Release() override;

	// Functions which run every frame
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
};