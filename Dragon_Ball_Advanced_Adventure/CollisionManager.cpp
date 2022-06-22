#include "stdafx.h"
#include "CollisionManager.h"
#include "Character.h"
#include "UIManager.h"
#include "DamageNumbers.h"
#include "Enum.h"
#include "AbstractFactory.h"
#include "Kamehameha.h"
#include "Enemy.h"
#include "Player.h"
#include "SoundManager.h"
#include "Item.h"

extern float g_fSound;



CollisionManager::CollisionManager()
{
	srand(time(NULL));
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Collision_Damage(list<Obj*> _Colliders, list<Obj*> _Collided)
{
	RECT Rect{};

	for (auto& Collider : _Colliders)
	{
		Character* pCharacter = dynamic_cast<Character*>(Collider);

		if (!pCharacter)
			return;

		if (pCharacter->Get_CanDamage()) // TRUE when:
											// - FrameStart is between DamageNotifyStart and DamageNotifyEnd;
											// - MotionAlreadyDamaged is FALSE (that is, Character has not applied damage in the current Motion yet).
		{
			for (auto& Collided : _Collided)
			{
				if (!Collided->Get_Dead())
				{
					if (IntersectRect(&Rect, &(pCharacter->Get_CollisionRect()), &(Collided->Get_Rect())))
					{
						Character* pCollidedCharacter = dynamic_cast<Character*>(Collided);

						if (!pCollidedCharacter)
							return;

						int iDamage = pCharacter->Calculate_Damage();

						// Subtract Health
						pCollidedCharacter->Set_Health(iDamage);
						pCharacter->Set_MotionAlreadyDamaged(true);

						// If Collider is Player* - Increase Combo Counter
						Player* pPlayer = dynamic_cast<Player*>(pCharacter);
						if (pPlayer)
						{
							pPlayer->Increase_ComboCounter();
							pPlayer->Set_ComboTime(GetTickCount());
						}
															
						// Spawn DamageNumber UI
						DamageNumbers* pDamageNumber = new DamageNumbers();
						pDamageNumber->Initialize();
						pDamageNumber->Set_Position(pCollidedCharacter->Get_Info().fX, pCollidedCharacter->Get_Info().fY - (pCollidedCharacter->Get_FrameInfoRender().fCY / 2));
						pDamageNumber->Set_Number(iDamage);
						UIManager::Get_Instance()->Add_Object(UI_DAMAGE_NUMBERS, pDamageNumber);

						// If Health <= 0:	Die
						// If Health > 0:	Hit
						if (pCollidedCharacter->Get_Stats().iHealth <= 0)
						{
							pCollidedCharacter->Set_Dead();
							SoundManager::Get_Instance()->PlaySound(L"Hit_Dead.wav", CHANNEL_EFFECT, g_fSound / 2);
						}
						else
						{
							pCollidedCharacter->Set_IsHit();
							SoundManager::Get_Instance()->PlaySound(L"Hit_1.wav", CHANNEL_EFFECT, g_fSound / 2);
						}
					}
				}
			}
		}
	}
}

void CollisionManager::Collision_Item(list<Obj*> _Colliders, list<Obj*> _Collided)
{
	RECT Rect{};

	for (auto& Collider : _Colliders)
	{
		Character* pCharacter = dynamic_cast<Character*>(Collider);
		if (!pCharacter) return;

		for (auto& Collided : _Collided)
		{
			if (IntersectRect(&Rect, &(pCharacter->Get_CollisionRect()), &(Collided->Get_Rect())))
			{
				Item* pItem = dynamic_cast<Item*>(Collided);
				if (!pItem) return;

				pItem->Item_Effect(pCharacter);
				pItem->Item_Sound();
				pItem->Set_Dead();
			}
		}
	}
}

void CollisionManager::Collision_Projectile(list<Obj*> _Colliders, list<Obj*> _Collided)
{
	RECT Rect{};

	for (auto& Collider : _Colliders)
	{
		for (auto& Collided : _Collided)
		{
			if (!Collided->Get_Dead() && !Collider->Get_Owner()->Get_Dead() && !Collider->Get_Dead())
			{
				Character* pColliderOwner = dynamic_cast<Character*>(Collider->Get_Owner());
				if (!pColliderOwner)
					return;
				Character* pCollided = dynamic_cast<Character*>(Collided);
				if (!pCollided)
					return;
				
				if (pColliderOwner != pCollided && pColliderOwner->Get_ObjId() != pCollided->Get_ObjId())
				{
					if (IntersectRect(&Rect, &(Collider->Get_Rect()), &(Collided->Get_Rect())))
					{
						Projectile* pProjectile = dynamic_cast<Projectile*>(Collider);
						if (!pProjectile)
							return;

						Kamehameha* pKamehameha = dynamic_cast<Kamehameha*>(Collider);

						if (pProjectile->Get_CanDamage())
						{
							int iDamage = pColliderOwner->Calculate_Damage(pKamehameha);
							// Subtract Health
							pCollided->Set_Health(iDamage);

							// If Collider is Player* - Increase Combo Counter
							Player* pPlayer = dynamic_cast<Player*>(pColliderOwner);
							if (pPlayer)
							{
								pPlayer->Increase_ComboCounter();
								pPlayer->Set_ComboTime(GetTickCount());
							}

							// Spawn DamageNumber UI
							DamageNumbers* pDamageNumber = new DamageNumbers();
							pDamageNumber->Initialize();
							pDamageNumber->Set_Position(pCollided->Get_Info().fX, pCollided->Get_Info().fY - (pCollided->Get_FrameInfoRender().fCY / 2));
							pDamageNumber->Set_Number(iDamage);
							UIManager::Get_Instance()->Add_Object(UI_DAMAGE_NUMBERS, pDamageNumber);

							// If Health <= 0:	Die
							// If Health > 0:	Hit
							if (pCollided->Get_Stats().iHealth <= 0)
							{
								pCollided->Set_Dead();
								SoundManager::Get_Instance()->PlaySound(L"Hit_Dead.wav", CHANNEL_EFFECT, g_fSound / 2);
							}
							else
							{
								pCollided->Set_IsHit();
								SoundManager::Get_Instance()->PlaySound(L"Hit_1.wav", CHANNEL_EFFECT, g_fSound / 2);
							}
						}

						// If Collider is Kamehameha do NOT destroy Projectile
						if (!pKamehameha)
							Collider->Set_Dead();
					}
				}
			}
		}
	}
}