#pragma once

#include "Obj.h"

class CollisionManager
{
private:
	CollisionManager();
	~CollisionManager();

public:
	static void	Collision_Rect(list<Obj*> _Collider, list<Obj*> _Collided);
};

