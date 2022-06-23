#pragma once

#include "Character.h"

class Roshi :
	public Character
{
public:
	Roshi();
	~Roshi();

	void Initialize() override;
	void Release() override;

	// Functions which run every frame
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;

private:
	// Character Overrides
	void Gravity() override;
	bool Die() override;
	int Get_ColSize() override;
	void Can_Damage() override;
	void Reset_Animation() override;
	void Sound_On_Animation() override;
};