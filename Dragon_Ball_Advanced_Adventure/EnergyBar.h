#pragma once

#include "Obj.h"

class EnergyBar :
	public Obj
{
public:
	EnergyBar();
	~EnergyBar();

	void Initialize() override;
	void Release() override;


	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
};