#pragma once
class Main
{
public:
	Main();
	~Main();
	
	void Initialize();
	void Release();

	// Functions which run every frame
	void Update();
	void Late_Update();
	void Render();

private:
	HDC	m_hDC;
	DWORD m_dwTime;
	int	m_iFPS;
	TCHAR m_szFPS[64];

	void Render_FPS();
};

