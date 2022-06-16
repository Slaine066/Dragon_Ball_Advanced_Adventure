#pragma once

class Obj;

template<typename T>
class AbstractFactory
{
public:
	AbstractFactory() {}
	~AbstractFactory() {}

	// Empty Instance 
	static Obj*	Create()
	{
		Obj* pInstance = new T;
		pInstance->Initialize();

		return pInstance;
	}

	// Instance with Position, Direction and Owner
	static Obj*	Create(float fX, float fY, DIRID eDir = DIR_END, Obj* pObj = nullptr)
	{
		Obj* pInstance = new T;
		pInstance->Initialize();
		pInstance->Set_Position(fX, fY);

		if (eDir != DIR_END)
			pInstance->Set_Direction(eDir);

		if (pObj)
			pInstance->Set_Owner(pObj);

		return pInstance;
	}
};