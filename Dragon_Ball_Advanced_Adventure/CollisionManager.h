#pragma once

#include "Obj.h"

class CollisionManager
{
private:
	CollisionManager();
	~CollisionManager();

public:
	static void	Collision_Rect(list<Obj*> _Colliders, list<Obj*> _Collided);
	static void Collision_Bullet(list<Obj*> _Colliders, list<Obj*> _Collided);
};

