#include "stdafx.h"
#include "CollisionManager.h"
#include "Character.h"

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Collision_Rect(list<Obj*> _Colliders, list<Obj*> _Collided)
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
				if (IntersectRect(&Rect, &(pCharacter->Get_CollisionRect()), &(Collided->Get_Rect())))
				{
					pCharacter->Set_MotionAlreadyDamaged(true);
					Character* pCollidedCharacter = dynamic_cast<Character*>(Collided);

					if (!pCollidedCharacter)
						return;

					pCollidedCharacter->Set_Health(pCharacter->Get_Stats().iDamage); // Subtract Health

					// If Health <= 0:	Die
					// If Health > 0:	Hit
					if (pCollidedCharacter->Get_Stats().iHealth <= 0)
						pCollidedCharacter->Set_Dead();
					else
						pCollidedCharacter->Set_IsHit();
				}
			}
		}
	}
}