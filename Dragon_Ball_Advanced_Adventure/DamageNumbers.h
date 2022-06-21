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

	void Set_Number(int iNumber) 
	{ 
		m_iNumber = iNumber; 

		if (m_iNumber != 0)
			Get_Digit(m_iNumber);
	}

private:
	void Get_Digit(int iNumber);

	int m_iNumber;
	DWORD m_dwTime;

	deque<int> m_Numbers;
};