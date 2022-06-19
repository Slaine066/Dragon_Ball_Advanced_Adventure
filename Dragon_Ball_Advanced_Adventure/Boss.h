#pragma once

#include "Enemy.h"

class Boss :
	public Enemy
{
public:
	Boss() : m_iPattern(0) {};
	virtual ~Boss() {};

protected:
	int m_iPattern;
};