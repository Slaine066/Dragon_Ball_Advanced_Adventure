#pragma once

#include "Enemy.h"

class Wolf :
	public Enemy
{
public:
	Wolf();
	~Wolf();

	void Initialize() override;
	void Release() override;

	// Functions which run every frame
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;

private:
	// Obj Overrides
	void Change_Frame() override;
	void Change_Motion() override;

	// Character Overrides
	void Gravity() override;
	bool Die() override;
	int Get_ColSize() override;
	void Can_Damage() override;
	void Reset_Animation() override;
	void Sound_On_Animation() override;

	// Enemy Overrides
	void Find_Target() override;
	void AI_Behavior() override;
	void Move_ToTarget() override;
	void Patrol() override;
};