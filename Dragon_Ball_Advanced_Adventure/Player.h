#pragma once

#include "Character.h"

class Player :
	public Character
{
public:
	Player();
	virtual ~Player();

	enum STATE { 
		IDLE, 
		RUN, 
		SPRINT, 
		JUMP,
		FALL,
		ATTACK_1,
		ATTACK_2,
		ATTACK_3,
		ATTACK_4,
		ATTACK_5,
		ATTACK_JUMP,
		CHARGING,
		ATTACK_SPECIAL,
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

	STATE Get_CurState() { return m_eCurState; }

private:
	// Obj overrides
	void Change_Motion() override;
	void Change_Frame() override;

	// Character overrides
	void Gravity() override;
	bool Die() override;
	int Get_ColSize() override;
	void Can_Damage() override;
	void Reset_Animation() override;

	void Key_Input();
	void Offset();
	void Move(bool bIsRight);
	void Jump();
	void Attack();
	void Attack_Special();
	void Check_Combo();

	STATE m_ePreState;
	STATE m_eCurState;

	float m_fSprintSpeed;
	float m_fFallSpeed;
	bool m_bIsJumping;

	float m_fJumpPower;
	float m_fJumpTime;
	float m_fAccel;
	
	bool m_bIsComboActive;

	DWORD m_dwChargingTime;
};