#pragma once

#include "Obj.h"

class DamageNumbers : public Obj
{
public:
	DamageNumbers();
	~DamageNumbers();

	void Initialize() override;
	void Release() override;

	// Functions which run every frame
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;

	void Set_Number(int iNumber) { m_iNumber = iNumber; }

private:
	int m_iNumber;
	DWORD m_dwTime;
};