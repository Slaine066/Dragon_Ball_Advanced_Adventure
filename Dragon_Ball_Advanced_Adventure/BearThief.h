#pragma once

#include "Boss.h"

class BearThief :
	public Boss
{
public:
	BearThief();
	~BearThief();

	enum STATE {
		IDLE,
		WALK,
		ATTACK,
		ATTACK_SPECIAL,
		HIT,
		DEAD,
		END
	};

	void Initialize() override;
	void Release() override;

	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;

private:
	STATE m_ePreState;
	STATE m_eCurState;

	// Obj Overrides
	void Change_Motion() override;
	void Change_Frame() override;

	// Character Overrides
	void Gravity() override;
	bool Die() override;
	int Get_ColSize() override;
	void Can_Damage() override;
	void Reset_Animation() override;
	
	// Enemy Overrides
	void Find_Target() override;
	void AI_Behavior() override;
	void Move_ToTarget() override;
	void Patrol() override;
};