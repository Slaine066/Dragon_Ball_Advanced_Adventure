#pragma once

#include "Obj.h"
#include "Character.h"

class Bullet :
	public Obj
{
public:
	Bullet();
	~Bullet();

	void Initialize() override;
	void Release() override;

	// Functions which run every frame
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
};