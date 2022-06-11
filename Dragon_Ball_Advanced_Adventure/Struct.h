#pragma once

typedef struct tagInfo
{
	float fX; // X Axis Location
	float fY; // Y Axis Location
	float fCX; // Width Size
	float fCY; // Heigth Size
} INFO;

typedef struct tagFrame
{
	int iFrameStart;
	int	iFrameEnd;
	int	iMotion;

	DWORD dwFrameSpeed;
	DWORD dwFrameTime;
} FRAME;