#pragma once

#include "Obj.h"

class Tile :
	public Obj
{
public:
	Tile();
	~Tile();

public:
	const int& Get_DrawID() { return m_iDrawID; }
	const int& Get_Option() { return m_iOption; }

	void Set_DrawID(int _iDrawID) { m_iDrawID = _iDrawID; }
	void Set_Option(int _iOption) { m_iOption = _iOption; }

	void Initialize() override;
	void Release() override;

	// Functions which run every frame
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;

private:
	int	m_iDrawID;
	int	m_iOption;
};