#pragma once

#include "Scene.h"

class BossStage :
	public Scene
{
public:
	BossStage();
	~BossStage();

	void Initialize() override;
	void Release() override;

	// Functions which run every frame
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;

private:
	bool m_bStageClear;
};