#pragma once

#include "Obj.h"

class Kamehameha :
	public Obj
{
public:
	Kamehameha();
	~Kamehameha();

	void Initialize() override;
	void Release() override;

	// Functions which run every frame
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;


};