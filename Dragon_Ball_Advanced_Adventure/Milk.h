#pragma once

#include "Item.h"
#include "Character.h"

class Milk :
	public Item
{
public:
	Milk();
	~Milk();

	void Initialize() override;
	void Release() override;

	// Functions which run every frame
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;

	// Item Overrides
	virtual void Item_Effect(Character * pPlayer) override;

private:
	int m_iHealAmount;
};