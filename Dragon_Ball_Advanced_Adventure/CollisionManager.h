#pragma once

#include "Obj.h"

class CollisionManager
{
private:
	CollisionManager();
	~CollisionManager();

public:
	static void	Collision_Damage(list<Obj*> _Colliders, list<Obj*> _Collided);
	static void Collision_Item(list<Obj*> _Colliders, list<Obj*> _Collided);
	static void Collision_Projectile(list<Obj*> _Colliders, list<Obj*> _Collided);
};

