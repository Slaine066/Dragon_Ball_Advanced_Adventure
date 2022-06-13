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

	// Instance with Position and Angle
	static Obj* Create(float fX, float fY, float fAngle)
	{
		Obj* pInstance = new T;
		pInstance->Initialize();
		pInstance->Set_Position(fX, fY);
		pInstance->Set_Angle(fAngle);

		return pInstance;
	}

	// Instance with Position and Direction
	static Obj*	Create(float fX, float fY, DIRID eDir = DIR_END)
	{
		Obj* pInstance = new T;
		pInstance->Initialize();
		pInstance->Set_Position(fX, fY);

		if (eDir != DIR_END)
			pInstance->Set_Direction(eDir);

		return pInstance;
	}
};