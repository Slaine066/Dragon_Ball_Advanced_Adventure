#pragma once

typedef struct tagInfo
{
	float fX; // X Axis Location
	float fY; // Y Axis Location
	float fCX; // Width Size
	float fCY; // Heigth Size
} INFO;

typedef struct tagFrameInfo
{
	float fCX; // Width Size
	float fCY; // Height Size
} FRAMEINFO;

typedef struct tagFrame
{
	int iFrameStart;
	int	iFrameEnd;

	int iDamageNotifyStart;
	int iDamageNotifyEnd;

	int	iMotion;

	DWORD dwFrameSpeed;
	DWORD dwFrameTime;
} FRAME;

typedef struct tagStats
{
	int iHealth;
	int iHealthMax;
	int iEnergy;
	int iEnergyMax;
	int iDamage;
	int iSpecialDamage;
} STATS;