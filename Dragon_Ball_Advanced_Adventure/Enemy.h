#pragma once
#include "Character.h"
class Enemy :
	public Character
{
public:
	Enemy();
	virtual ~Enemy();

protected:
	virtual void AI_Behavior() PURE;
};

