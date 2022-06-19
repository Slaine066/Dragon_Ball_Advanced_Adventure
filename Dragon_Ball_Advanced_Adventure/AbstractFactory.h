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
	static Obj*	Create(float fX, float fY, DIRID eDir = DIR_END, Obj* pOwner = nullptr)
	{
		Obj* pInstance = new T;

		pInstance->Set_Position(fX, fY);

		if (eDir != DIR_END)
			pInstance->Set_Direction(eDir);

		if (pOwner)
			pInstance->Set_Owner(pOwner);

		pInstance->Initialize();
	
		return pInstance;
	}
};