#pragma once

#include "Define.h"

class Scene abstract
{
public:
	Scene() {};
	virtual ~Scene() {};

	virtual	void Initialize() PURE;
	virtual	void Release() PURE;

	// Functions which run every frame
	virtual	int	Update() PURE;
	virtual	void Late_Update() PURE;
	virtual	void Render(HDC hDC) PURE;
};