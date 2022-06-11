#pragma once
class ScrollManager
{
private:
	ScrollManager();
	~ScrollManager();

	static ScrollManager* m_pInstance;
	float m_fScrollX;
	float m_fScrollY;

public:
	float Get_ScrollX() { return m_fScrollX; }
	float Get_ScrollY() { return m_fScrollY; }

	void Set_ScrollX(float x) { m_fScrollX += x; }
	void Set_ScrollY(float y) { m_fScrollY += y; }

	void Scroll_Lock();

	static ScrollManager* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new ScrollManager;

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
};

