#pragma once

#include "Item.h"

class EnergyDrink :
	public Item
{
public:
	EnergyDrink();
	~EnergyDrink();

	void Initialize() override;
	void Release() override;

	// Functions which run every frame
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;

	// Item Overrides
	void Item_Effect(Character * pPlayer) override;

private:
	int m_iIncreaseAmount;
};

