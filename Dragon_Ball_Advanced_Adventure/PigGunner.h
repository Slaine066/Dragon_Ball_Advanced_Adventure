#pragma once
#include "Enemy.h"

class PigGunner :
	public Enemy
{
public:
	PigGunner();
	~PigGunner();

	enum STATE {
		IDLE,
		WALK,
		ATTACK,
		HIT,
		DEAD,
		END
	};

	void Initialize() override;
	void Release() override;

	// Functions which run every frame
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;

private:
	// Obj Overrides
	void Change_Motion() override;
	void Change_Frame() override;

	// Character Overrides
	bool Die() override;
	int Get_ColSize() override;
	void Can_Damage() override;
	void Reset_Animation() override;

	// Enemy Overrides
	void Find_Target() override;
	void AI_Behavior() override;
	void Move_ToTarget() override;
	void Patrol() override;

	void Gravity();

	STATE m_ePreState;
	STATE m_eCurState;

	bool m_bIsAttacking;
};