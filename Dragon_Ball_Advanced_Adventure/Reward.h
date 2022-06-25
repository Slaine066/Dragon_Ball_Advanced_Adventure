#pragma once

#include "Obj.h"

class Item;

class Reward :
	public Obj
{
public:
	Reward();
	~Reward();

	void Initialize() override;
	void Release() override;

	// Functions which run every frame
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;

	Item* Get_RewardItem() { return m_pRewardItem; }
	void Set_RewardItem(Item* pRewardItem) { m_pRewardItem = pRewardItem; }

private:
	Item* m_pRewardItem;
};